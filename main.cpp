#include <iostream>
#include <string>
#include <vector>
#include "OrderBookEntry.h"
#include "MerkelMain.h"
#include "CSVReader.h"
#include "Bot.h"

int main()
{
    MerkelMain app{};
    // app.init();

    Bot bot{app};
    bot.init();

    return 0;
}