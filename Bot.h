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
    OrderBookEntry createBid(std::string product, double price, std::string timestamp); 
    OrderBookEntry createAsk(std::string product, double price, std::string timestamp); 
    void placeOrders();

    std::string currentTime;
    std::vector<std::map<std::string, double>> EMAs;
    MerkelMain &app;

    void showValueInUsd(std::map<std::string, double> rates);

    /** adds an order to asks or bids */
    void addCurrentTimeAsksAndBids(const OrderBookEntry &order, std::vector<OrderBookEntry> &currentTimeAsks, std::vector<OrderBookEntry> &currentTimeBids);
public:
    Bot(MerkelMain &app);
    /** Goes through orders, does analysis on each timestamp and handles buying/selling based on the analysis  */
    void init();
};