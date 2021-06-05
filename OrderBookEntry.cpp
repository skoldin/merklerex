#include "OrderBookEntry.h"

OrderBookEntry::OrderBookEntry(double price,
                               double amount,
                               std::string timestamp,
                               std::string product,
                               OrderBookType orderType,
                               std::string username)
    : price(price),
      amount(amount),
      timestamp(timestamp),
      product(product),
      orderType(orderType),
      username(username){};

OrderBookType OrderBookEntry::stringToOrderBookType(std::string s)
{
  if (s == "ask")
  {
    return OrderBookType::ask;
  }
  if (s == "bid")
  {
    return OrderBookType::bid;
  }

  return OrderBookType::unknown;
}