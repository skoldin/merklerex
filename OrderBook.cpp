#include "OrderBook.h"
#include "CSVReader.h"
#include <map>
#include <algorithm>
#include "Logger.h"

/** construct, reading a csv data file */
OrderBook::OrderBook(std::string filename)
{
    orders = CSVReader::readCSV(filename);
};

/** return vector of all known products in the dataset */
std::vector<std::string> OrderBook::getKnownProducts()
{
    std::vector<std::string> products;
    std::map<std::string, bool> prodMap;

    for (OrderBookEntry &entry : orders)
    {
        prodMap[entry.product] = true;
    }

    for (auto const &entry : prodMap)
    {
        products.push_back(entry.first);
    }

    return products;
};

/** return all orders */

/** return vector of Orders according to the sent filters */
std::vector<OrderBookEntry> OrderBook::getOrders(
    OrderBookType type,
    std::string timestamp,
    std::string product)
{
    std::vector<OrderBookEntry> ordersSub;

    for (OrderBookEntry &entry : orders)
    {
        if (entry.orderType == type &&
            entry.product == product &&
            entry.timestamp == timestamp)
        {
            ordersSub.push_back(entry);
        }
    }

    return ordersSub;
};

double OrderBook::getHighPrice(std::vector<OrderBookEntry> &orders)
{
    double max = orders[0].price;

    for (const auto &order : orders)
    {
        if (order.price > max)
        {
            max = order.price;
        }
    }

    return max;
}

double OrderBook::getLowPrice(std::vector<OrderBookEntry> &orders)
{
    double min = orders[0].price;

    for (const auto &order : orders)
    {
        if (order.price < min)
        {
            min = order.price;
        }
    }

    return min;
}

std::string OrderBook::getEarliestTime()
{
    return orders[0].timestamp;
}

std::string OrderBook::getNextTime(std::string timestamp)
{
    for (const auto &order : orders)
    {
        if (timestamp < order.timestamp)
        {
            return order.timestamp;
        }
    }

    return orders[0].timestamp;
}
// returns the change of price between the first and the last order as a percentage
double OrderBook::getPriceChangePercentage(std::vector<OrderBookEntry> &orders)
{
    double startPrice = orders[0].price;
    double endPrice = orders[orders.size() - 1].price;
    double diff = endPrice - startPrice;

    float percent = diff / (startPrice / 100);

    return percent;
}

void OrderBook::insertOrder(OrderBookEntry &order)
{
    std::string message = "Inserted an order: " +
                          OrderBookEntry::orderBookTypeToString(order.orderType) + "," + order.product + "," + std::to_string(order.price) +
                          "," + std::to_string(order.amount) + "," + order.timestamp;
    std::cout << "OrderBook::insertOrder " << message << std::endl;
    Logger::pushToLog(message);

    orders.push_back(order);
}

std::vector<OrderBookEntry> OrderBook::matchAsksToBids(std::string product, std::string timestamp)
{
    std::vector<OrderBookEntry> asks, bids, sales;

    asks = OrderBook::getOrders(OrderBookType::ask, timestamp, product);
    bids = OrderBook::getOrders(OrderBookType::bid, timestamp, product);

    std::sort(asks.begin(), asks.end(), OrderBookEntry::compareByPriceAsc);
    std::sort(bids.begin(), bids.end(), OrderBookEntry::compareByPriceDesc);

    unsigned int askIndex = 0, bidIndex = 0;

    while (askIndex < asks.size() && bidIndex < bids.size())
    {
        OrderBookEntry &ask = asks[askIndex];
        OrderBookEntry &bid = bids[bidIndex];

        if (bid.price < ask.price)
        {
            bidIndex += 1;
            continue;
        }

        double saleAmount;

        // fully buy out the ask
        if (bid.amount >= ask.amount)
        {
            saleAmount = ask.amount;

            // make sure the bid is not processed again
            bid.amount = bid.amount - ask.amount;

            askIndex += 1;
        }
        else // fully sell out the bid
        {
            saleAmount = bid.amount;

            // the remaining amount will be processed by further bids
            ask.amount = ask.amount - bid.amount;

            bidIndex += 1;
        }

        // either bid or ask username is going to be simuser. Decide the type of operation based on this
        OrderBookType type = OrderBookType::asksale;
        std::string username = ask.username;

        if (bid.username == "simuser" || bid.username == "bot")
        {
            type = OrderBookType::bidsale;
            username = bid.username;
        }

        if (type == OrderBookType::asksale && (ask.username != "simuser" || ask.username != "bot"))
        {
            std::cout << "The ask sale was not put by simuser" << std::endl;
            throw "The ask sale was not put by simuser";
        }

        Logger::pushToLog("Make a sell: " + OrderBookEntry::orderBookTypeToString(type) + "," + product + "," +
                          std::to_string(ask.price) + "," + std::to_string(saleAmount) + "," + timestamp);
        sales.push_back({ask.price, saleAmount, timestamp, product, type, username});
    }

    return sales;
}

const std::vector<OrderBookEntry> &OrderBook::getAllOrders()
{
    return orders;
}

void OrderBook::withdrawOrders(
    OrderBookType orderType,
    std::string product,
    std::string username)
{

    std::vector<OrderBookEntry> filteredOrders;

    for (const auto &order : orders)
    {
        if (orderType == order.orderType &&
            product == order.product &&
            username == order.username)
        {
            std::string message = "Withdrawn an order: " + OrderBookEntry::orderBookTypeToString(orderType) + "," + product;
            std::cout << "Bot::withdrawOrder " << message << std::endl;
            Logger::pushToLog(message);
            continue;
        }

        filteredOrders.push_back(order);
    }

    orders = filteredOrders;
    std::cout << orders.size() << std::endl;
}