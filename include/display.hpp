#include "order_book.hpp"

void display_orders(const BuyOrderBook& buy_orders, const SellOrderBook& sell_orders);
void display_trades(const std::vector<Trade>& trades);
void display_stats(const std::vector<Trade>& trades);
void display_depth(const BuyOrderBook& buy_orders, const SellOrderBook& sell_orders);
