#include "src/Orderbook.h"
#include "src/Parser.h"

// -------------------------------------------------------------------------------
// Todo list
// -------------------------------------------------------------------------------
// TODO implement a way of issuing trades 
//      This should allow the trades to go off the best price on each orderbook
// TODO test this with real market data
// TODO Implement a way of switching between FIFO and Pro-Rata trades
// TODO Add a UserID system, which will tie a user to a trade
// TODO Add robust tests
// -------------------------------------------------------------------------------

int main() {
    Orderbook::Orderbook<Orderbook::FIFO> orderbook;
    Orderbook::OrderParser parser("assets/Data.txt");
    std::vector<Orderbook::Order> orders;
    while (!parser.isEOF()) {
        orders.push_back(parser.getNextLine());
    }

    for (auto order : orders) {
        orderbook.addOrder(order);
    }

    auto trade = orderbook.tryMatch();

    return 0;
}