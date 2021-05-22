#include "OrderBook.h"
#include "CSVReader.h"
#include <map>
#include <algorithm>

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

/** return vector of Orders according to thhe sent filters */
std::vector<OrderBookEntry> OrderBook::getOrders(
    OrderBookType type,
    std::string product,
    std::string timestamp)
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