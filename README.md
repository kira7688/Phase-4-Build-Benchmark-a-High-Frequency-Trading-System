# HFT Trading System System

A performance-optimized High-Frequency Trading (HFT) system prototype in C++.

## 📝 Design Overview & Architecture
This system is composed of several independent components structured for low-latency market simulation and order processing. The system measures the "Tick-to-Trade" latency via the `std::chrono::high_resolution_clock` to benchmark its efficiency.

### System Architecture Diagram
```text
+------------------+       +-----------------+       +------------------+
| MarketDataFeed   |  -->  | OrderBook       | <---> | OrderManagement  |
+------------------+       +-----------------+       +------------------+
                                 |     |
                                 v     v
                         +-------------------+      +------------------+
                         | MatchingEngine    | ---> | TradeLogger      |
                         +-------------------+      +------------------+
```
- **MarketDataFeed:** Simulates high-frequency incoming ticks.
- **OrderBook:** Maintains open bids and asks in price-priority structures via `<map>` and `std::shared_ptr`.
- **OrderManager:** Stores the tracking mapping of live order statuses (`NEW`, `FILLED`, etc.).
- **MatchingEngine:** Consumes inbound orders against resting orders, resolving to flat trade structures for strong temporal and spatial cache locality.
- **TradeLogger:** Flushes and reports grouped match output asynchronously (or locally in batches).

## 🚀 Build Instructions (CMake)
```bash
mkdir build
cd build
cmake ..
cmake --build .
```

## 📉 Running the Application
```bash
./build/Debug/hft_app.exe  # On Windows
./build/hft_app            # On Unix-based systems
```
Running this standard executable automatically performs a continuous *Load Scaling* performance test and reports latency statistics per scaled generation (e.g., 1k, 10k, 100k ticks).

## 🗂 Final Deliverables Checklist
- [x] Modular source code (Separate `.hpp` and `.cpp` files in `include/` and `src/`).
- [x] README covering architecture, build instructions, and design breakdown.
- [x] Benchmark report and statistics located in `BenchmarkReport.md` providing testing answers.
- [x] Performance test results built directly inside `main.cpp` running scale comparisons.
- [x] Architecture diagram textually enclosed above.
