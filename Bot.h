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
    /** gets average prices for each product */
    std::map<std::string, double> getRates(std::vector<OrderBookEntry> orders);
    /** calculates exponential moving average based on simple moving average for each product */
    void calculateEMA(std::map<std::string, double> rates);
    OrderBookEntry createBid(std::string product, double price, std::string timestamp); 
    OrderBookEntry createAsk(std::string product, double price, std::string timestamp);
    /** compares EMAs and places and withdraws bids or asks based on trends */
    void placeOrders();

    std::string currentTime;
    std::vector<std::map<std::string, double>> EMAs;
    MerkelMain &app;

    double getValueInUsd(std::map<std::string, double> rates);

    /** adds an order to asks or bids */
    void addCurrentTimeAsksAndBids(const OrderBookEntry &order, std::vector<OrderBookEntry> &currentTimeAsks, std::vector<OrderBookEntry> &currentTimeBids);
public:
    Bot(MerkelMain &app);
    /** Goes through orders, does analysis on each timestamp and handles buying/selling based on the analysis  */
    void init();
};