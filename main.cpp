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
#define ORDERBOOK_MAP2
#ifdef ORDERBOOK_MAP2

struct OrderbookValue {
    std::shared_ptr<Order> order;
    OrderbookValue* nextOrder;
};

struct OrderbookKey {
    Price price;
    Quantity quantity;
    OrderbookValue* orders;

    OrderbookValue* find(OrderID id) {
        OrderbookValue* ptr = orders;
        while (ptr) {
            if (ptr->order->getOrderID() == id) {
                return ptr;
            }
            if (ptr->nextOrder) {
                ptr = ptr->nextOrder;
            }
            else {
                return nullptr;
            }
        }
        return nullptr;
    };

    std::optional<OrderbookValue*> findPrevious(OrderID id) {
        OrderbookValue* ptr = orders;
        while (ptr) {
            if (ptr->nextOrder) {
                if (ptr->nextOrder->order->getOrderID() == id) {
                    return ptr;
                }

                ptr = ptr->nextOrder;
            }
            else {
                if (orders->order->getOrderID() == id) {
                    return nullptr;
                }
                return std::nullopt;
            }
        }
        return std::nullopt;
    }

    void pushBack(Order tempOrder) {
        OrderbookValue* ptr = orders;
        quantity += tempOrder.getQuantity();
        while (ptr) {
            if (ptr->nextOrder) {
                ptr = ptr->nextOrder;
            }
            else {
                OrderbookValue obv = { .order = std::make_shared<Order>(tempOrder), .nextOrder = nullptr };
                ptr->nextOrder = new OrderbookValue(obv);
                return;
            }
        }
        OrderbookValue obv = { .order = std::make_shared<Order>(tempOrder), .nextOrder = nullptr };
        orders = new OrderbookValue(obv);
        return;
    }

    void erase(OrderID id) {
        if (auto value = findPrevious(id); value) {
            if (value.value()) {
                auto* valueNext = value.value()->nextOrder;
                value.value()->nextOrder = value.value()->nextOrder->nextOrder;
                quantity -= valueNext->order->getQuantity();
                delete valueNext;
            }
            else {
                quantity -= orders->order->getQuantity();
                OrderbookValue* orderValue = orders;
                OrderbookValue* nextValue = orderValue->nextOrder;
                delete orderValue;
                orders = nextValue;
            }
        }
        auto a = 1;
    }

    void clear() {
        auto* ptr = orders;
        while (ptr) {
            auto* nextPtr = ptr->nextOrder;
            if (!nextPtr) {
                delete ptr;
                ptr = nullptr;
                return;
            }
            else if (nextPtr->nextOrder) {
                delete ptr;
                ptr = nextPtr;
            }
            else {
                delete ptr;
            }
        }
        orders = nullptr;
        quantity = 0;
    }
};

template<typename T>
class OrderbookMap {
public:
    OrderbookMap() = default;
    OrderbookMap(const OrderbookMap&) = delete;
    OrderbookMap(OrderbookMap&&) = delete;
    void operator=(const OrderbookMap&) = delete;
    void operator=(OrderbookMap&&) = delete;
    ~OrderbookMap() {
        for (auto& key : m_orderbook) {
            key.clear();
        }
    };

    std::optional<std::shared_ptr<Order>> ifContainsGet(OrderID id) {
        auto found = std::find_if(std::begin(m_orderbook), std::end(m_orderbook), [id](OrderbookKey item) {
            if (auto found = item.find(id); found) {
                return true;
            }
            return false;
            }
        );
        if (found != std::end(m_orderbook)) {
            auto f = m_orderbook.at(std::distance(std::begin(m_orderbook), found)).find(id);
            std::shared_ptr<Order> res = f->order;
            return res;
        }
        return std::nullopt;
    }

    OrderbookKey* ifContainsGet(Price price) {
        auto found = std::find_if(std::begin(m_orderbook), std::end(m_orderbook), [price](OrderbookKey item) {return price == item.price;});
        if (found != std::end(m_orderbook)) {
            return &m_orderbook.at(std::distance(std::begin(m_orderbook), found));
        }
        return nullptr;
    }

