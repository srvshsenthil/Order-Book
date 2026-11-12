#pragma once

#include "order_types.hpp"

void match_order(Order& order, BuyOrderBook& buy_orders, SellOrderBook& sell_orders, std::vector<Trade>& trades, OrderLookup& order_lookup);
void add_order(const Order& order, BuyOrderBook& buy_orders, SellOrderBook& sell_orders, OrderLookup& order_lookup);
void execute_trade(Order& incoming, Order& resting, std::vector<Trade>& trades, int transf);
bool cancel_order(BuyOrderBook& buy_orders, SellOrderBook& sell_orders, OrderLookup& order_lookup);
bool remove_order(long long id, BuyOrderBook& buy_orders, SellOrderBook& sell_orders, OrderLookup& order_lookup);
void modify_order(BuyOrderBook& buy_orders, SellOrderBook& sell_orders, std::vector<Trade>& trades, OrderLookup& order_lookup);
bool order_exists(long long id, const std::unordered_set<long long>& used_order_ids);
bool can_fully_fill(const Order& order, const BuyOrderBook& buy_orders, const SellOrderBook& sell_orders);
bool can_match(const Order& order, double resting_price);
