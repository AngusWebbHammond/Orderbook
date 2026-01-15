#include "HelperFunctions.h"
namespace Orderbook
{
    auto generateOrderID() -> OrderID
    {
        static OrderID id{ 0 };
        return id++;
    }
}