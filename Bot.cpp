#include "Bot.h"
#include "OrderBook.h"

std::map<std::string, double> Bot::getRates(std::vector<OrderBookEntry> orders)
{
    // store products and their corresponding simple averages
    std::map<std::string, double> rates;
    // store total sum for each product
    std::map<std::string, double> sums;
    // store the number of appearances for each product
    std::map<std::string, unsigned long> counts;

    std::cout << "Got " << orders.size() << " orders" << std::endl;

    for (const OrderBookEntry &order : orders)
    {
        // if the product is not added yet, itinialize count to 1 and sum to the current price
        // otherwise, increment the count and add the price to existing
        if (sums.find(order.product) == rates.end())
        {
            counts.insert({order.product, 1});
            sums.insert({order.product, order.price});
        }
        else
        {
            // std::cout << "The product appears more than once " << order.product << std::endl;
            counts[order.product] += 1;
            sums[order.product] += order.price;
        }
    }

    // iterate through sums map and calculate simple moving average for each
    for (const auto &sum : sums)
    {
        // calculate SMA by dividing the total price (stored as value) in sums by the count received from counts by the product name (stored as key)
        double SMA = sum.second / counts[sum.first];
        rates.insert({sum.first, SMA});
    }

    return rates;
}

void Bot::calculateEMA(std::map<std::string, double> rates)
{
    std::map<std::string, double> currentEMAs;

    // go through all SMAs
    for (auto const &rate : rates)
    {
        double currentEMA = rate.second;

        // check if there is the last EMA for this product
        if (EMAs.size() > 0 && EMAs[EMAs.size() - 1][rate.first] > 0)
        {
            double lastEMA = EMAs[EMAs.size() - 1][rate.first];

            // calculate current EMA considering the previoius one
            currentEMA = (currentEMA * 0.5) + (lastEMA * 0.5);
        }

        currentEMAs[rate.first] = currentEMA;
    }

    EMAs.push_back(currentEMAs);

    // remove outdated records
    unsigned int maxEMAsSize = 5;

    if (EMAs.size() > maxEMAsSize)
    {
        EMAs.erase(EMAs.begin());
    }
}

/** Starts the bot with some initial currency */
void Bot::init()
{
    currentTime = orderBook.getEarliestTime();

    wallet.insertCurrency("BTC", 10);

    std::vector<OrderBookEntry> orders = orderBook.getAllOrders();
    std::string currentTimestamp = currentTime;
    std::vector<OrderBookEntry> currentTimeOrders;

    for (const auto &order : orders)
    {
        // collect orders until the timestamp changes
        if (order.timestamp == currentTimestamp)
        {
            currentTimeOrders.push_back(order);
            continue;
        }

        std::cout << "There are " << currentTimeOrders.size() << " orders on " << currentTimestamp << std::endl;

        // once the timestamp changes, process collected orders
        // calculate SMA for each product in the timestamp
        std::map<std::string, double> rates = getRates(currentTimeOrders);
        // calculate EMA for each product in the timestamp
        calculateEMA(rates);

        // TODO: debug info
        for (const auto &EMA : EMAs)
        {
            for (const auto &test : EMA)
            {
                std::cout << "The product: " << test.first << " The EMA: " << test.second << std::endl;
            }
            std::cout << "---------" << std::endl;
        }

        // remove everything from currentTimeOrders and add the current order
        currentTimeOrders.clear();
        currentTimestamp = order.timestamp;
        currentTimeOrders.push_back(order);
    }
}

void Bot::handleOrders()
{
    // if the number of EMAs < 2, there is no enough data for decisions
    if (EMAs.size() < 2)
    {
        std::cout << "Not enough data for predictions, wait for next timestamps" << std::endl;
        return;
    }

    // EMAs for the previoius timestamp
    std::map<std::string, double> previousEMAs = EMAs[EMAs.size() - 2];
    // EMAs for the current timestamp
    std::map<std::string, double> currentEMAs = EMAs[EMAs.size() - 1];

    // TODO: loop through known products, for each compare EMA if exists


}