#pragma once
#include "Types.h"

#include <iostream>
#include <format>
#include <ranges>

namespace Orderbook {
    struct Trade {
        std::pair<OrderID, OrderID> ids;
        Price price;
        Quantity quantity;
        Time time;
    };
}
