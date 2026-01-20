#include "Order.h"
#include <cassert>

auto Orderbook::Order::performTrade(Quantity other) -> Quantity
{
    assert(m_quantity >= other, "Please only call this function with a quantity less than the currently selected orders quantity.");
    m_quantity -= other;
    return m_quantity;
}
