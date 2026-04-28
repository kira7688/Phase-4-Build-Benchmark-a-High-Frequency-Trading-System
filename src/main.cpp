#include <iostream>
#include <vector>
#include <numeric>
#include <algorithm>
#include <cmath>
#include <memory>
#include "../include/Order.hpp"
#include "../include/Timer.hpp"
#include "../include/OrderBook.hpp"
#include "../include/MatchingEngine.hpp"
#include "../include/OrderManager.hpp"
#include "../include/MarketData.hpp"
#include "../include/TradeLogger.hpp"

using OrderType = Order<double, int>;
using OrderPtr = std::shared_ptr<OrderType>;
using OrderBookType = OrderBook<double, int>;
using MatchingEngineType = MatchingEngine<double, int>;
using OrderManagerType = OrderManager<double, int>;
using TradeLoggerType = TradeLogger<double, int>;

void analyzeLatencies(const std::vector<long long>& latencies) {
    if (latencies.empty()) return;

    auto latencies_copy = latencies;
    std::sort(latencies_copy.begin(), latencies_copy.end());

    auto min = latencies_copy.front();
    auto max = latencies_copy.back();
    double mean = std::accumulate(latencies_copy.begin(), latencies_copy.end(), 0.0) / latencies_copy.size();
    double variance = 0.0;
    for (auto l : latencies_copy) variance += (l - mean) * (l - mean);
    double stddev = std::sqrt(variance / latencies_copy.size());
    long long p99 = latencies_copy[static_cast<int>(latencies_copy.size() * 0.99)];

    std::cout << "Tick-to-Trade Latency (nanoseconds):\n";
    std::cout << "Min: " << min << "\nMax: " << max << "\nMean: " << mean
              << "\nStdDev: " << stddev << "\nP99: " << p99 << "\n";
}

void runLoadScalingExperiment(int num_ticks) {
    std::cout << "\n--- Running Load Scaling Experiment: " << num_ticks << " ticks ---\n";
    std::vector<long long> latencies;
    latencies.reserve(num_ticks);

    OrderBookType order_book;
    MatchingEngineType engine;
    OrderManagerType manager;
    TradeLoggerType logger;

    // Pre-populate some resting orders
    for (int i = 0; i < 5000; ++i) {
        auto resting_bid = std::make_shared<OrderType>(i + num_ticks, "AAPL", 145.0 + (i % 5), 50, true);
        auto resting_ask = std::make_shared<OrderType>(i + num_ticks * 2, "AAPL", 155.0 + (i % 5), 50, false);
        order_book.addOrder(resting_bid);
        order_book.addOrder(resting_ask);
    }

    for (int i = 0; i < num_ticks; ++i) {
        Timer timer;
        timer.start();

        // Simulated tick + order match
        MarketData tick{"AAPL", 149.0, 151.0, std::chrono::high_resolution_clock::now()};
        auto order = std::make_shared<OrderType>(i, tick.symbol, 150.0 + (i % 5), 100, i % 2 == 0);
        
        manager.addOrder(order);
        
        auto trades = engine.matchOrder(order, order_book);
        logger.logTrades(trades);

        int matched_qty = 0;
        for (const auto& t : trades) matched_qty += t.quantity;
        
        manager.updateOrderState(order->id, matched_qty);

        latencies.push_back(timer.stop());
    }

    // Analyze latency
    analyzeLatencies(latencies);
    logger.printStats();
}

int main() {
    std::cout << "Starting HFT System Benchmarks...\n";
    
    // Load scaling experiment
    std::vector<int> tick_loads = {1000, 10000, 100000};
    for (int ticks : tick_loads) {
        runLoadScalingExperiment(ticks);
    }

    return 0;
}