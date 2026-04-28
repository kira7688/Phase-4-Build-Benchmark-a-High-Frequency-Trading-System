#include "../include/TradeLogger.hpp"

template <typename PriceType, typename OrderIdType>
TradeLogger<PriceType, OrderIdType>::TradeLogger() {
    m_trades.reserve(100000);
}

template <typename PriceType, typename OrderIdType>
void TradeLogger<PriceType, OrderIdType>::logTrade(const TradeType& trade) {
    m_trades.push_back(trade);
}

template <typename PriceType, typename OrderIdType>
void TradeLogger<PriceType, OrderIdType>::logTrades(const std::vector<TradeType>& trades) {
    for (const auto& trade : trades) {
        logTrade(trade);
    }
}

template <typename PriceType, typename OrderIdType>
void TradeLogger<PriceType, OrderIdType>::printStats() const {
    std::cout << "Logged " << m_trades.size() << " trades." << std::endl;
    // Batch logging could write to a file or database here.
}

// Explicit template instantiation
template class TradeLogger<double, int>;
