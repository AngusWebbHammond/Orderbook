#pragma once

#include "Types.h"
#include "Order.h"
#include "OrderbookContainer.h"
#include "MatchingEngine.h"
#include "Trade.h"

#include <optional>

namespace Orderbook {
    template<SupportsMatching MatchingType>
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

        MatchingEngine<MatchingType> m_matchingEngine;
    };

    template<SupportsMatching MatchingType>
    auto Orderbook<MatchingType>::addOrder(Order& order) -> OrderID
    {
        if (order.getOrderType() == OrderType::BUY) {
            m_buyBook.insert(order);
        }
        else {
            m_sellBook.insert(order);
        }
        return order.getOrderID();
    }

    template<SupportsMatching MatchingType>
    auto Orderbook<MatchingType>::getOrder(OrderID orderID) -> std::optional<std::shared_ptr<Order>>
    {
        auto sellOrder = m_sellBook.ifContainsGet(orderID);
        auto buyOrder = m_buyBook.ifContainsGet(orderID);
        if (sellOrder) {
            return sellOrder.value();
        }
        else if (buyOrder) {
            return buyOrder.value();
        }
        else {
            return std::nullopt;
        }
    }

    template<SupportsMatching MatchingType>
    auto Orderbook<MatchingType>::cancelOrder(OrderID orderID) -> bool
    {
        if (auto order = getOrder(orderID); order) {
            if (order.value()->getOrderType() == OrderType::BUY) {
                OrderbookKey* item = m_buyBook.ifContainsGet(order.value()->getPrice());
                item->erase(orderID);
                return true;
            }
            else {
                auto item = m_sellBook.ifContainsGet(order.value()->getPrice());
                item->erase(orderID);
                return true;
            }
        }
        else {
            return false;
        }
    }

    template<SupportsMatching MatchingType>
    auto Orderbook<MatchingType>::tryMatch() -> std::optional<Trade>
    {
        auto buyFront{ m_buyBook.getFront() };
        auto sellFront{ m_sellBook.getFront() };
        auto trade{ m_matchingEngine.match(buyFront, sellFront) };

        if (buyFront->quantity == 0) {
            m_buyBook.ifContainsRemove(buyFront->price);
        }

        if (sellFront->quantity == 0) {
            m_sellBook.ifContainsRemove(sellFront->price);
        }

        return trade;
    }
}