#include "OrderBook.hpp"
#include "OptionsPricing.hpp"
#include <iostream>
#include <exception>

using namespace std;

int main() {

    cout << "░█████╗░░░░░░░░░░░░░░░  ░█████╗░██████╗░██████╗░███████╗██████╗░██████╗░░█████╗░░█████╗░██╗░░██╗" << endl;
    cout << "██╔══██╗░░██╗░░░░██╗░░  ██╔══██╗██╔══██╗██╔══██╗██╔════╝██╔══██╗██╔══██╗██╔══██╗██╔══██╗██║░██╔╝" << endl;
    cout << "██║░░╚═╝██████╗██████╗  ██║░░██║██████╔╝██║░░██║█████╗░░██████╔╝██████╦╝██║░░██║██║░░██║█████═╝░" << endl;
    cout << "██║░░██╗╚═██╔═╝╚═██╔═╝  ██║░░██║██╔══██╗██║░░██║██╔══╝░░██╔══██╗██╔══██╗██║░░██║██║░░██║██╔═██╗░" << endl;
    cout << "╚█████╔╝░░╚═╝░░░░╚═╝░░  ╚█████╔╝██║░░██║██████╔╝███████╗██║░░██║██████╦╝╚█████╔╝╚█████╔╝██║░╚██╗" << endl;
    cout << "░╚════╝░░░░░░░░░░░░░░░  ░╚════╝░╚═╝░░╚═╝╚═════╝░╚══════╝╚═╝░░╚═╝╚═════╝░░╚════╝░░╚════╝░╚═╝░░╚═╝" << endl;

    OrderBook orderbook(500);

    for (auto n : orderbook.getBids()) {
        cout << n.first << endl;
    }

    cout << " " << endl;

    for (auto n : orderbook.getAsks()) {
        cout << n.first << endl;
    }

    while (true) {
        orderbook.printOrderBook(5);
        char order_type;
        char buy_sell_type;
        char options_type;
        int num_orders;
        double limit_price;

        cout << "\nSelect Order Type" << endl;

        cout << "1: Market Order" << endl;
        cout << "2: Limit Order" << endl;
        cout << "3: Options Order" << endl;
        cout << "\nEnter your choice: ";

        cin >> order_type;

        while (order_type != '1' && order_type != '2' && order_type != '3') {
            cerr << "[Error] Invalid Order Type, please enter 1, 2, or 3" << endl;
            cin.clear();
            cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
            cout << "Enter your choice: ";
            cin >> order_type;
            cout << "Enter your choice: ";
        }

        if (order_type == '1') {
            cout << "\nSelect Buy or Sell" << endl;
            cout << "1: Buy" << endl;
            cout << "2: Sell" << endl;
            cout << "\nEnter your choice: ";

            cin >> buy_sell_type;

            while (buy_sell_type != '1' && buy_sell_type != '2') {
                cerr << "[Error] Invalid Buy/Sell Type, please enter 1 or 2" << endl;
                cin.clear();
                cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
                cout << "Enter your choice: ";
                cin >> buy_sell_type;
            }

            cout << "\nEnter number of orders: " << endl;
            cin >> num_orders;
            while (cin.fail()) {
                cout << "[Error] Invalid input: Please try again\n" << endl;
                cin.clear();
                cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
                cout << "\nEnter number of orders: " << endl;
                cin >> num_orders;
            }

            if (buy_sell_type == '1') {
                orderbook.addMarketBuyOrder(num_orders);
            } else {
                orderbook.addMarketSellOrder(num_orders);
            }

        } else if (order_type == '2') {
            cout << "\nSelect Buy or Sell" << endl;
            cout << "1: Buy" << endl;
            cout << "2: Sell" << endl;
            cout << "\nEnter your choice: ";

            cin >> buy_sell_type;

            while (buy_sell_type != '1' && buy_sell_type != '2') {
                cerr << "[Error] Invalid Buy/Sell Type, please enter 1 or 2" << endl;
                cin.clear();
                cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
                cout << "Enter your choice: ";
                cin >> buy_sell_type;
            }

            cout << "\nEnter limit price: " << endl;
            cin >> limit_price;
            while (cin.fail()) {
                cout << "[Error] Invalid input: Please try again\n" << endl;
                cin.clear();
                cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
                cout << "\nEnter limit price: " << endl;
                cin >> limit_price;
            }

            cout << "\nEnter number of orders: " << endl;
            cin >> num_orders;
            while (cin.fail()) {
                cout << "[Error] Invalid input: Please try again\n" << endl;
                cin.clear();
                cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
                cout << "\nEnter number of orders: " << endl;
                cin >> num_orders;
            }

            if (buy_sell_type == '1') {
                orderbook.addLimitBuyOrder(limit_price, num_orders);
            } else {
                orderbook.addLimitSellOrder(limit_price, num_orders);
            }

        } else {
            cout << "\nSelect Options Pricing Model" << endl;
            cout << "1: Black-Scholes" << endl;
            cout << "2: Binomial" << endl;
            cout << "\nEnter your choice: ";

            cin >> options_type;

            while (options_type != '1' && options_type != '2') {
                cerr << "[Error] Invalid Options Model Type, please enter 1 or 2" << endl;
                cin.clear();
                cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
                cout << "Enter your choice: ";
                cin >> options_type;
            }

            OptionsPricing options;
            if (options_type == 1) {
                // TODO
            }
        }
        // cout << "\n==============================" << endl;
        cin.clear();
        cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
    }

    return 0;
}
