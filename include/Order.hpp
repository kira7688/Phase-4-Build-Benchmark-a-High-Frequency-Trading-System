#pragma once
#include <string>
#include <memory>

template <typename PriceType, typename OrderIdType>
struct Order {
    OrderIdType id;
    std::string symbol;
    PriceType price;
    int quantity;
    bool is_buy;

    Order(OrderIdType id, std::string sym, PriceType pr, int qty, bool buy);
};