#include <iostream>
#include "MerkelMain.h"
#include "OrderBookEntry.h"
#include "CSVReader.h"
#include "Wallet.h"
#include <vector>

void MerkelMain::init()
{
    int input;
    currentTime = orderBook.getEarliestTime();

    wallet.insertCurrency("BTC", 10);

    std::cout << wallet.toString() << std::endl;

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
    std::cout << "3: Make an ask" << std::endl;
    std::cout << "4: Make a bid" << std::endl;
    std::cout << "5: Print wallet" << std::endl;
    std::cout << "6: Continue" << std::endl;

    std::cout << "=================" << std::endl;
    std::cout << "Current time is: " << currentTime << std::endl;
    std::cout << "Type in 1-6" << std::endl;
}

int MerkelMain::getUserOption()
{
    std::string line;
    // init variable and assign user input to it
    int userOption;
    std::getline(std::cin, line);

    try {
        userOption = std::stoi(line);
    } catch (const std::exception &e)
    {
        std::cout << "Failed to convert menu option to int" << std::endl;
    }
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

void MerkelMain::enterOrder(OrderBookType orderBookType) {
    std::string orderTypeName = orderBookType == OrderBookType::bid ? "a bid" : "an ask";
    std::cout << "Make " << orderTypeName << " - enter the product, price, amount eg ETH/BTC,200,0.5" << std::endl;
    std::string input;

    std::getline(std::cin, input);

    std::vector<std::string> tokens = CSVReader::tokenise(input, ',');

    if (tokens.size() != 3)
    {
        std::cout << "Bad input!" << input << std::endl;
    }
    else
    {
        try
        {
            OrderBookEntry orderBookEntry = CSVReader::stringsToOrderBookEntry(
                tokens[1],
                tokens[2],
                currentTime,
                tokens[0],
                OrderBookType::bid);

            orderBookEntry.username = "simuser";    

            if (wallet.canFulfillOrder(orderBookEntry)) {
                orderBook.insertOrder(orderBookEntry);
            } else {
                std::cout << "Insufficient funds" << std::endl;
            }
        }
        catch (const std::exception &e)
        {
            std::cout << "MerkelMain::enterOrder Bad input " << input << std::endl;
        }
    }

    std::cout << "You typed: " << input << std::endl;
}

void MerkelMain::printWallet() { 
    std::cout << "Wallet contains" << std::endl;
    std::cout << wallet.toString() << std::endl;
}

void MerkelMain::gotoNextTimeFrame()
{
    std::cout << "Going to next time frame" << std::endl;

    for (std::string &product : orderBook.getKnownProducts())
    {
        std::cout << "matching " << product << std::endl;

        std::vector<OrderBookEntry> sales = orderBook.matchAsksToBids(product, currentTime);
        std::cout << "Sales: " << sales.size() << std::endl;

        for (OrderBookEntry &sale : sales)
        {
            std::cout << "Sale price: " << sale.price << " amount " << sale.amount << std::endl;
            if (sale.username == "simuser")
            {
                // TODO: DEBUG IT BUYS MORE THAN ASKED
                // TEST: MAKE A BID ETH/BTC,1,1
                // EXPECTED: BUYING 1 ETH FOR SOME BTC
                // ACTUAL: IT SEEMS ITS COMPLETELY BUYING OUT SOME BID
                wallet.processSale(sale);
            }
        }
    }

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
        enterOrder(OrderBookType::ask);
    }
    if (userOption == 4)
    {
        enterOrder(OrderBookType::bid);
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