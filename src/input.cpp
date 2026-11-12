#include "input.hpp"

Order get_order(const std::unordered_set<long long>& used_order_ids){
    Order o;
    std::string sideinput;
    std::string typeinput;
    std::string timeinput;

    while(true){
        std::cout << "Enter Order ID: ";
        if(std::cin >> o.id && o.id > 0 && !order_exists(o.id, used_order_ids)) break;
        std::cout << "Invalid or previously used Order ID." << '\n';
        std::cin.clear();
        std::cin.ignore(1000, '\n');
    }

    while(true){
        std::cout << "Enter Side (BUY/SELL): ";
        if(std::cin >> sideinput){
            if(sideinput == "BUY"){
                o.side = Side::BUY;
                break;
            }else if(sideinput == "SELL"){
                o.side = Side::SELL;
                break;
            }
        }
        std::cout << "Invalid side. Enter BUY or SELL." << '\n';
        std::cin.clear();
        std::cin.ignore(1000, '\n');
    }

    while(true){
        std::cout << "Enter Order Type (LIMIT/MARKET): ";
        if(std::cin >> typeinput){
            if(typeinput == "LIMIT"){
                o.type = OrderType::LIMIT;
                break;
            }else if(typeinput == "MARKET"){
                o.type = OrderType::MARKET;
                break;
            }
        }
        std::cout << "Invalid order type. Enter LIMIT or MARKET." << '\n';
        std::cin.clear();
        std::cin.ignore(1000, '\n');
    }

    while(true){
        std::cout << "Enter Time In Force (GTC/IOC/FOK/POST ONLY): ";
        if(std::cin >> timeinput){
            if(timeinput == "GTC"){
                o.time = TimeInForce::GTC;
                break;
            }else if(timeinput == "IOC"){
                o.time = TimeInForce::IOC;
                break;
            }else if(timeinput == "FOK"){
                o.time = TimeInForce::FOK;
                break;
            }else if(timeinput == "POST ONLY"){
                o.time = TimeInForce::POST_ONLY;
                break;
            }
        }
        std::cout << "Invalid time in force. Enter GTC, IOC, FOK or POST ONLY." << '\n';
        std::cin.clear();
        std::cin.ignore(1000, '\n');
    }

    if(o.type == OrderType::MARKET){
        while(o.time != TimeInForce::IOC && o.time != TimeInForce::FOK){
            std::cout << "Market orders only support IOC or FOK.\n";
            std::cout << "Enter Time In Force (IOC/FOK): ";
            if(std::cin >> timeinput){
                if(timeinput == "IOC"){
                    o.time = TimeInForce::IOC;
                    break;
                }else if(timeinput == "FOK"){
                    o.time = TimeInForce::FOK;
                    break;
                }
            }
            std::cout << "Invalid time in force.\n";
            std::cin.clear();
            std::cin.ignore(1000, '\n');
        }
        o.price = 0;
    }else{
        while(true){
            std::cout << "Enter Price: ";
            if(std::cin >> o.price && std::isfinite(o.price) && o.price > 0) break;
            std::cout << "Invalid price. Enter a valid input." << '\n';
            std::cin.clear();
            std::cin.ignore(1000, '\n');
        }
    }

    while(true){
        std::cout << "Enter Quantity: ";
        if(std::cin >> o.quantity && o.quantity > 0) break;
        std::cout << "Invalid quantity. Enter a valid input." << '\n';
        std::cin.clear();
        std::cin.ignore(1000, '\n');
    }

    return o;
}
