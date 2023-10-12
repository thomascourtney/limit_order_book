#include <iostream>
#include <map>
#include <vector>

// Define a structure for an order
struct Order {
    int order_id;
    int price;
    int quantity;
    char side; // 'B' for buy, 'S' for sell

    Order(int id, int p, int q, char s) : order_id(id), price(p), quantity(q), side(s) {}
};

// Define the limit order book
class LimitOrderBook {
public:
    void addOrder(Order order) {
        if (order.side == 'B') {
            // Buy order
            buy_orders[order.price].push_back(order);
        } else if (order.side == 'S') {
            // Sell order
            sell_orders[order.price].push_back(order);
        }
    }

    void matchOrders() {
        for (auto& buy_order : buy_orders) {
            int buy_price = buy_order.first;
            auto& buy_orders_at_price = buy_order.second;

            if (sell_orders.empty()) {
                break; // No matching sell orders
            }

            auto it = sell_orders.begin();
            int sell_price = it->first;
            auto& sell_orders_at_price = it->second;

            if (buy_price >= sell_price) {
                // Match orders
                int matched_quantity = std::min(buy_orders_at_price.front().quantity, sell_orders_at_price.front().quantity);
                std::cout << "Matched: Buy " << buy_price << " x " << matched_quantity << " @ Sell " << sell_price << std::endl;

                // Update order quantities and remove if filled
                buy_orders_at_price.front().quantity -= matched_quantity;
                sell_orders_at_price.front().quantity -= matched_quantity;

                if (buy_orders_at_price.front().quantity == 0) {
                    buy_orders_at_price.erase(buy_orders_at_price.begin());
                }
                if (sell_orders_at_price.front().quantity == 0) {
                    sell_orders_at_price.erase(sell_orders_at_price.begin());
                }

                // Remove price levels if no more orders at that price
                if (buy_orders_at_price.empty()) {
                    buy_orders.erase(buy_price);
                }
                if (sell_orders_at_price.empty()) {
                    sell_orders.erase(sell_price);
                }
            }
        }
    }

private:
    std::map<int, std::vector<Order> > buy_orders;
    std::map<int, std::vector<Order> > sell_orders;
};

int main() {
    LimitOrderBook orderBook;

    // Add buy orders
    orderBook.addOrder(Order(1, 100, 10, 'B'));
    orderBook.addOrder(Order(2, 99, 15, 'B'));
    orderBook.addOrder(Order(3, 101, 5, 'B'));
    orderBook.addOrder(Order(4, 98, 20, 'B'));

    // Add sell orders
    orderBook.addOrder(Order(5, 102, 10, 'S'));
    orderBook.addOrder(Order(6, 103, 15, 'S'));
    orderBook.addOrder(Order(7, 101, 5, 'S'));
    orderBook.addOrder(Order(8, 104, 20, 'S'));

    std::cout << "Order Book Before Matching:" << std::endl;
    orderBook.matchOrders();
    std::cout << "Order Book After Matching:" << std::endl;
    return 0;
}
