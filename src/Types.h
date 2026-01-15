#pragma once
#include <cstdint>

namespace Orderbook {
    using Price = long double;
    using Quantity = uint32_t;
    using OrderID = uint64_t;

    enum class OrderType {
        BUY,
        SELL
    };
}