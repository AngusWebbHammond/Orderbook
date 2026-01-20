#pragma once
#include "Trade.h"

#include <concepts>

namespace Orderbook {
    class OrderbookKey;

    template<typename MatchingType>
    concept SupportsMatching = requires(MatchingType arg, OrderbookKey * key1, OrderbookKey * key2) {
        arg.match(key1, key2);
        { arg.match(key1, key2) } -> std::same_as<std::optional<Trade>>;
    };
}
