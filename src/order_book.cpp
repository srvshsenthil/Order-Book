#include "order_book.hpp"

void match_order(Order& order, BuyOrderBook& buy_orders, SellOrderBook& sell_orders, std::vector<Trade>& trades, OrderLookup& order_lookup){
    if(order.time == TimeInForce::POST_ONLY){
        bool would_match = false;
        if(order.side == Side::BUY && !sell_orders.empty()){
            would_match = can_match(order, sell_orders.begin()->first);
        }else if(order.side == Side::SELL && !buy_orders.empty()){
            would_match = can_match(order, buy_orders.begin()->first);
        }
        if(would_match){
            std::cout << "Post-only order rejected. Order would execute immediately.\n";
            return;
        }
        add_order(order, buy_orders, sell_orders, order_lookup);
        std::cout << "Post-only order added to the order book.\n";
        return;
    }
    if(order.time == TimeInForce::FOK){
        if(!can_fully_fill(order, buy_orders, sell_orders)){
            std::cout << "FOK order cancelled. Insufficient liquidity for full execution.\n";
            return;
        }
    }
    
    int transf;

    if(order.side == Side::BUY){
        while(order.quantity > 0 && !sell_orders.empty()){
            auto it = sell_orders.begin();
            if(order.price >= it->first){
                Order& resting_order = it->second.front();
                transf = std::min(order.quantity, resting_order.quantity);
                execute_trade(order, resting_order, trades, transf);
                if(resting_order.quantity == 0){
                    order_lookup.erase(resting_order.id);
                    it->second.pop_front();
                }
                if(it->second.empty()) sell_orders.erase(it);
            }else break;
        }
        if(order.quantity > 0 && order.time == TimeInForce::GTC){
            add_order(order, buy_orders, sell_orders, order_lookup);
            std::cout << "Remaining quantity added to the order book.\n";
        }else if(order.quantity > 0 && order.time == TimeInForce::IOC){
            std::cout << "Unfilled IOC quantity cancelled: " << order.quantity << '\n';
        }
    }else{
        while(order.quantity > 0 && !buy_orders.empty()){
            auto it = buy_orders.begin();
            if(order.price <= it->first){
                Order& resting_order = it->second.front();
                transf = std::min(order.quantity, resting_order.quantity);
                execute_trade(order, resting_order, trades, transf);
                if(resting_order.quantity == 0){
                    order_lookup.erase(resting_order.id);
                    it->second.pop_front();
                }
                if(it->second.empty()) buy_orders.erase(it);
            }else break;
        }
        if(order.quantity > 0 && order.time == TimeInForce::GTC){
            add_order(order, buy_orders, sell_orders, order_lookup);
            std::cout << "Remaining quantity added to the order book.\n";
        }else if(order.quantity > 0 && order.time == TimeInForce::IOC){
            std::cout << "Unfilled IOC quantity cancelled: " << order.quantity << '\n';
        }
    }
    if(order.quantity == 0){
        std::cout << "Order completely filled.\n";
    }
}

void add_order(const Order& order, BuyOrderBook& buy_orders, SellOrderBook& sell_orders, OrderLookup& order_lookup){
    if(order.side == Side::BUY){
        auto level_it = buy_orders.try_emplace(order.price).first;
        level_it->second.push_back(order);
        auto order_it = std::prev(level_it->second.end());
        order_lookup.emplace(order.id, OrderLocation{
            Side::BUY,
            order_it,
            level_it,
            SellOrderBook::iterator{}
        });
    }else{
        auto level_it = sell_orders.try_emplace(order.price).first;
        level_it->second.push_back(order);
        auto order_it = std::prev(level_it->second.end());
        order_lookup.emplace(order.id, OrderLocation{
            Side::SELL,
            order_it,
            BuyOrderBook::iterator{},
            level_it
        });
    }
}

