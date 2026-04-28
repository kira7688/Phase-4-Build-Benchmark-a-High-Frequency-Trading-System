#include "../include/OrderBook.hpp"

template <typename PriceType, typename OrderIdType>
void OrderBook<PriceType, OrderIdType>::addOrder(OrderPtr order) {
    if (order->is_buy) {
        bids.insert({order->price, order});
    } else {
        asks.insert({order->price, order});
    }
}

template <typename PriceType, typename OrderIdType>
void OrderBook<PriceType, OrderIdType>::removeOrder(OrderIdType id, bool is_buy, PriceType price) {
    if (is_buy) {
        auto range = bids.equal_range(price);
        for (auto it = range.first; it != range.second; ++it) {
            if (it->second->id == id) {
                bids.erase(it);
                break;
            }
        }
    } else {
        auto range = asks.equal_range(price);
        for (auto it = range.first; it != range.second; ++it) {
            if (it->second->id == id) {
                asks.erase(it);
                break;
            }
        }
    }
}

template <typename PriceType, typename OrderIdType>
std::multimap<PriceType, std::shared_ptr<Order<PriceType, OrderIdType>>, std::greater<PriceType>>& OrderBook<PriceType, OrderIdType>::getBids() {
    return bids;
}

template <typename PriceType, typename OrderIdType>
std::multimap<PriceType, std::shared_ptr<Order<PriceType, OrderIdType>>, std::less<PriceType>>& OrderBook<PriceType, OrderIdType>::getAsks() {
    return asks;
}

// Explicit template instantiation
template class OrderBook<double, int>;
