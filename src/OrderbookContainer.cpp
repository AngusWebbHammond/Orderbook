#include "OrderbookContainer.h"

#include <algorithm>

namespace Orderbook {


    auto OrderbookKey::find(OrderID id) -> OrderbookValue*
    {
        OrderbookValue* ptr = orders;
        while (ptr) {
            if (ptr->order->getOrderID() == id) {
                return ptr;
            }
            if (ptr->nextOrder) {
                ptr = ptr->nextOrder;
            }
            else {
                return nullptr;
            }
        }
        return nullptr;
    }

    auto OrderbookKey::findPrevious(OrderID id) -> std::optional<OrderbookValue*>
    {
        OrderbookValue* ptr = orders;
        while (ptr) {
            if (ptr->nextOrder) {
                if (ptr->nextOrder->order->getOrderID() == id) {
                    return ptr;
                }

                ptr = ptr->nextOrder;
            }
            else {
                if (orders->order->getOrderID() == id) {
                    return nullptr;
                }
                return std::nullopt;
            }
        }
        return std::nullopt;
    }

    auto OrderbookKey::pushBack(Order tempOrder) -> void
    {
        OrderbookValue* ptr = orders;
        quantity += tempOrder.getQuantity();
        while (ptr) {
            if (ptr->nextOrder) {
                ptr = ptr->nextOrder;
            }
            else {
                OrderbookValue obv = { .order = std::make_shared<Order>(tempOrder), .nextOrder = nullptr };
                ptr->nextOrder = new OrderbookValue(obv);
                return;
            }
        }
        OrderbookValue obv = { .order = std::make_shared<Order>(tempOrder), .nextOrder = nullptr };
        orders = new OrderbookValue(obv);
        return;
    }

    auto OrderbookKey::erase(OrderID id) -> void
    {
        if (auto value = findPrevious(id); value) {
            if (value.value()) {
                auto* valueNext = value.value()->nextOrder;
                value.value()->nextOrder = value.value()->nextOrder->nextOrder;
                quantity -= valueNext->order->getQuantity();
                delete valueNext;
            }
            else {
                quantity -= orders->order->getQuantity();
                OrderbookValue* orderValue = orders;
                OrderbookValue* nextValue = orderValue->nextOrder;
                delete orderValue;
                orders = nextValue;
            }
        }
        auto a = 1;
    }

    auto OrderbookKey::clear() -> void
    {
        auto* ptr = orders;
        while (ptr) {
            auto* nextPtr = ptr->nextOrder;
            if (!nextPtr) {
                delete ptr;
                ptr = nullptr;
                orders = nullptr;
                quantity = 0;
                return;
            }
            else {
                delete ptr;
                ptr = nextPtr;
            }
        }
    }

    auto OrderbookKey::getFront() -> std::shared_ptr<Order>
    {
        return orders->order;
    }
}
