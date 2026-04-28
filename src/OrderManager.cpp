#include "../include/OrderManager.hpp"
#include <stdexcept>

template <typename PriceType, typename OrderIdType>
void OrderManager<PriceType, OrderIdType>::addOrder(OrderPtr order) {
    if (orders.find(order->id) != orders.end()) {
        throw std::invalid_argument("Order already exists");
    }
    orders.emplace(order->id, OrderInfo<PriceType, OrderIdType>(order, OrderState::NEW));
}

template <typename PriceType, typename OrderIdType>
void OrderManager<PriceType, OrderIdType>::updateOrderState(OrderIdType id, int matched_qty) {
    auto it = orders.find(id);
    if (it != orders.end()) {
        if (it->second.order->quantity == 0) {
            it->second.state = OrderState::FILLED;
        } else if (matched_qty > 0) {
            it->second.state = OrderState::PARTIALLY_FILLED;
        }
    }
}

template <typename PriceType, typename OrderIdType>
void OrderManager<PriceType, OrderIdType>::cancelOrder(OrderIdType id) {
    auto it = orders.find(id);
    if (it != orders.end()) {
        it->second.state = OrderState::CANCELED;
    }
}

template <typename PriceType, typename OrderIdType>
std::shared_ptr<Order<PriceType, OrderIdType>> OrderManager<PriceType, OrderIdType>::getOrder(OrderIdType id) {
    auto it = orders.find(id);
    if (it != orders.end()) {
        return it->second.order;
    }
    return nullptr;
}

// Explicit template instantiation
template class OrderManager<double, int>;