void execute_trade(Order& incoming, Order& resting, std::vector<Trade>& trades, int transf){
    Trade trade;
    incoming.quantity -= transf;
    resting.quantity -= transf;
    if(incoming.side == Side::BUY){
        trade.buy_order_id = incoming.id;
        trade.sell_order_id = resting.id;
    }else{
        trade.buy_order_id = resting.id;
        trade.sell_order_id = incoming.id;
    }
    trade.price = resting.price;
    trade.quantity = transf;
    trades.push_back(trade);
}

bool cancel_order(long long id, BuyOrderBook& buy_orders, SellOrderBook& sell_orders, OrderLookup& order_lookup){
    if(!(std::cin >> id) || id <= 0){
        std::cout << "Invalid Order ID.\n";
        std::cin.clear();
        std::cin.ignore(1000, '\n');
        return false;
    }

    return remove_order(id, buy_orders, sell_orders, order_lookup);
}

bool remove_order(long long id, BuyOrderBook& buy_orders, SellOrderBook& sell_orders, OrderLookup& order_lookup){
    auto it = order_lookup.find(id);
    if(it == order_lookup.end()) return false;
    if(it->second.side == Side::BUY){
        auto level_it = it->second.buy_level;
        level_it->second.erase(it->second.order_it);
        if(level_it->second.empty()) buy_orders.erase(level_it);
    }else{
        auto level_it = it->second.sell_level;
        level_it->second.erase(it->second.order_it);
        if(level_it->second.empty()) sell_orders.erase(level_it);
    }
    order_lookup.erase(it);
    return true;
}

void modify_order(long long id, BuyOrderBook& buy_orders, SellOrderBook& sell_orders, std::vector<Trade>& trades, OrderLookup& order_lookup){
    Order o;
    auto it = order_lookup.find(id);
    if(it == order_lookup.end()){
        std::cout << "Order not found or already executed.\n";
        return;
    }
    o = *it->second.order_it;

    std::cout << std::left << std::setw(15) << "Order ID";
    std::cout << std::left << std::setw(10) << "Side";
    std::cout << std::left << std::setw(10) << "Price";
    std::cout << std::left << std::setw(12) << "Quantity" << '\n';

    std::cout << std::left << std::setw(15) << o.id;
    std::cout << std::left << std::setw(10) << ((o.side == Side::BUY) ? "BUY" : "SELL");
    std::cout << std::left << std::setw(10) << o.price;
    std::cout << std::left << std::setw(12) << o.quantity << '\n';

    remove_order(id, buy_orders, sell_orders, order_lookup);

    while(true){
        std::cout << "Enter New Price: ";
        if(std::cin >> o.price && std::isfinite(o.price) && o.price > 0) break;
        std::cout << "Invalid price. Enter a valid input." << '\n';
        std::cin.clear();
        std::cin.ignore(1000, '\n');
    }

    while(true){
        std::cout << "Enter New Quantity: ";
        if(std::cin >> o.quantity && o.quantity > 0) break;
        std::cout << "Invalid quantity. Enter a valid input." << '\n';
        std::cin.clear();
        std::cin.ignore(1000, '\n');
    }
    
    match_order(o, buy_orders, sell_orders, trades, order_lookup);
}

bool can_match(const Order& order, double resting_price){
    if(order.type == OrderType::MARKET) return true;
    if(order.side == Side::BUY) return order.price >= resting_price;
    return order.price <= resting_price;
}

bool can_fully_fill(const Order& order, const BuyOrderBook& buy_orders, const SellOrderBook& sell_orders){
    long long available_quantity = 0;

    if(order.side == Side::BUY){
        for(const auto& [price, orders] : sell_orders){
            if(!can_match(order, price)) break;
            for(const Order& resting : orders){
                available_quantity += resting.quantity;
                if(available_quantity >= order.quantity) return true;
            }
        }
    }else{
        for(const auto& [price, orders] : buy_orders){
            if(!can_match(order, price)) break;
            for(const Order& resting : orders){
                available_quantity += resting.quantity;
                if(available_quantity >= order.quantity) return true;
            }
        }
    }

    return available_quantity >= order.quantity;
}

bool order_exists(long long id, const std::unordered_set<long long>& used_order_ids){
    return used_order_ids.find(id) != used_order_ids.end();
}
