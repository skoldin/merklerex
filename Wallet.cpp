#include <string>
#include <map>
#include "Wallet.h"
#include "CSVReader.h"

Wallet::Wallet()
{
}

void Wallet::insertCurrency(std::string type, double amount)
{
    if (amount < 0)
    {
        throw "Trying to insert negative balance";
    }

    if (currencies.count(type) == 0)
    {
        currencies[type] = 0;
    }

    currencies[type] += amount;
}
bool Wallet::removeCurrency(std::string type, double amount)
{
    if (amount < 0)
    {
        throw "Trying to insert negative balance";
    }

    if (!containsCurrency(type, amount))
    {
        return false;
    }

    currencies[type] -= amount;

    return true;
}
bool Wallet::containsCurrency(std::string type, double amount)
{
    if (currencies.count(type) == 0)
    {
        return false;
    }

    return currencies[type] >= amount;
}

std::string Wallet::toString()
{
    std::string result;
    for (std::pair<std::string, double> pair : currencies)
    {
        std::string currency = pair.first;
        double amount = pair.second;

        result += currency + " : " + std::to_string(amount) + "\n";
    }
    return result;
}

std::string Wallet::getOrderCurrencyType(std::string product, OrderBookType orderType)
{
    std::vector<std::string> orderCurrencies = CSVReader::tokenise(product, '/');

    if (orderType == OrderBookType::ask)
    {
        return orderCurrencies[0];
    }

    if (orderType == OrderBookType::bid)
    {
        return orderCurrencies[1];
    }
}

bool Wallet::canFulfillOrder(const OrderBookEntry &order)
{

    std::string currency = getOrderCurrencyType(order.product, order.orderType);

    if (order.orderType == OrderBookType::ask)
    {
        double amount = order.amount;

        std::cout << "Wallet::canFulfillOrder " << currency << " : " << amount << std::endl;
        return containsCurrency(currency, amount);
    }

    if (order.orderType == OrderBookType::bid)
    {
        double amount = order.amount * order.price;

        std::cout << "Wallet::canFulfillOrder " << currency << " : " << amount << std::endl;
        return containsCurrency(currency, amount);
    }

    return false;
}

void Wallet::processSale(OrderBookEntry &sale)
{
    std::vector<std::string> orderCurrencies = CSVReader::tokenise(sale.product, '/');

    if (sale.orderType == OrderBookType::asksale)
    {
        double outgoingAmount = sale.amount;

        std::string outgoingCurrency = orderCurrencies[0];

        double incomingAmount = sale.amount * sale.price;

        std::string incomingCurrency = orderCurrencies[1];

        currencies[incomingCurrency] += incomingAmount;
        currencies[outgoingCurrency] -= outgoingAmount;
    }

    if (sale.orderType == OrderBookType::bidsale)
    {
        double incomingAmount = sale.amount;

        std::string incomingCurrency = orderCurrencies[0];

        double outgoingAmount = sale.amount * sale.price;

        std::string outgoingCurrency = orderCurrencies[1];

        currencies[incomingCurrency] += incomingAmount;
        currencies[outgoingCurrency] -= outgoingAmount;
    }
}

double Wallet::checkBalance(std::string type)
{
    return currencies[type];
}