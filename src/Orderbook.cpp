#include "Orderbook.h"

namespace Orderbook
{
    auto Orderbook::addOrder(Order& order) -> OrderID
    {
        if (order.getOrderType() == OrderType::BUY) {
            m_buyBook.insert(order);
        }
        else {
            m_sellBook.insert(order);
        }
        return order.getOrderID();
    }

    auto Orderbook::getOrder(OrderID orderID) -> std::optional<std::shared_ptr<Order>>
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

    auto Orderbook::cancelOrder(OrderID orderID) -> bool
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

    auto Orderbook::tryMatch() -> std::optional<Trade>
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
