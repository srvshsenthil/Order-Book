#include "display.hpp"

void display_orders(const BuyOrderBook& buy_orders, const SellOrderBook& sell_orders){
    if(buy_orders.empty() && sell_orders.empty()){
        std::cout << "\nNo orders to display.\n";
        return;
    }

    long long total_quantity;

    if(!buy_orders.empty()){
        std::cout << "\nBuy Order Details\n";
        for(const auto& [price, orders] : buy_orders){
            total_quantity = 0;
            std::cout << "Price: " << price << '\n';
            std::cout << std::left << std::setw(15) << "Order ID";
            std::cout << std::left << std::setw(10) << "Side";
            std::cout << std::left << std::setw(12) << "Quantity";
            std::cout << std::left << std::setw(10) << "Order Type";
            std::cout << std::left << std::setw(10) << "Time In Force" << '\n';
            for(const Order& order : orders){
                std::cout << std::left << std::setw(15) << order.id;
                std::cout << std::left << std::setw(10) << "BUY";
                std::cout << std::left << std::setw(12) << order.quantity;
                std::cout << std::left << std::setw(10) << get_order_type(order.type);
                std::cout << std::left << std::setw(10) << get_time_in_force(order.time) << '\n';
                total_quantity += order.quantity;
            }
            std::cout << "Total quantity: " << total_quantity << "\n\n";
        }
    }
    if(!sell_orders.empty()){
        std::cout << "\nSell Order Details\n";

        for(const auto& [price, orders] : sell_orders){
            total_quantity = 0;
            std::cout << "Price: " << price << '\n';
            std::cout << std::left << std::setw(15) << "Order ID";
            std::cout << std::left << std::setw(10) << "Side";
            std::cout << std::left << std::setw(12) << "Quantity";
            std::cout << std::left << std::setw(10) << "Order Type";
            std::cout << std::left << std::setw(10) << "Time In Force" << '\n';
            for (const Order& order : orders){
                std::cout << std::left << std::setw(15) << order.id;
                std::cout << std::left << std::setw(10) << "SELL";
                std::cout << std::left << std::setw(12) << order.quantity;
                std::cout << std::left << std::setw(12) << get_order_type(order.type);
                std::cout << std::left << std::setw(12) << get_time_in_force(order.time) << '\n';
                total_quantity += order.quantity;
            }
            std::cout << "Total quantity: " << total_quantity << "\n\n";
        }
    }
}

void display_trades(const std::vector<Trade>& trades){
    if(!trades.empty()){
        std::cout << "\nExecuted Trades\n";
        std::cout << std::left << std::setw(15) << "Buy Order ID";
        std::cout << std::left << std::setw(15) << "Sell Order ID";
        std::cout << std::left << std::setw(10) << "Price";
        std::cout << std::left << std::setw(12) << "Quantity" << '\n';

        for(const auto& trade : trades){
            std::cout << std::left << std::setw(15) << trade.buy_order_id;
            std::cout << std::left << std::setw(15) << trade.sell_order_id;
            std::cout << std::left << std::setw(10) << trade.price;
            std::cout << std::left << std::setw(12) << trade.quantity << '\n';
        }
    }else{
        std::cout << "\nNo trades executed.\n";
    }
}

void display_stats(const std::vector<Trade>& trades){
    if(trades.empty()){
        std::cout << "No trades executed yet.\n";
        return;
    }

    double last_price = trades.back().price;
    double highest_price = trades[0].price;
    double lowest_price = trades[0].price;
    long long total_quantity = 0;
    double total_value = 0;
    double vwap = 0;

    for(const Trade& t : trades){
        highest_price = std::max(highest_price, t.price);
        lowest_price = std::min(lowest_price, t.price);
        total_quantity += t.quantity;
        total_value += t.price * t.quantity;
    }
    vwap = total_value / total_quantity;

    std::cout << "\nMarket Statistics\n";
    std::cout << std::left << std::setw(30) << "Last Traded Price" << ": " << last_price << '\n';
    std::cout << std::left << std::setw(30) << "Highest Traded Price" << ": " << highest_price << '\n';
    std::cout << std::left << std::setw(30) << "Lowest Traded Price" << ": " << lowest_price << '\n';
    std::cout << std::left << std::setw(30) << "Total Traded Quantity" << ": " << total_quantity << '\n';
    std::cout << std::left << std::setw(30) << "Total Number of Trades" << ": " << trades.size() << '\n';
    std::cout << std::left << std::setw(30) << "VWAP" << ": " << std::fixed << std::setprecision(2) << vwap << '\n';
}

void display_depth(const BuyOrderBook& buy_orders, const SellOrderBook& sell_orders){
    std::cout << "\nORDER BOOK DEPTH\n";
    std::cout << std::left << std::setw(30) << "BUY";
    std::cout << std::left << std::setw(30) << "SELL" << '\n';
    std::cout << std::left << std::setw(15) << "Price";
    std::cout << std::left << std::setw(15) << "Quantity";
    std::cout << std::left << std::setw(15) << "Price";
    std::cout << std::left << std::setw(15) << "Quantity" << '\n';

    auto buy_it = buy_orders.begin();
    auto sell_it = sell_orders.begin();

    while(buy_it != buy_orders.end() || sell_it != sell_orders.end()){
        if(buy_it != buy_orders.end()){
            long long total_quantity = 0;
            for(const Order& o : buy_it->second){
                total_quantity += o.quantity;
            }
            std::cout << std::left << std::setw(15) << buy_it->first;
            std::cout << std::left << std::setw(15) << total_quantity;
            ++buy_it;
        }
        else{
            std::cout << std::left << std::setw(30) << "";
        }

        if(sell_it != sell_orders.end()){
            long long total_quantity = 0;
            for(const Order& o : sell_it->second) total_quantity += o.quantity;
            std::cout << std::left << std::setw(15) << sell_it->first;
            std::cout << std::left << std::setw(15) << total_quantity;
            ++sell_it;
        }
        std::cout << '\n';
    }
}

std::string get_order_type(OrderType type){
    if(type == OrderType::LIMIT) return "LIMIT";
    return "MARKET";
}

std::string get_time_in_force(TimeInForce time){
    if(time == TimeInForce::GTC) return "GTC";
    if(time == TimeInForce::IOC) return "IOC";
    if(time == TimeInForce::FOK) return "FOK";
    return "POST_ONLY";
}
