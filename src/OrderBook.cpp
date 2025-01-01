#include "../includes/OrderBook.hpp"
#include <iomanip>
#include <cmath>
#include <random>

map<double, int> OrderBook::getBids() {
    return _bids;
}

map<double, int, greater<double>> OrderBook::getAsks() {
    return _asks;
}

OrderBook::OrderBook(int orders) {
    std::random_device rd{};
    std::mt19937 gen{rd()};

    std::uniform_int_distribution<> mean_dist(100, 999999);
    double mean = mean_dist(gen);

    std::uniform_real_distribution<> stddev_factor(0.1, 0.3);
    double stddev = mean * stddev_factor(gen);

    std::normal_distribution<> d{mean, stddev};

    auto random_positive = [&d, &gen]() {
        double value;
        do {
            value = d(gen);
        } while (value <= 0);
        return std::round(value / 0.25) * 0.25;
    };

    // Coin flip to determine if the mean (if exists) will be on the buy or sell side
    std::bernoulli_distribution coin_flip(0.5);
    bool is_mean_buy = coin_flip(gen);

    // Random number of orders per price
    std::uniform_int_distribution<> quantity_dist(1, 5000);

    // cout << mean << endl;

    for (int n = 0; n < orders; ++n) {
        double price = random_positive();
        price = roundToTwoDecimalPlaces(price);

        int quantity = quantity_dist(gen);

        if (price < mean || (price == mean && is_mean_buy)) {
            _bids[price] += quantity; 
        } else {
            _asks[price] += quantity;
        }
    }
}

double OrderBook::roundToTwoDecimalPlaces(double value) {
    return std::round(value * 100.0) / 100.0;
}


void OrderBook::addMarketBuyOrder(int orders) {
    if (_asks.empty()) {
        // cout << "No sell orders..." << endl;
        // if (prev_sell) {
        //     cout << "Setting limit at previous sell" << endl;
        //     addLimitBuyOrder(prev_sell, orders);
        // } else {
            cout << "No sell orders... The market is your oyster, set a limit price!" << endl;
            cout << "Enter a price: " << endl;
            double price;
            cin >> price;

            while (cin.fail()) {
                cout << "[Error] Invalid input: Please try again\n" << endl;
                cout << "Enter a price: " << endl;
                cin >> price;
            }

            addLimitBuyOrder(price, orders);
        // }
    } else {
        double price = -1;
        matchOrders(_asks, orders, true, price, true); 
    }
}

void OrderBook::addMarketSellOrder(int orders) {
    if (_bids.empty()) {
        // cout << "No buy orders..." << endl;
        // if (prev_buy) {
        //     cout << "Setting limit at previous buy" << endl;
        //     addLimitSellOrder(prev_buy, orders);
        // } else {
            cout << "No buy orders... The market is your oyster, set a limit price!" << endl;
            cout << "Enter a price: " << endl;
            double price;
            cin >> price;

            while (cin.fail()) {
                cout << "[Error] Invalid input: Please try again\n" << endl;
                cout << "Enter a price: " << endl;
                cin >> price;
            }

            addLimitSellOrder(price, orders);
        // }
    } else {
        double price = -1;
        matchOrders(_bids, orders, false, price, true);
    }
}

void OrderBook::addLimitBuyOrder(double price, int orders) {
    if (!_asks.empty() && price >= _asks.rbegin()->first) {
        matchOrders(_asks, orders, true, price, false); 
    } 
    
    if (orders > 0) {
        _bids[price] += orders;
    }
}

void OrderBook::addLimitSellOrder(double price, int orders) {
    if (!_bids.empty() && price <= _bids.rbegin()->first) {
        matchOrders(_bids, orders, false, price, false);  
    } 
    
    if (orders > 0) {
        _asks[price] += orders; 
    }
}