    void insert(Order tempOrder) {
        if (auto* item = ifContainsGet(tempOrder.getPrice()); item) {
            item->pushBack(tempOrder);
            return;
        }
        OrderbookValue obv = { .order{ std::make_shared<Order>(tempOrder) }, .nextOrder{ nullptr } };
        auto* ptr = new OrderbookValue(std::move(obv));
        OrderbookKey item = { .price{tempOrder.getPrice()}, .quantity{tempOrder.getQuantity()}, .orders{ptr} };
        m_orderbook.push_back(item);
    }

    void ifContainsRemove(Price price) {
        if (auto key = ifContainsGet(price); key) {
            key->clear();
            m_orderbook.erase(std::find_if(std::begin(m_orderbook), std::end(m_orderbook), [price](auto item) {return price == item.price;}));
            return;
        }
        return;
    }
private:
    std::vector<OrderbookKey> m_orderbook;
};
#else
template<typename T>
using OrderbookMap = std::map<Price, Quantity, T>;
#endif

class Orderbook {
public:
    Orderbook() = default;
    Orderbook(const Orderbook&) = delete;
    Orderbook(Orderbook&&) = delete;
    void operator=(const Orderbook&) = delete;
    void operator=(Orderbook&&) = delete;
    ~Orderbook() = default;

    OrderID addOrder(Order& order) {
#ifndef ORDERBOOK_MAP2
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
#else
        if (order.getOrderType() == OrderType::BUY) {
            m_buyBook.insert(order);
        }
        else {
            m_sellBook.insert(order);
        }
        return order.getOrderID();
#endif
    }
#ifndef ORDERBOOK_MAP2
    std::optional<Order>
#else
    std::optional<std::shared_ptr<Order>>
#endif
        getOrder(OrderID orderID) {
#ifndef ORDERBOOK_MAP2
        auto sellOrder = std::find_if(std::begin(m_allSellOrders), std::end(m_allSellOrders), [orderID](Order& order) { return order.getOrderID() == orderID;});
        auto buyOrder = std::find_if(std::begin(m_allBuyOrders), std::end(m_allBuyOrders), [orderID](Order& order) { return order.getOrderID() == orderID;});
        if (sellOrder != std::end(m_allSellOrders)) {
            return m_allSellOrders.at(std::distance(std::begin(m_allSellOrders), sellOrder));
        }
        else if (buyOrder != std::end(m_allBuyOrders)) {
            return m_allBuyOrders.at(std::distance(std::begin(m_allBuyOrders), buyOrder));
        }
        else return std::nullopt;
#else
        auto sellOrder = m_sellBook.ifContainsGet(orderID);
        auto buyOrder = m_buyBook.ifContainsGet(orderID);
        if (sellOrder) {
            return sellOrder.value();
        }
        else if (buyOrder) {
            return buyOrder.value();
        }
        else {
            return std::nullopt;
        }
#endif
    }

    bool cancelOrder(OrderID orderID) {
#ifndef ORDERBOOK_MAP2
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
#else
        if (auto order = getOrder(orderID); order) {
            if (order.value()->getOrderType() == OrderType::BUY) {
                OrderbookKey* item = m_buyBook.ifContainsGet(order.value()->getPrice());
                item->erase(orderID);
                return true;
            }
            else {
                auto item = m_sellBook.ifContainsGet(order.value()->getPrice());
                item->erase(orderID);
                return true;
            }
        }
        else {
            return false;
        }
#endif
    }

private:
#ifndef ORDERBOOK_MAP2
    // Could join the below into a std::map<std::pair<Price, Quantity>, std::vector<Order>, T>
    OrderbookMap<std::less<>> m_buyBook{};
    OrderbookMap<std::greater<>> m_sellBook{};
    std::vector<Order> m_allBuyOrders{};
    std::vector<Order> m_allSellOrders{};
#else
    OrderbookMap<std::less<>> m_buyBook{};
    OrderbookMap<std::greater<>> m_sellBook{};
#endif
};

int main() {
    Orderbook orderbook;

    Order orderBuy1(10, 10, OrderType::BUY);
    Order orderBuy2(12, 10, OrderType::BUY);
    Order orderSell1(20, 31, OrderType::SELL);
    Order orderSell2(20, 32, OrderType::SELL);
    orderbook.addOrder(orderBuy1);
    orderbook.addOrder(orderBuy2);
    orderbook.addOrder(orderSell1);
    orderbook.addOrder(orderSell2);

    orderbook.cancelOrder(0);
    orderbook.cancelOrder(2);

    orderbook.addOrder(orderBuy1);
    return 0;
}