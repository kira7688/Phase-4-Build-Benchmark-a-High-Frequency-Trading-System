#include <iostream>
#include <vector>
#include <cmath>
#include <numeric>
#include <algorithm>
#include "../include/Timer.hpp"
#include "../include/OrderBook.hpp"
#include "../include/MatchingEngine.hpp"

using OrderType = Order<double, int>;

int test_main() {
    std::cout << "Running Latency Tests...\n";
    std::vector<long long> latencies;
    int test_runs = 10000;
    
    OrderBook<double, int> order_book;
    MatchingEngine<double, int> engine;
    
    // Add resting orders
    for(int i=0; i<1000; ++i) {
        order_book.addOrder(std::make_shared<OrderType>(i, "SYM", 100.0, 10, true));
    }
    
    for(int i=0; i<test_runs; ++i) {
        Timer timer;
        timer.start();
        
        auto order = std::make_shared<OrderType>(i+1000, "SYM", 100.0, 1, false);
        engine.matchOrder(order, order_book);
        
        latencies.push_back(timer.stop());
    }
    
    double mean = std::accumulate(latencies.begin(), latencies.end(), 0.0) / latencies.size();
    
    std::cout << "Mean Latency: " << mean << " ns\n";
    return 0;
}
