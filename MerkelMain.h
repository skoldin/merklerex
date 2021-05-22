#pragma once

#include <iostream>
#include <vector>
#include "OrderBookEntry.h"
#include "OrderBook.h"

class MerkelMain
{

public:
    // MerkelMain();
    void init();

private:
    void printMenu();
    int getUserOption();
    void printHelp();
    void printMarketStats();
    void enterOffer();
    void enterBid();
    void printWallet();
    void gotoNextTimeFrame();
    void processUserOption(int userOption);

    std::string currentTime;

    OrderBook orderBook{"20200317.csv"};
};