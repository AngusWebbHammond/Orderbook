#include "MatchingEngine.h"
#include "Trade.h"
#include "OrderbookContainer.h"

#include <chrono>

namespace Orderbook
{
    auto FIFO::match(OrderbookKey* buy, OrderbookKey* sell) -> std::optional<Trade>
    {
        if (buy->price >= sell->price) {
            auto frontBuyOrder{ buy->getFront() };
            auto frontSellOrder{ sell->getFront() };
            Quantity quantity{ std::min(frontBuyOrder->getQuantity(), frontSellOrder->getQuantity()) };

            Trade trade{ .ids{frontBuyOrder->getOrderID(), frontSellOrder->getOrderID()}, .price{sell->price}, .quantity{quantity}, .time{std::chrono::steady_clock::now()} };

            if (frontBuyOrder->performTrade(quantity) == 0) {
                buy->erase(frontBuyOrder->getOrderID());
            }
            if (frontSellOrder->performTrade(quantity) == 0) {
                sell->erase(frontSellOrder->getOrderID());
            }

            sell->quantity -= quantity;
            buy->quantity -= quantity;

            return trade;
        }
        else {
            return std::nullopt;
        }
    }
}