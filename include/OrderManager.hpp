#pragma once

#include <memory>
#include <unordered_map>
#include <stdexcept>
#include <type_traits>
#include "Order.hpp"

enum class OrderState {
    NEW,
    PARTIALLY_FILLED,
    FILLED,
    CANCELED
};

template <typename PriceType, typename OrderIdType>
struct OrderInfo {
    std::shared_ptr<Order<PriceType, OrderIdType>> order;
    OrderState state;

    OrderInfo(std::shared_ptr<Order<PriceType, OrderIdType>> ord, OrderState st)
        : order(ord), state(st) {}
};

template <typename PriceType, typename OrderIdType>
class OrderManager {
    static_assert(std::is_integral_v<OrderIdType>, "Order ID must be an integer");

public:
    using OrderPtr = std::shared_ptr<Order<PriceType, OrderIdType>>;
    using InfoMap = std::unordered_map<OrderIdType, OrderInfo<PriceType, OrderIdType>>;

    void addOrder(OrderPtr order);
    void updateOrderState(OrderIdType id, int matched_qty);
    void cancelOrder(OrderIdType id);
    OrderPtr getOrder(OrderIdType id);

private:
    InfoMap orders;
};
