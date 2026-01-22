#include "src/Orderbook.h"
#include "src/Parser.h"
#include "src/Trade.h"
#include "src/OrderbookContainer.h"
#include "src/MatchingEngine.h"

#include <chrono>
#include <print>

// -------------------------------------------------------------------------------
// Todo list
// -------------------------------------------------------------------------------
// TODO test this with real market data
// TODO Implement a way of switching between FIFO and Pro-Rata trades
// TODO Add a UserID system, which will tie a user to a trade
// TODO Add robust tests
// TODO Add more Trade Types : Pro-Rata, Top, ...
// TODO Add a way of doing multiple trade types, for example FIFO or Top then Pro-Rata
// TODO Add a GUI/Web-interface to allow adding/removing from the order book in realtime
// TODO Add Threading support for the matching engine.
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

    while (true) {
        auto trade{ orderbook.tryMatch() };
        if (trade.has_value()) {
            std::println("{}", trade.value());
        }
        else {
            break;
        }
    }

    return 0;
}