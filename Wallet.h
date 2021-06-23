#pragma once

#include <string>
#include <map>
#include "OrderBookEntry.h"

class Wallet
{
    public:
        Wallet();
        /** insert currency to the wallet */
        void insertCurrency(std::string type, double amount);
        /** remove currency from the wallet */
        bool removeCurrency(std::string type, double amount);
        /** check if there is enough of this currency in the wallet */
        bool containsCurrency(std::string type, double amount);

        /** check if an ask or bid can be accomodated with the current wallet content  */
        bool canFulfillOrder(const OrderBookEntry &order);
        /** update the contents of the wallet assuming the order was made by the owner of the wallet */
        void processSale(OrderBookEntry &sale);
        /** check the amount of specified currency in the wallet */
        double checkBalance(std::string type);
        /** get the type of currency we need to fulfull the order */
        std::string getOrderCurrencyType(std::string product, OrderBookType orderType);

        /** generate a string representation of the wallet */
        std::string toString();
    private:
        std::map<std::string, double> currencies;
};