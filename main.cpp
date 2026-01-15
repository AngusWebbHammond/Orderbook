#include "src/Orderbook.h"

// -------------------------------------------------------------------------------
// Todo list
// -------------------------------------------------------------------------------
// TODO implement a way of issuing trades 
//      This should allow the trades to go off the best price on each orderbook
// TODO test this with real market data
// TODO Add a csv parser for incoming trades, for example:
//      "S, 10, 10, ID" : Sell 10 at 10 price
//      "B, 100, 9, ID" : Buy 100 at 9 price
// TODO Implement a way of switching between FIFO and Pro-Rata trades
// TODO Add a UserID system, which will tie a user to a trade
// TODO Add robust tests
// -------------------------------------------------------------------------------

int main() {
    Orderbook::Orderbook orderbook;

    Orderbook::Order orderBuy1(10, 10, Orderbook::OrderType::BUY);
    Orderbook::Order orderBuy2(12, 10, Orderbook::OrderType::BUY);
    Orderbook::Order orderSell1(20, 31, Orderbook::OrderType::SELL);
    Orderbook::Order orderSell2(20, 32, Orderbook::OrderType::SELL);
    orderbook.addOrder(orderBuy1);
    orderbook.addOrder(orderBuy2);
    orderbook.addOrder(orderSell1);
    orderbook.addOrder(orderSell2);

    orderbook.cancelOrder(0);
    orderbook.cancelOrder(2);

    orderbook.addOrder(orderBuy1);
    return 0;
}