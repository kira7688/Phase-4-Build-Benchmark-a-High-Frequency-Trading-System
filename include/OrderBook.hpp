#pragma once

#include <vector>
#include <map>
#include <memory>
#include <algorithm>
#include "Order.hpp"

template <typename PriceType, typename OrderIdType>
class OrderBook {
public:
    using OrderPtr = std::shared_ptr<Order<PriceType, OrderIdType>>;

    void addOrder(OrderPtr order);

    void removeOrder(OrderIdType id, bool is_buy, PriceType price);

    // High to low for bids (descending)
    std::multimap<PriceType, OrderPtr, std::greater<PriceType>>& getBids();
    // Low to high for asks (ascending)
    std::multimap<PriceType, OrderPtr, std::less<PriceType>>& getAsks();

private:
    std::multimap<PriceType, OrderPtr, std::greater<PriceType>> bids;
    std::multimap<PriceType, OrderPtr, std::less<PriceType>> asks;
};
