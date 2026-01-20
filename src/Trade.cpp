#include "Trade.h"

std::ostream& operator<<(std::ostream& os, const Orderbook::Trade& trade)
{
    os << "Sell ID: " << trade.ids.first << ", Buy ID: " << trade.ids.second << ", Price: " << trade.price << ", Quantity: " << trade.quantity;
    return os;
}