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

template<>
struct std::formatter<Orderbook::Trade> {
    constexpr auto parse(auto& ctx) { return ctx.begin(); }

    auto format(const Orderbook::Trade& trade, auto& ctx) const {
        return std::format_to(
            ctx.out(),
            "Sell ID: {}, Buy ID: {}, Price: {}, Quantity: {}",
            trade.ids.first,
            trade.ids.second,
            trade.price,
            trade.quantity
        );
    }
};
