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

    auto min = *std::min_element(latencies.begin(), latencies.end());
    auto max = *std::max_element(latencies.begin(), latencies.end());
    double mean = std::accumulate(latencies.begin(), latencies.end(), 0.0) / latencies.size();
    double variance = 0.0;
    for (auto l : latencies) variance += (l - mean) * (l - mean);
    double stddev = std::sqrt(variance / latencies.size());
    long long p99 = latencies[static_cast<int>(latencies.size() * 0.99)];

    std::cout << "Tick-to-Trade Latency (nanoseconds):\n";
    std::cout << "Min: " << min << "\nMax: " << max << "\nMean: " << mean
              << "\nStdDev: " << stddev << "\nP99: " << p99 << "\n";
}

int main() {
    std::vector<long long> latencies;
    const int num_ticks = 10000;
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

    return 0;
}