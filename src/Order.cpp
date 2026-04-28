#include "../include/Order.hpp"
#include <utility>

template <typename PriceType, typename OrderIdType>
Order<PriceType, OrderIdType>::Order(OrderIdType id, std::string sym, PriceType pr, int qty, bool buy)
    : id(id), symbol(std::move(sym)), price(pr), quantity(qty), is_buy(buy) {
}

// Explicit template instantiation
template struct Order<double, int>;


