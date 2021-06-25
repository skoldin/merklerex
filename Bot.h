#pragma once

#include <map>
#include <string>
#include "OrderBookEntry.h"
#include "OrderBook.h"
#include "Wallet.h"
#include "MerkelMain.h"

class Bot
{
private:
    // TODO: add comments
    std::map<std::string, double> getRates(std::vector<OrderBookEntry> orders);
    void calculateEMA(std::map<std::string, double> rates);
    void withdrawOrder(OrderBookType orderType, std::string product, std::string timestamp);
    OrderBookEntry createBid(std::string product, double price, std::string timestamp); 
    OrderBookEntry createAsk(std::string product, double price, std::string timestamp); 
    void placeOrders();

    std::string currentTime;
    std::vector<std::map<std::string, double>> EMAs;
    Wallet wallet;
    MerkelMain app;
    OrderBook orderBook;
public:
    Bot(MerkelMain app);
    void init();
};