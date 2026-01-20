#pragma once

#include "Types.h"
#include "HelperFunctions.h"

namespace Orderbook {
    class Order {
    public:
        Order(Price price,
            Quantity quantity,
            OrderType orderType,
            OrderID  orderID = generateOrderID())
            : m_orderID{ orderID },
            m_price{ price },
            m_quantity{ quantity },
            m_orderType{ orderType } {
        };
        Order(const Order&) = default;
        Order(Order&&) = default;
        auto operator=(const Order&) -> Order &
            = default;
        auto operator=(Order&&) -> Order &
            = default;
        ~Order() = default;

        auto operator==(Order other) -> bool { return other.m_orderID == m_orderID; }

        [[nodiscard]] auto getPrice() -> Price { return m_price; }
        [[nodiscard]] auto getQuantity() -> Quantity& { return m_quantity; }
        [[nodiscard]] auto getOrderType() -> OrderType { return m_orderType; }
        [[nodiscard]] auto getOrderID() -> OrderID { return m_orderID; }

        auto performTrade(Quantity) -> Quantity;

    private:
        OrderID m_orderID{};
        Price m_price{};
        Quantity m_quantity{};
        OrderType m_orderType{};
    };
}