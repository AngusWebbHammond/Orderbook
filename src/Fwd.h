#pragma once
#include "Concepts.h"
#include "Types.h"

namespace Orderbook {
    template<SupportsMatching MatchingType>
    class Orderbook;

    template<SupportsMatching MatchingType>
    class MatchingEngine;

    class Order;
    class FIFO;
    class OrderbookValue;
    class OrderbookKey;

    template<OrderType BookType>
    class OrderbookMap;

    class Trade;
    class OrderParser;
}