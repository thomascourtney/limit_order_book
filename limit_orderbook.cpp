#include <iostream>
#include <map>
#include <vector>
#include <chrono>
using namespace std;

class Order {
public:
    int order_id;
    int price;
    int quantity;
    char side; // 'B' for buy, 'S' for sell

    Order(int id, int p, int q, char s) : order_id(id), price(p), quantity(q), side(s) {}
};

class LimitOrderBook {
private:
    map<int, vector<Order> > buy_orders;
    map<int, vector<Order> > sell_orders;

public:
    void addOrder(Order order);
    void matchOrders();

};

void LimitOrderBook::addOrder(Order order) {
        if (order.side == 'B') {
            // Buy order
            buy_orders[order.price].push_back(order);
        } else if (order.side == 'S') {
            // Sell order
            sell_orders[order.price].push_back(order);
        }
}

void LimitOrderBook::matchOrders() {
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
            int matched_quantity = min(buy_orders_at_price.front().quantity, sell_orders_at_price.front().quantity);
            cout << "Matched: Buy " << buy_price << " x " << matched_quantity << " @ Sell " << sell_price << endl;

            // Update order quantities and remove if filled
            buy_orders_at_price.front().quantity -= matched_quantity;
            sell_orders_at_price.front().quantity -= matched_quantity;

            if (buy_orders_at_price.front().quantity == 0) {
                buy_orders_at_price.erase(buy_orders_at_price.begin());
            }
            if (sell_orders_at_price.front().quantity == 0) {
                sell_orders_at_price.erase(sell_orders_at_price.begin());
            }

            if (buy_orders_at_price.empty()) {
                buy_orders.erase(buy_price);
            }
            if (sell_orders_at_price.empty()) {
                sell_orders.erase(sell_price);
            }
        }
    }
}


int getRandomPrice() {
    // Generate random prices within a certain range
    return 80 + (rand() % (250 - 50 + 1));
}

int getRandomQuantity() {
    // Generate random quantities within a certain range
    return 1 + (rand() % (30 - 1 + 1));
}

char getRandomSide() {
    // Generate a random side ('B' for buy or 'S' for sell)
    return (rand() % 2 == 0) ? 'B' : 'S';
}

int main() {
    auto start_time = chrono::high_resolution_clock::now();

    LimitOrderBook orderBook;

    srand(static_cast<unsigned>(chrono::system_clock::now().time_since_epoch().count()));

    // Add random orders
    for (int i = 1; i <= 1000; ++i) {
        orderBook.addOrder(Order(i, getRandomPrice(), getRandomQuantity(), getRandomSide()));
    }

    cout << "Order Book Before Matching:" << endl;
    orderBook.matchOrders();
    cout << "Order Book After Matching:" << endl;

    auto end_time = chrono::high_resolution_clock::now();

    auto duration = chrono::duration_cast<chrono::microseconds>(end_time - start_time);

    cout << "Time taken by the program: " << duration.count() << " microseconds" << endl;

    return 0;
}
