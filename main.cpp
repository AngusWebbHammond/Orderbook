#include "src/Orderbook.h"
#include "src/Parser.h"

// -------------------------------------------------------------------------------
// Todo list
// -------------------------------------------------------------------------------
// TODO test this with real market data
// TODO Implement a way of switching between FIFO and Pro-Rata trades
// TODO Add a UserID system, which will tie a user to a trade
// TODO Add robust tests
// TODO Add more Trade Types : Pro-Rata, Top, ...
// TODO Add a way of doing multiple trade types, for example FIFO or Top then Pro-Rata
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