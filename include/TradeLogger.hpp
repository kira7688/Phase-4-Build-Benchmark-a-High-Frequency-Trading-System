#pragma once

#include <vector>
#include <iostream>
#include "MatchingEngine.hpp"

template <typename PriceType, typename OrderIdType>
class TradeLogger {
public:
    using TradeType = Trade<PriceType, OrderIdType>;

    TradeLogger();

    void logTrade(const TradeType& trade);
    void logTrades(const std::vector<TradeType>& trades);
    void printStats() const;

private:
    std::vector<TradeType> m_trades;
};