void OrderBook::printOrderBook(int orders) {
    int max_display = orders; // Limit to the top N orders

    cout << "=========== Order Book ===========" << endl;

    // Prepare sorted sell orders 
    vector<pair<double, int>> sorted_asks(_asks.begin(), _asks.end());
    sort(sorted_asks.begin(), sorted_asks.end(), [](auto& a, auto& b) {
        return a.first < b.first; // Descending order of price
    });

    // Prepare sorted buy orders 
    vector<pair<double, int>> sorted_bids(_bids.begin(), _bids.end());
    sort(sorted_bids.begin(), sorted_bids.end(), [](auto& a, auto& b) {
        return a.first > b.first; // Ascending order of price
    });
    
    auto advance_buy_it = sorted_bids.begin();
    auto advance_sell_it = sorted_asks.begin();
    int i = 0;
    while (i < orders) {
        ++advance_buy_it;
        ++advance_sell_it;
        ++i;
    }

    // Reverse for the most expensive N to be in descending order
    vector<pair<double, int>> reverse_sorted_bids(sorted_bids.begin(), advance_buy_it);
    sort(reverse_sorted_bids.begin(), reverse_sorted_bids.end(), [](auto& a, auto& b) {
        return a.first > b.first; // Descending order of price
    });

    // Reverse for the most expensive N to be in descending order
    vector<pair<double, int>> reverse_sorted_asks(sorted_asks.begin(), advance_sell_it);
    sort(reverse_sorted_asks.begin(), reverse_sorted_asks.end(), [](auto& a, auto& b) {
        return a.first < b.first; 
    });

    
    int min_quantity = INT_MAX;
    int max_quantity = 0;
    for (const auto order : sorted_bids) {
        min_quantity = min(min_quantity, order.second);
        max_quantity = max(max_quantity, order.second);
    }
    for (const auto& order : sorted_asks) {
        min_quantity = min(min_quantity, order.second);
        max_quantity = max(max_quantity, order.second);
    }

    // cout << "\033[31mSELL ORDERS\033[0m" << endl;
    // cout << "\n" << endl;
    cout << "Price       Quantity   Volume" << endl;
    cout << "----------------------------------" << endl;
    for (int i = reverse_sorted_asks.size() - 1; (reverse_sorted_asks.size() - 1 - i) < max_display && i >= 0; --i) {
        auto sell_order = reverse_sorted_asks[i];
        int scaled_stars = 10;
        if (max_quantity > min_quantity) {
            scaled_stars = max(1, static_cast<int>(((sell_order.second - min_quantity) / 
                        static_cast<double>(max_quantity - min_quantity)) * 10));
        }
        cout << std::fixed << std::setprecision(2);
        cout << "\033[31m$" << setw(10) << sell_order.first << setw(9) << sell_order.second << "   ";
        cout << string(scaled_stars, '*') << "\033[0m" << endl;
    }

    // cout << "\033[32mBUY ORDERS\033[0m" << endl;
    // cout << "Price       Quantity   Volume" << endl;
    cout << "----------------------------------" << endl;
    for (int i = 0; i < max_display && i < reverse_sorted_bids.size(); ++i) {
        auto buy_order = reverse_sorted_bids[i];
        int scaled_stars = 10;
        if (max_quantity > min_quantity) {
            scaled_stars = max(1, static_cast<int>(((buy_order.second - min_quantity) / 
                        static_cast<double>(max_quantity - min_quantity)) * 10));
        }
        cout << std::fixed << std::setprecision(2);
        cout << "\033[32m$" << setw(10) << buy_order.first << setw(9) << buy_order.second << "   ";
        cout << string(scaled_stars, '*') << "\033[0m" << endl;
    }

    cout << "==================================" << endl;
}


template <typename T, typename Compare>
void OrderBook::matchOrders(std::map<T, int, Compare>& orders, int& incomingOrders, bool isBuyOrder, double price, bool isMarketOrder) {
    while (!orders.empty() && incomingOrders > 0) {
        auto iter = std::prev(orders.end());
        // auto iter = isBuyOrder ? std::prev(orders.end()) : orders.begin();
        if (isMarketOrder) {
            // For market orders, we essentially set price to the max or min 
            // thresholds depending on the order
            if (isBuyOrder) {
                price = orders.begin()->first;
            } else {
                price = orders.begin()->first;
            }
        } 
        auto& first_order = *iter;

        if ((isBuyOrder && price < first_order.first) || 
            (!isBuyOrder && price > first_order.first)) {
            cout << " " << endl;
            return;
        }

        int count = first_order.second;

        cout << "\nMatching " << incomingOrders << " orders at price $" 
                    << fixed << setprecision(2) << first_order.first << endl;

        if (count <= incomingOrders) {
            cout << "Fully matched " << count << " orders" << endl;
            incomingOrders -= count;
            orders.erase(iter);
        } else {
            cout << "Partially matched " << incomingOrders << " orders" << endl;
            first_order.second -= incomingOrders;
            incomingOrders = 0;
        }

        cout << "Remaining orders: " << incomingOrders << endl;
    }
}
