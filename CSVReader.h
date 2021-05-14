#pragma once

#include <string>
#include <vector>
#include <iostream>
#include "OrderBookEntry.h"

class CSVReader 
{
    public:
        CSVReader();
        static std::vector<OrderBookEntry> readCSV(std::string csvFile);

    private:
        static std::vector<std::string> tokenise(std::string csvLine, char separator);
        static OrderBookEntry stringsToOrderBookEntry(std::vector<std::string> strings);

};