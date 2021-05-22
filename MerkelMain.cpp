#include <iostream>
#include "MerkelMain.h"
#include "OrderBookEntry.h"
#include "CSVReader.h"
#include <vector>

void MerkelMain::init()
{
    int input;
    currentTime = orderBook.getEarliestTime();
    while (true)
    {

        printMenu();
        input = getUserOption();
        processUserOption(input);
    }
}

void MerkelMain::printMenu()
{
    std::cout << "1: Print help" << std::endl;
    std::cout << "2: Print exchange stats" << std::endl;
    std::cout << "3: Make an offer" << std::endl;
    std::cout << "4: Make a bid" << std::endl;
    std::cout << "5: Print wallet" << std::endl;
    std::cout << "6: Continue" << std::endl;

    std::cout << "=================" << std::endl;
    std::cout << "Current time is: " << currentTime << std::endl;
    std::cout << "Type in 1-6" << std::endl;
}

int MerkelMain::getUserOption()
{
    // init variable and assign user input to it
    int userOption;
    std::cin >> userOption;
    std::cout << "You chose: " << userOption << std::endl;

    return userOption;
}

void MerkelMain::printHelp()
{
    std::cout << "Your aim is to make money, analyze the market, and make bids and offers." << std::endl;
}

void MerkelMain::printMarketStats()
{
    for (std::string const &product : orderBook.getKnownProducts())
    {
        std::cout << "Product: " << product << std::endl;

        std::vector<OrderBookEntry> entries = orderBook.getOrders(
            OrderBookType::ask,
            product,
            currentTime);

        std::cout << "Asks seen: " << entries.size() << std::endl;
        std::cout << "Max ask: " << OrderBook::getHighPrice(entries) << std::endl;
        std::cout << "Min ask: " << OrderBook::getLowPrice(entries) << std::endl;
        
        // debug
        std::cout << "The price has changed by " << OrderBook::getPriceChangePercentage(entries) << "%" << std::endl;
    }


}

void MerkelMain::enterOffer()
{
    std::cout << "Mark and offer- enter the amount" << std::endl;
}

void MerkelMain::enterBid()
{
    std::cout << "Make a bid - enter the amount" << std::endl;
}

void MerkelMain::printWallet() { std::cout << "Your wallet is empty" << std::endl; }

void MerkelMain::gotoNextTimeFrame()
{
    std::cout << "Going to next time frame" << std::endl;
    currentTime = orderBook.getNextTime(currentTime);
}

void MerkelMain::processUserOption(int userOption)
{
    if (userOption == 0)
    {
        std::cout << "Invalid choice. Choose 1-6" << std::endl;
    }
    if (userOption == 1)
    {
        printHelp();
    }
    if (userOption == 2)
    {
        printMarketStats();
    }
    if (userOption == 3)
    {
        enterOffer();
    }
    if (userOption == 4)
    {
        enterBid();
    }
    if (userOption == 5)
    {
        printWallet();
    }
    if (userOption == 6)
    {
        gotoNextTimeFrame();
    }
}