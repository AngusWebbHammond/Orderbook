#pragma once

#include "Types.h"
#include "Order.h"

#include <memory>
#include <optional>
#include <vector>
#include <ranges>
#include <algorithm>
#include <concepts>

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
        auto getFront() -> std::shared_ptr<Order>;
    };

    template<OrderType BookType>
    class OrderbookMap {
    public:
        OrderbookMap();
        OrderbookMap(const OrderbookMap&) = delete;
        OrderbookMap(OrderbookMap&&) = delete;
        auto operator=(const OrderbookMap&) -> void = delete;
        auto operator=(OrderbookMap&&) -> void = delete;
        ~OrderbookMap();

        auto ifContainsGet(OrderID id) -> std::optional<std::shared_ptr<Order>>;
        auto ifContainsGet(Price price) -> OrderbookKey*;
        auto insert(Order tempOrder) -> void;
        auto ifContainsRemove(Price price) -> void;
        auto getFront() -> OrderbookKey*;

    private:
        auto sort() -> void;

        std::vector<OrderbookKey> m_orderbook;
    };

    template<OrderType BookType>
    OrderbookMap<BookType>::OrderbookMap()
        : m_orderbook{}
    {
        m_orderbook.reserve(20);
    }

    template<OrderType BookType>
    OrderbookMap<BookType>::~OrderbookMap()
    {
        for (auto& key : m_orderbook) {
            key.clear();
        }
    }

    template<OrderType BookType>
    auto OrderbookMap<BookType>::ifContainsGet(OrderID id) -> std::optional<std::shared_ptr<Order>>
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

    template<OrderType BookType>
    auto OrderbookMap<BookType>::ifContainsGet(Price price) -> OrderbookKey*
    {
        auto found = std::find_if(std::begin(m_orderbook), std::end(m_orderbook), [price](OrderbookKey item) {return price == item.price;});
        if (found != std::end(m_orderbook)) {
            return &m_orderbook.at(std::distance(std::begin(m_orderbook), found));
        }
        return nullptr;
    }

    template<OrderType BookType>
    auto OrderbookMap<BookType>::insert(Order tempOrder) -> void
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

    template<OrderType BookType>
    auto OrderbookMap<BookType>::ifContainsRemove(Price price) -> void
    {
        if (auto key = ifContainsGet(price); key) {
            key->clear();
            m_orderbook.erase(std::find_if(std::begin(m_orderbook), std::end(m_orderbook), [price](auto item) {return price == item.price;}));
        }
    }

    template<OrderType BookType>
    auto OrderbookMap<BookType>::getFront() -> OrderbookKey*
    {
        if (m_orderbook.size() > 0) {
            return &m_orderbook.front();
        }
        return nullptr;
    }

    template<OrderType BookType>
    auto OrderbookMap<BookType>::sort() -> void
    {
        if constexpr (BookType == OrderType::BUY) {
            std::ranges::sort(m_orderbook, [](const auto& a, const auto& b) {return a.price > b.price;});
        }
        else {
            std::ranges::sort(m_orderbook, [](const auto& a, const auto& b) {return a.price < b.price;});
        }
    }
}