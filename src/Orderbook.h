#pragma once

#include "Types.h"
#include "Order.h"
#include "OrderbookContainer.h"
#include "MatchingEngine.h"
#include "Trade.h"

#include <optional>

namespace Orderbook {
    class Orderbook {
    public:
        Orderbook() = default;
        Orderbook(const Orderbook&) = delete;
        Orderbook(Orderbook&&) = delete;
        void operator=(const Orderbook&) = delete;
        void operator=(Orderbook&&) = delete;
        ~Orderbook() = default;

        auto addOrder(Order&) -> OrderID;
        auto getOrder(OrderID orderID) -> std::optional<std::shared_ptr<Order>>;
        auto cancelOrder(OrderID orderID) -> bool;

        auto tryMatch() -> std::optional<Trade>;

    private:
        OrderbookMap m_buyBook{ OrderType::BUY };
        OrderbookMap m_sellBook{ OrderType::SELL };

        MatchingEngine<FIFO> m_matchingEngine;
    };
}