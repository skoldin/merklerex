#pragma once
#include "OrderBookEntry.h"
#include "CSVReader.h"
#include <string>
#include <vector>
#include <map>

class OrderBook
{
    public:
        /** construct, reading a csv data file */
        OrderBook(std::string filename);

        /** return vector of all known products in the dataset */
        std::vector<std::string> getKnownProducts();

        /** return vector of Orders according to thhe sent filters */
        std::vector<OrderBookEntry> getOrders(
            OrderBookType type, 
            std::string product, 
            std::string timestamp
        );

        static double getHighPrice(std::vector<OrderBookEntry>& orders);
        static double getLowPrice(std::vector<OrderBookEntry>& orders);
        std::string getEarliestTime();
        /** Returns the next time after the timestamp passed in
         * If there is no next timestamp, return the earliest to get back to the start
         */
        std::string getNextTime(std::string timestamp);

        /** Returns percentage in price change for each product */
        static double getPriceChangePercentage(std::vector<OrderBookEntry>& orders);

    private:
       std::vector<OrderBookEntry> orders;
};