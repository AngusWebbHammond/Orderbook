#pragma once
#include "Fwd.h"

#include <concepts>
#include <optional>

namespace Orderbook {
    class FIFO {
    public:
        auto match(OrderbookKey*, OrderbookKey*) -> std::optional<Trade>;
    };

    template<SupportsMatching MatchingType>
    class MatchingEngine {
    public:
        auto match(OrderbookKey*, OrderbookKey*) -> std::optional<Trade>;
    private:
        MatchingType m_matchingType;
    };

    template<SupportsMatching MatchingType>
    inline auto MatchingEngine<MatchingType>::match(OrderbookKey* key1, OrderbookKey* key2) -> std::optional<Trade>
    {
        return m_matchingType.match(key1, key2);
    }
}