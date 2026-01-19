#include "Parser.h"

#include <fstream>
#include <format>
#include <iostream>
#include <any>
#include <charconv>
#include <print>

namespace Orderbook
{
    OrderParser::OrderParser(std::string file) {
        openNewFile(file);
    }

    OrderParser::~OrderParser()
    {
        m_file.close();
    }

    auto OrderParser::getFullParsedData() -> std::vector<Order>
    {
        std::vector<Order> orders;
        while (!m_file.eof()) {
            orders.push_back(getNextLine());
        }

        return orders;
    }

    auto OrderParser::getNextLine() -> Order
    {

        if (m_file.eof()) {
            throw std::runtime_error(std::format("You are at the end of the file, open a new one to get the next line."));
        }

        std::string line;
        std::getline(m_file, line);

        if (line.empty()) {
            throw std::runtime_error(std::format("Empty line."));
        }

        auto order = parseStringToOrder(line);

        return order;
    }

    auto OrderParser::openNewFile(std::string file) -> void
    {
        if (m_file.is_open()) m_file.close();
        m_file.open(file);
        if (m_file.is_open()) return;
        throw std::runtime_error(std::format("File {}, failed to open.", file));
    }

    auto OrderParser::parseStringToOrder(std::string stringOrder) -> Order
    {
        OrderType orderType{ stringOrder.front() == 'S' ? OrderType::SELL : OrderType::BUY };

        stringOrder = stringOrder.substr(3);
        auto found = std::find(std::begin(stringOrder), std::end(stringOrder), ',');
        auto dist = std::distance(std::begin(stringOrder), found);
        auto sub = stringOrder.substr(0, dist);

        Quantity quantity{ std::stoul(sub) };

        stringOrder = stringOrder.substr(dist);
        while (stringOrder.front() == ' ' || stringOrder.front() == ',') {
            stringOrder = stringOrder.substr(1);
        }
        found = std::find(std::begin(stringOrder), std::end(stringOrder), ',');
        dist = std::distance(std::begin(stringOrder), found);
        sub = stringOrder.substr(0, dist);

        Price price{ std::stold(sub) };

        stringOrder = stringOrder.substr(dist);
        while (stringOrder.front() == ' ' || stringOrder.front() == ',') {
            stringOrder = stringOrder.substr(1);
        }

        OrderID orderID{ std::stoull(stringOrder) };

        return Order{ price, quantity, orderType, orderID };
    }
}