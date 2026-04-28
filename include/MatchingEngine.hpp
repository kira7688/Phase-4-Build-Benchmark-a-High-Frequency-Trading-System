#pragma once

#include <vector>
#include <memory>
#include <chrono>
#include <algorithm>
#include "Order.hpp"

// Forward declaration if needed, or include OrderBook.hpp
#include "OrderBook.hpp"

// Structure to represent a matched trade
template <typename PriceType, typename OrderIdType>
struct Trade {
    OrderIdType buy_order_id;
    OrderIdType sell_order_id;
    PriceType price;
    int quantity;
    long long latency_ns;
};

template <typename PriceType, typename OrderIdType>
class MatchingEngine {
public:
    using OrderPtr = std::shared_ptr<Order<PriceType, OrderIdType>>;
    using TradeType = Trade<PriceType, OrderIdType>;
    using OrderBookType = OrderBook<PriceType, OrderIdType>;

    MatchingEngine();

    // Match order with order book and return matched trades
    std::vector<TradeType> matchOrder(OrderPtr incoming_order, OrderBookType& order_book);

    const std::vector<TradeType>& getMatchedTrades() const;

private:
    // Flat vector for better cache locality when storing past trades
    std::vector<TradeType> m_matchedTrades;
};