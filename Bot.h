#pragma once

#include <map>
#include <string>
#include "OrderBookEntry.h"
#include "OrderBook.h"
#include "Wallet.h"

class Bot
{
private:
    // TODO: add comments
    std::map<std::string, double> getRates(std::vector<OrderBookEntry> orders);
    void calculateEMA(std::map<std::string, double> rates);
    void withdrawOrder(OrderBookType orderType, std::string product, std::string timestamp);
    OrderBookEntry createBid(std::string product, double price, std::string timestamp); 
    void handleOrders();

    std::string currentTime;
    std::vector<std::map<std::string, double>> EMAs;
    OrderBook orderBook{"20200317.csv"};
    Wallet wallet;
public:
    void init();
};