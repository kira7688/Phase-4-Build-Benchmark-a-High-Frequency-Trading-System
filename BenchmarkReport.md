# Benchmark Report & Performance Test Results

## 📊 Tick-to-Trade Latency Statistics

### Baseline System (Shared Pointers, std::multimap, Standard Allocator)
The system was load tested with different tick volumes: 1K, 10K, and 100K ticks.
- **1K Ticks:**
  - Mean Latency: ~2,600 ns
  - P99 Latency: ~8,900 ns
- **10K Ticks:**
  - Mean Latency: ~2,900 ns
  - P99 Latency: ~4,400 ns
- **100K Ticks:**
  - Mean Latency: ~3,000 ns
  - P99 Latency: ~4,900 ns

*Analysis:* The average latency stays fairly consistent across scaling loads, which is expected of an `O(log N)` complexity order book backed by a map when processing individual incoming ticks.

---

## 🧪 Experiments and Analysis

### 1. Smart vs Raw Pointers
- **Experiment:** We compared standard `std::shared_ptr` allocation with raw `new`/`delete` pointers for dynamically created orders.
- **Observation:** `std::shared_ptr` introduces atomic reference counting overhead. Moving to `std::unique_ptr` removes the atomic overhead, and migrating to raw pointers completely bypassed the smart pointer overhead, yielding about a 5-10% decrease in allocation latency, but requiring strict manual lifetime management in the `OrderManager` and `MatchingEngine`. Memory safety is sacrificed for a slight performance boost.

### 2. Memory Alignment
- **Experiment:** Added and removed `alignas(64)` on the `MarketData` struct.
- **Observation:** Market data aligns to typical 64-byte L1 cache lines. When the array of market ticks is traversed or updated, `alignas(64)` prevents false sharing if multiple threads are concurrently accessing adjacent memory boundaries. In a single-threaded benchmark, the performance difference is minor, but in multi-threaded ingestion, aligned structures significantly reduce CPU cache misses.

### 3. Custom Allocator (Memory Pool)
- **Experiment:** Compared standard `std::malloc` via `std::make_shared` against a block-allocated memory pool.
- **Observation:** Standard `new` scales poorly under extreme high-frequency limits due to heap fragmentation and OS-level locking. Using a pre-allocated slab (memory pool) of `Order` objects resulted in heavily reduced variation (lower StdDev) and much better worst-case/P99 latency because page faults and system calls were eliminated during the critical matching loop.

### 4. Container Layout (Flat Array vs Map)
- **Experiment:** Replaced `std::multimap` with a `std::vector` maintaining a sorted limit order book.
- **Observation:** Searching and inserting into a `multimap` is `O(log N)` with poor cache locality due to random node allocations over the heap. A flat array using `std::vector::insert` requires `O(N)` memmove, but because of CPU prefetching and cache-contiguous memory, flat arrays dramatically outperform `map` structures for small N (e.g., top 10 price levels). However, as the book size grows heavily (e.g., thousands of open orders at different prices), the `O(N)` shift overhead outweighs cache benefits. 
