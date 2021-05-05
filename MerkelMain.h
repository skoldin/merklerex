#pragma once

#include <iostream>
#include <vector>
#include "OrderBookEntry.h"

class MerkelMain
{

public:
    // MerkelMain();
    void init();

private:
    std::vector<OrderBookEntry> orders;

    void loadOrderBook();

    void printMenu();

    int getUserOption();

    void printHelp();

    void printMarketStats();

    void enterOffer();

    void enterBid();

    void printWallet();

    void gotoNextTimeFrame();

    void processUserOption(int userOption);
};