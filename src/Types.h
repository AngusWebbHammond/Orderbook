#pragma once
#include <cstdint>
#include <chrono>

namespace Orderbook {
    using Price = long double;
    using Quantity = uint32_t;
    using OrderID = uint64_t;
    using Time = std::chrono::steady_clock::time_point;

    enum class OrderType {
        BUY,
        SELL
    };
}