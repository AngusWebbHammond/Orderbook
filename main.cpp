#include <map>
#include <vector>
#include <optional>
#include <algorithm>
#include <exception>
#include <format>

using Price = long double;
using Quantity = uint32_t;
using OrderID = uint64_t;

enum class OrderType {
    BUY,
    SELL
};

class Order {
public:
    Order(Price price, Quantity quantity, OrderType orderType) : m_price{ price }, m_quantity{ quantity }, m_orderType{ orderType } {
        static OrderID id{};
        m_orderID = id++;
    };
    Order(const Order&) = default;
    Order(Order&&) = default;
    Order& operator=(const Order&) = default;
    Order& operator=(Order&&) = default;
    ~Order() = default;

    bool operator==(Order other) { return other.m_orderID == m_orderID; }

    [[nodiscard]] Price getPrice() { return m_price; }
    [[nodiscard]] Quantity getQuantity() { return m_quantity; }
    [[nodiscard]] OrderType getOrderType() { return m_orderType; }
    [[nodiscard]] OrderID getOrderID() { return m_orderID; }

    void fufillOrder(Order& order) {
        if (order.m_quantity > m_quantity) {
            throw std::runtime_error(std::format("Please use a quantity for the order that is less than {}.", m_quantity));
        }
        m_quantity -= order.m_quantity;
    }

private:
    OrderID m_orderID{};
    Price m_price{};
    Quantity m_quantity{};
    OrderType m_orderType{};
};

class Orderbook {
public:
    Orderbook() = default;
    Orderbook(const Orderbook&) = delete;
    Orderbook(Orderbook&&) = delete;
    void operator=(const Orderbook&) = delete;
    void operator=(Orderbook&&) = delete;
    ~Orderbook() = default;

    OrderID addOrder(Order& order) {
        if (order.getOrderType() == OrderType::BUY) {
            if (m_buyBook.contains(order.getPrice())) {
                m_buyBook.at(order.getPrice()) += order.getQuantity();
            }
            else {
                m_buyBook[order.getPrice()] = order.getQuantity();
            }
            m_allBuyOrders.push_back(std::move(order));
            return order.getOrderID();
        }
        else {
            if (m_sellBook.contains(order.getPrice())) {
                m_sellBook.at(order.getPrice()) += order.getQuantity();
            }
            else {
                m_sellBook[order.getPrice()] = order.getQuantity();
            }
            m_allSellOrders.push_back(std::move(order));
            return order.getOrderID();
        }
    }

    std::optional<Order> getOrder(OrderID orderID) {
        auto sellOrder = std::find_if(std::begin(m_allSellOrders), std::end(m_allSellOrders), [orderID](Order& order) { return order.getOrderID() == orderID;});
        auto buyOrder = std::find_if(std::begin(m_allBuyOrders), std::end(m_allBuyOrders), [orderID](Order& order) { return order.getOrderID() == orderID;});
        if (sellOrder != std::end(m_allSellOrders)) {
            return m_allSellOrders.at(std::distance(std::begin(m_allSellOrders), sellOrder));
        }
        else if (buyOrder != std::end(m_allBuyOrders)) {
            return m_allBuyOrders.at(std::distance(std::begin(m_allBuyOrders), buyOrder));
        }
        else return std::nullopt;
    }

    bool cancelOrder(OrderID orderID) {
        if (auto order = getOrder(orderID); order.has_value()) {
            if (order.value().getOrderType() == OrderType::BUY) {
                auto buyOrder = std::find(std::begin(m_allBuyOrders), std::end(m_allBuyOrders), order.value());
                m_allBuyOrders.erase(buyOrder);
                m_buyBook.at(order.value().getPrice()) -= order.value().getQuantity();
            }
            else {
                auto sellOrder = std::find(std::begin(m_allSellOrders), std::end(m_allSellOrders), order.value());
                m_allBuyOrders.erase(sellOrder);
                m_sellBook.at(order.value().getPrice()) -= order.value().getQuantity();
            }
            return true;
        }
        return false;
    }

private:
    std::map<Price, Quantity, std::greater<Price>> m_buyBook{};
    std::map<Price, Quantity, std::less<Price>> m_sellBook{};
    std::vector<Order> m_allBuyOrders{};
    std::vector<Order> m_allSellOrders{};
};

int main() {
    Orderbook orderbook;

    Order orderBuy(10, 10, OrderType::BUY);
    Order orderSell(20, 31, OrderType::SELL);
    orderbook.addOrder(orderBuy);
    orderbook.addOrder(orderSell);

    orderbook.cancelOrder(0);

    return 0;
}