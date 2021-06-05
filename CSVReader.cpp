#include <string>
#include <vector>
#include <iostream>
#include <fstream>
#include "CSVReader.h"

CSVReader::CSVReader()
{
}

std::vector<OrderBookEntry> CSVReader::readCSV(std::string csvFile)
{
    std::ifstream csvFileStream{csvFile};
    std::string line;
    std::vector<OrderBookEntry> entries;
    std::vector<std::string> tokens;

    if (csvFileStream.is_open())
    {
        while (std::getline(csvFileStream, line))
        {
            tokens = tokenise(line, ',');

            try
            {
                OrderBookEntry orderBookEntry = stringsToOrderBookEntry(tokens);
                entries.push_back(orderBookEntry);
            }
            catch (const std::exception &e)
            {
                std::cout << "CSVReader::readCSV read " << entries.size() << " entries" << std::endl;
            }
        }

        csvFileStream.close();
    }

    std::cout << "CSVReader::readCSV bad data" << std::endl;
    return entries;
}

OrderBookEntry CSVReader::stringsToOrderBookEntry(std::vector<std::string> tokens)
{
    double price, amount;

    if (tokens.size() != 5)
    {
        std::cout << "Bad line" << std::endl;
        throw std::exception{};
    }

    try
    {
        price = std::stod(tokens[3]);
        amount = std::stod(tokens[4]);
    }
    catch (const std::exception &e)
    {
        std::cout << "Bad float! " << tokens[3] << std::endl;
        std::cout << "Bad float! " << tokens[4] << std::endl;
        throw;
    }

    OrderBookEntry orderBookEntry{
        price,
        amount,
        tokens[0],
        tokens[1],
        OrderBookEntry::stringToOrderBookType(tokens[2])};

    return orderBookEntry;
}

std::vector<std::string> CSVReader::tokenise(std::string csvLine, char separator)
{
    std::vector<std::string> tokens;

    signed int start, end;

    start = csvLine.find_first_not_of(separator, 0);

    std::string token;

    do
    {
        end = csvLine.find_first_of(separator, start);

        if (start == csvLine.length() || start == end)
            break;

        if (end >= 0)
        {
            token = csvLine.substr(start, end - start);
        }
        else
        {
            token = csvLine.substr(start, csvLine.length() - start);
        }

        tokens.push_back(token);

        start = end + 1;
    } while (end > 0);

    return tokens;
}

OrderBookEntry CSVReader::stringsToOrderBookEntry(
    std::string priceString,
    std::string amountString,
    std::string timestamp,
    std::string product,
    OrderBookType orderType)
{
    double price, amount;
    try
    {
        price = std::stod(priceString);
        amount = std::stod(amountString);
    }
    catch (const std::exception &e)
    {
        std::cout << "CSVReader::stringsToOrderBookEntry: Bad float! " << price << std::endl;
        std::cout << "CSVReader::stringsToOrderBookEntry: Bad float! " << amount<< std::endl;
        throw;
    }

    OrderBookEntry orderBookEntry{
        price,
        amount,
        timestamp,
        product,
        orderType};

    return orderBookEntry;
}