#pragma once
#include "Order.h"
#include "OrderbookContainer.h"
#include "Trade.h"

#include <concepts>

namespace Orderbook {
    template<typename Arg>
    concept SupportsMatching = requires(Arg arg, OrderbookKey * key1, OrderbookKey * key2) {
        arg.match(key1, key2);
        { arg.match(key1, key2) } -> std::same_as<std::optional<Trade>>;
    };

    class FIFO {
    public:
        auto match(OrderbookKey*, OrderbookKey*) -> std::optional<Trade>;
    };

    template<SupportsMatching Arg>
    class MatchingEngine {
    public:
        auto match(OrderbookKey*, OrderbookKey*) -> std::optional<Trade>;
    private:
        Arg m_matchingType;
    };

    template<SupportsMatching Arg>
    inline auto MatchingEngine<Arg>::match(OrderbookKey* key1, OrderbookKey* key2) -> std::optional<Trade>
    {
        return m_matchingType.match(key1, key2);
    }
}