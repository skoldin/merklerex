#include "Bot.h"
#include "OrderBook.h"
#include "MerkelMain.h"
#include <limits>

Bot::Bot(MerkelMain app): app(app), orderBook(app.getOrderBook()){}

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
        // we are interested only in asks
        if (order.orderType != OrderBookType::ask)
        {
            continue;
        }

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
    orderBook = app.getOrderBook();
    currentTime = orderBook.getEarliestTime();

    wallet.insertCurrency("USDT", 10000);
    // wallet.insertCurrency("BTC", 10);
    // wallet.insertCurrency("DOGE", 1000000);
    // wallet.insertCurrency("ETH", 300);

    std::vector<OrderBookEntry> orders = orderBook.getAllOrders();
    std::vector<OrderBookEntry> currentTimeOrders;

    for (const auto &order : orders)
    {
        // collect orders until the timestamp changes
        if (order.timestamp == currentTime)
        {
            currentTimeOrders.push_back(order);
            continue;
        }

        std::cout << "There are " << currentTimeOrders.size() << " orders on " << currentTime << std::endl;

        // once the timestamp changes, process collected orders
        // calculate SMA for each product in the timestamp
        std::map<std::string, double> rates = getRates(currentTimeOrders);
        // calculate EMA for each product in the timestamp
        calculateEMA(rates);

        // TODO: debug info
        // for (const auto &EMA : EMAs)
        // {
        //     for (const auto &test : EMA)
        //     {
        //         std::cout << "The product: " << test.first << " The EMA: " << test.second << std::endl;
        //     }
        //     std::cout << "---------" << std::endl;
        // }

        // place orders based on changes in EMAs comparing previous and current timestamps
        placeOrders();

        // std::cout << "TEST RECEIVING ORDERS FOR " << currentTime << std::endl;
        // std::vector<OrderBookEntry> result = orderBook.getOrders(OrderBookType::ask, currentTime, "BTC/USDT");
        // std::cout << "AMOUNT " << result.size() << std::endl;
        
        // remove everything from currentTimeOrders and add the current order
        currentTimeOrders.clear();
        currentTime = order.timestamp;
        currentTimeOrders.push_back(order);

        // go to the next timestamp
        app.gotoNextTimeFrame();

        std::cout << "-----------------------" << std::endl;
    }

    std::cout << wallet.toString() << std::endl;
}

void Bot::withdrawOrder(OrderBookType orderType, std::string product, std::string timestamp)
{

}

OrderBookEntry Bot::createBid(std::string product, double price, std::string timestamp)
{
    std::cout << "CREATE BID " << timestamp << std::endl;

    std::string currencyType = wallet.getOrderCurrencyType(product, OrderBookType::bid);
    double availableCurrency = wallet.checkBalance(currencyType);

    std::cout << "We have " << availableCurrency << " " << currencyType << std::endl;

    // add only 20% to market price because it's used in calculations whether we can fulfill the order
    // so if set it higher, we'll get the priority but will not be able to buy as much as we can
    OrderBookEntry orderBookEntry{
        price * 1.2, 
        availableCurrency / price,
        timestamp,
        product,
        OrderBookType::bid,
        "bot"};

    while(!wallet.canFulfillOrder(orderBookEntry))
    {
        // decrease the amount until we can fulfill the order
        if (orderBookEntry.amount > 0)
        {
            orderBookEntry.amount = orderBookEntry.amount / 2;
        }
    }

    return orderBookEntry;
}

OrderBookEntry Bot::createAsk(std::string product, double price, std::string timestamp)
{
    std::cout << "CREATE ASK " << timestamp << std::endl;

    std::string currencyType = wallet.getOrderCurrencyType(product, OrderBookType::ask);
    double availableCurrency = wallet.checkBalance(currencyType);

    std::cout << "We have " << availableCurrency << " " << currencyType << std::endl;

    OrderBookEntry orderBookEntry{
        price,
        availableCurrency,
        timestamp,
        product,
        OrderBookType::ask,
        "bot"};

    std::cout << "Ask " << availableCurrency << " " << product << std::endl;
    // if we cannnot fulfill the order, set amount to 0
    if (!wallet.canFulfillOrder(orderBookEntry))
    {
        std::cout << "The order cannot be placed" << std::endl;
        orderBookEntry.amount = 0;
    }

    return orderBookEntry;
}


void Bot::placeOrders()
{
    // if the number of EMAs < 2, there is no enough data for decisions
    if (EMAs.size() < 2)
    {
        std::cout << "Not enough data for predictions, wait for next timestamps" << std::endl;
        return;
    }

    std::cout << "START COMPARING EMAs" << std::endl;

    // EMAs for the previoius timestamp
    std::map<std::string, double> previousEMAs = EMAs[EMAs.size() - 2];
    // EMAs for the current timestamp
    std::map<std::string, double> currentEMAs = EMAs[EMAs.size() - 1];

    for (auto const &eachProductPreviousEMA : previousEMAs)
    {

        std::string productName = eachProductPreviousEMA.first;
        double previousEMA = eachProductPreviousEMA.second;
        double currentEMA = currentEMAs[productName];

        // compare previous EMA with the current one for each entry
        if (previousEMA > currentEMA)
        {
            // the price decreased, it's a good time to remove bids and create asks as we expect it might decrease further
            std::cout << productName << " price decreased from " << previousEMA << " to " << currentEMA << std::endl;

            OrderBookEntry orderBookEntry = createAsk(productName, currentEMA, currentTime);

            if (orderBookEntry.amount > 0)
            {
                // TODO: push to log
                orderBook.insertOrder(orderBookEntry);
            }

            // remove current bids
            // withdrawOrder(OrderBookType::bid, productName, currentTime);
        }
        else if (currentEMA > previousEMA)
        {
            // the price increased, it's a good time to remove asks and create bids as we expect it might increase further
            std::cout << productName << " price increased from " << previousEMA << " to " << currentEMA << std::endl;

            OrderBookEntry orderBookEntry = createBid(productName, currentEMA, currentTime);

            if (orderBookEntry.amount > 0)
            {
                // TODO: push to log
                orderBook.insertOrder(orderBookEntry);
            }

            // remove current akss
            // withdrawOrder(OrderBookType::ask, productName, currentTime);
        }
    }
}