#pragma once

#include <iostream>
#include <vector>
#include "OrderBookEntry.h"
#include "OrderBook.h"
#include "Wallet.h"

class MerkelMain
{

public:
    // MerkelMain();
    void init();
    OrderBook& getOrderBook();
    Wallet& getWallet();
    void gotoNextTimeFrame();
private:
    void printMenu();
    int getUserOption();
    void printHelp();
    void printMarketStats();
    void printWallet();
    void processUserOption(int userOption);
    void enterOrder(OrderBookType orderBookType);

    std::string currentTime;

    OrderBook orderBook{"20200317.csv"};

    Wallet wallet;
};