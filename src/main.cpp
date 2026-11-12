#include <bits/stdc++.h>

#include "order_book.hpp"
#include "input.hpp"
#include "display.hpp"

int main(){
    BuyOrderBook buy_orders;
    SellOrderBook sell_orders;
    std::vector<Trade> trades;
    OrderLookup order_lookup;
    std::unordered_set<long long> used_order_ids;
    char state;

    while(true){
        std::cout << "\nP - Place Order\n";
        std::cout << "C - Cancel Order\n";
        std::cout << "M - Modify Order\n";
        std::cout << "O - Order Book\n";
        std::cout << "T - Trades\n";
        std::cout << "S - Market Statistics\n";
        std::cout << "B - Order Book Depth\n";
        std::cout << "E - Exit\n\n";
        std::cout << "Enter choice: ";

        while(true){
            if(std::cin >> state &&
                (state == 'P' || state == 'M' || state == 'E' || state == 'O' || state == 'T'
                || state == 'S' || state == 'B' || state == 'C')) break;
            std::cout << "Invalid." << '\n';
            std::cin.clear();
            std::cin.ignore(1000, '\n');
        }

        if(state == 'P'){
            Order o = get_order(used_order_ids);
            used_order_ids.insert(o.id);
            match_order(o, buy_orders, sell_orders, trades, order_lookup);
        }
        else if(state == 'C'){
            if(cancel_order(buy_orders, sell_orders, order_lookup)) std::cout << "Order cancelled successfully.\n";
            else std::cout << "Order not found or already executed.\n";
        }
        else if(state == 'M') modify_order(buy_orders, sell_orders, trades, order_lookup);
        else if(state == 'O') display_orders(buy_orders, sell_orders);
        else if(state == 'T') display_trades(trades);
        else if(state == 'S') display_stats(trades);
        else if(state == 'B') display_depth(buy_orders, sell_orders);
        else break;
    }
    return 0;
}
