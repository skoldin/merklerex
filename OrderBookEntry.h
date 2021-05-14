#pragma once

#include <string>

enum OrderBookType
{
    bid,
    ask,
    unknown
};

class OrderBookEntry
{
public:
    OrderBookEntry(double price,
                   double amount,
                   std::string timestamp,
                   std::string product,
                   OrderBookType orderType);

    static OrderBookType stringToOrderBookType(std::string);

    double price;
    double amount;
    std::string timestamp;
    std::string product;
    OrderBookType orderType;
};