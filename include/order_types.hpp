#pragma once

#include <map>
#include <list>
#include <functional>
#include <unordered_map>
#include <vector>
#include <string>
#include <unordered_set>

enum class OrderType{
    LIMIT,
    MARKET
};

enum class TimeInForce{
    GTC,
    IOC,
    FOK,
    POST_ONLY
};

enum class Side{
    BUY,
    SELL
};

struct Order{
    long long id;
    Side side;
    double price;
    int quantity;
    OrderType type;
    TimeInForce time;
};

struct Trade{
    long long buy_order_id;
    long long sell_order_id;
    double price;
    int quantity;
};

using BuyOrderBook = std::map<double, std::list<Order>, std::greater<double>>;
using SellOrderBook = std::map<double, std::list<Order>>;

struct OrderLocation{
    Side side;
    std::list<Order>::iterator order_it;
    BuyOrderBook::iterator buy_level;
    SellOrderBook::iterator sell_level;
};

using OrderLookup = std::unordered_map<long long, OrderLocation>;
