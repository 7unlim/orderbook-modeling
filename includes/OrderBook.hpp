#pragma once
#ifndef ORDERBOOK_HPP
#define ORDERBOOK_HPP
#include <map>
#include <iostream>

using namespace std;

class OrderBook {
    public:
        OrderBook(int orders);

        void addMarketBuyOrder(int orders);
        void addMarketSellOrder(int orders);
        void addLimitBuyOrder(double price, int orders);
        void addLimitSellOrder(double price, int orders);
        // void processLimboBuys();
        // void processLimboSells();

        template <typename T, typename Compare>
        void matchOrders(std::map<T, int, Compare>& orders, int& incomingOrders, bool isBuyOrder, double price, bool isMarketOrder);
        void printOrderBook(int orders);
        map<double, int> getBids();
        map<double, int, greater<double>> getAsks();
        double roundToTwoDecimalPlaces(double value);
        // double roundToPrecision(double value, int decimals);

    private:
        // Num of orders, price
        map<double, int> _bids;
        map<double, int, greater<double>> _asks;

        // double min_order_price = numeric_limits<double>::max();
        // double max_order_price = 0;
        // int prev_sell;
        // int prev_buy;
};


#endif