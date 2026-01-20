#pragma once
#include "Types.h"

namespace Orderbook {
    struct Trade {
        std::pair<OrderID, OrderID> ids;
        Price price;
        Quantity quantity;
        Time time;
    };
}