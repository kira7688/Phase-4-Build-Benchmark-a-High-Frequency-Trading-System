# HFT Trading System System

A performance-optimized High-Frequency Trading (HFT) system prototype in C++.
This system implements the following components:
- Market Data Feed Simulator
- OrderBook (Template-Based, O(log N) price levels using maps)
- Order Management System (OMS)
- Matching Engine
- Trade Logger

## Architecture

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

## Build Instructions (CMake)
```bash
mkdir build
cd build
cmake ..
cmake --build .
```

## Running the Application
```bash
./hft_app
```

The app simulates market data ticks, generates orders, attempts to match them, and logs the tick-to-trade latencies.
