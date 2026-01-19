#include "OrderbookContainer.h"

#include <algorithm>

namespace Orderbook {
    OrderbookMap::OrderbookMap(OrderType orderType)
        : m_orderbookType{ orderType },
        m_orderbook{}
    {
        m_orderbook.reserve(20);
    }

    OrderbookMap::~OrderbookMap()
    {
        for (auto& key : m_orderbook) {
            key.clear();
        }
    }

    auto OrderbookMap::ifContainsGet(OrderID id) -> std::optional<std::shared_ptr<Order>>
    {
        auto found = std::find_if(std::begin(m_orderbook), std::end(m_orderbook), [id](OrderbookKey item) {
            if (auto found = item.find(id); found) {
                return true;
            }
            return false;
            }
        );
        if (found != std::end(m_orderbook)) {
            auto f = m_orderbook.at(std::distance(std::begin(m_orderbook), found)).find(id);
            std::shared_ptr<Order> res = f->order;
            return res;
        }
        return std::nullopt;
    }

    auto OrderbookMap::ifContainsGet(Price price) -> OrderbookKey*
    {
        auto found = std::find_if(std::begin(m_orderbook), std::end(m_orderbook), [price](OrderbookKey item) {return price == item.price;});
        if (found != std::end(m_orderbook)) {
            return &m_orderbook.at(std::distance(std::begin(m_orderbook), found));
        }
        return nullptr;
    }

    auto OrderbookMap::insert(Order tempOrder) -> void
    {
        if (auto* item = ifContainsGet(tempOrder.getPrice()); item) {
            item->pushBack(tempOrder);
            return;
        }
        OrderbookValue obv = { .order{ std::make_shared<Order>(tempOrder) }, .nextOrder{ nullptr } };
        auto* ptr = new OrderbookValue(std::move(obv));
        OrderbookKey item = { .price{tempOrder.getPrice()}, .quantity{tempOrder.getQuantity()}, .orders{ptr} };
        m_orderbook.push_back(item);
        sort();
    }

    auto OrderbookMap::ifContainsRemove(Price price) -> void
    {
        if (auto key = ifContainsGet(price); key) {
            key->clear();
            m_orderbook.erase(std::find_if(std::begin(m_orderbook), std::end(m_orderbook), [price](auto item) {return price == item.price;}));
        }
    }

    auto OrderbookMap::sort() -> void
    {
        if (m_orderbookType == OrderType::BUY) {
            std::ranges::sort(m_orderbook, [](auto a, auto b) {return a.price < b.price;});
        }
        else {
            std::ranges::sort(m_orderbook, [](auto a, auto b) {return a.price > b.price;});
        }
    }

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
}
