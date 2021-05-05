#include "tokenize.h"
#include <fstream>
#include <string>

int main()
{
    // std::string input = "Hello,cruel,world";

    // std::vector<std::string> result = tokenise(input, ',');

    // for(std::string& token: result) {
    //    std::cout << token << std::endl;
    // }

    std::ifstream csvFile{"20200317.csv"};
    std::string line;
    std::vector<std::string> tokens;

    if (csvFile.is_open())
    {
        std::cout << "File open " << std::endl;

        while (std::getline(csvFile, line))
        {
            std::cout << "Read line " << line << std::endl;
            tokens = tokenise(line, ',');

            if (tokens.size() != 5)
            {
                std::cout << "Bad line" << std::endl;
                continue;
            }

            try
            {
                double price = std::stod(tokens[3]);
                double amount = std::stod(tokens[4]);
            }
            catch (std::exception &e)
            {
                std::cout << "Bad float! " << tokens[3] << std::endl;
                std::cout << "Bad float! " << tokens[4] << std::endl;

                break;
            }
        }

        csvFile.close();
    }
    else
    {
        std::cout << "Could not open file " << std::endl;
    }

    return 0;
}