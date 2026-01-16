#pragma once

#include "Types.h"
#include "Order.h"

#include <memory>
#include <optional>
#include <vector>

namespace Orderbook {
    struct OrderbookValue {
        std::shared_ptr<Order> order;
        OrderbookValue* nextOrder;
    };

    struct OrderbookKey {
        Price price;
        Quantity quantity;
        OrderbookValue* orders;

        auto find(OrderID id) -> OrderbookValue*;
        auto findPrevious(OrderID id) -> std::optional<OrderbookValue*>;
        auto pushBack(Order tempOrder) -> void;
        auto erase(OrderID id) -> void;
        auto clear() -> void;
    };

    class OrderbookMap {
    public:
        OrderbookMap(OrderType);
        OrderbookMap(const OrderbookMap&) = delete;
        OrderbookMap(OrderbookMap&&) = delete;
        auto operator=(const OrderbookMap&) -> void = delete;
        auto operator=(OrderbookMap&&) -> void = delete;
        ~OrderbookMap();

        auto ifContainsGet(OrderID id) -> std::optional<std::shared_ptr<Order>>;
        auto ifContainsGet(Price price) -> OrderbookKey*;
        auto insert(Order tempOrder) -> void;
        auto ifContainsRemove(Price price) -> void;

    private:
        auto sort() -> void;

        OrderType m_orderbookType;
        std::vector<OrderbookKey> m_orderbook;
    };
}