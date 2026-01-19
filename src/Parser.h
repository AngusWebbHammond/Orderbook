#pragma once

#include "Order.h"

#include <string>
#include <vector>
#include <fstream>

namespace Orderbook {
    class OrderParser {
    public:
        OrderParser(std::string);
        ~OrderParser();

        auto getFullParsedData() -> std::vector<Order>;
        auto getNextLine() -> Order;

        auto isEOF() -> bool { return m_file.eof(); };

        auto openNewFile(std::string) -> void;

    private:
        auto parseStringToOrder(std::string) -> Order;

        std::ifstream m_file;
    };
}