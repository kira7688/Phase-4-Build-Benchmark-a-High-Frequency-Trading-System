#include "../include/MatchingEngine.hpp"

template <typename PriceType, typename OrderIdType>
MatchingEngine<PriceType, OrderIdType>::MatchingEngine() {
    // Pre-allocate to optimize for cache locality and avoid reallocations
    m_matchedTrades.reserve(100000);
}

template <typename PriceType, typename OrderIdType>
std::vector<Trade<PriceType, OrderIdType>> MatchingEngine<PriceType, OrderIdType>::matchOrder(
    std::shared_ptr<Order<PriceType, OrderIdType>> incoming_order,
    OrderBook<PriceType, OrderIdType>& order_book) 
{
    auto start_time = std::chrono::high_resolution_clock::now();
    std::vector<TradeType> new_trades;

    if (incoming_order->is_buy) {
        auto& asks = order_book.getAsks();
        auto it = asks.begin();
        while (it != asks.end() && incoming_order->quantity > 0 && incoming_order->price >= it->first) {
            auto& resting_order = it->second;
            int traded_qty = std::min(incoming_order->quantity, resting_order->quantity);

            incoming_order->quantity -= traded_qty;
            resting_order->quantity -= traded_qty;

            TradeType trade;
            trade.buy_order_id = incoming_order->id;
            trade.sell_order_id = resting_order->id;
            trade.price = resting_order->price;
            trade.quantity = traded_qty;
            new_trades.push_back(trade);

            if (resting_order->quantity == 0) {
                it = asks.erase(it);
            } else {
                ++it;
            }
        }
        if (incoming_order->quantity > 0) {
            order_book.addOrder(incoming_order);
        }
    } else {
        auto& bids = order_book.getBids();
        auto it = bids.begin();
        while (it != bids.end() && incoming_order->quantity > 0 && incoming_order->price <= it->first) {
            auto& resting_order = it->second;
            int traded_qty = std::min(incoming_order->quantity, resting_order->quantity);

            incoming_order->quantity -= traded_qty;
            resting_order->quantity -= traded_qty;

            TradeType trade;
            trade.buy_order_id = resting_order->id;
            trade.sell_order_id = incoming_order->id;
            trade.price = resting_order->price;
            trade.quantity = traded_qty;
            new_trades.push_back(trade);

            if (resting_order->quantity == 0) {
                it = bids.erase(it);
            } else {
                ++it;
            }
        }
        if (incoming_order->quantity > 0) {
            order_book.addOrder(incoming_order);
        }
    }

    auto end_time = std::chrono::high_resolution_clock::now();
    auto latency = std::chrono::duration_cast<std::chrono::nanoseconds>(end_time - start_time).count();

    // If trades were made, we could attach latency to them
    for (auto& trade : new_trades) {
        trade.latency_ns = latency;
        m_matchedTrades.push_back(trade);
    }

    return new_trades;
}

template <typename PriceType, typename OrderIdType>
const std::vector<Trade<PriceType, OrderIdType>>& MatchingEngine<PriceType, OrderIdType>::getMatchedTrades() const {
    return m_matchedTrades;
}

// Explicit template instantiation
template class MatchingEngine<double, int>;
