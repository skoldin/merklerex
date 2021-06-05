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

/** return vector of Orders according to the sent filters */
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

void OrderBook::insertOrder(OrderBookEntry &order)
{
    orders.push_back(order);

    std::sort(orders.begin(), orders.end(), OrderBookEntry::compareByTimemstamp);
}

std::vector<OrderBookEntry> OrderBook::matchAsksToBids(std::string product, std::string timestamp)
{
    std::vector<OrderBookEntry> asks, bids, sales;

    asks = OrderBook::getOrders(OrderBookType::ask, product, timestamp);
    bids = OrderBook::getOrders(OrderBookType::bid, product, timestamp);

    std::sort(asks.begin(), asks.end(), OrderBookEntry::compareByPriceAsc);
    std::sort(bids.begin(), bids.end(), OrderBookEntry::compareByPriceDesc);

    std::cout << "PRODUCT " << product << std::endl;

    for (auto &ask : asks)
    {
        for (auto &bid : bids)
        {
            if (bid.price >= ask.price)
            {
                double saleAmount;

                if (bid.amount >= ask.amount)
                {
                    saleAmount = ask.amount;

                    // make sure the bid is not processed again
                    bid.amount = bid.amount - ask.amount;
                } else if (bid.amount < ask.amount)
                {
                    saleAmount = bid.amount;

                    // the remaining amount will be processed by further bids
                    ask.amount = ask.amount - bid.amount;
                }


                // either bid or ask username is going to be simuser. Decide the type of operation based on this
                OrderBookType type = OrderBookType::asksale;

                if (bid.username == "simuser")
                {
                    type = OrderBookType::bidsale;
                }

                if (type == OrderBookType::asksale && ask.username != "simuser")
                {
                    throw "The ask was not put by simuser";
                }


                sales.push_back({ask.price, saleAmount, timestamp, product, type, "simuser"});
            }
        }
    }

    return sales;
}