
#include "Logger.h"
#include <string>
#include <fstream>
#include <iostream>

void Logger::pushToLog(std::string message)
{
    std::string logFileName = "log.txt";
    std::ofstream logFile;

    logFile.open(logFileName, std::ofstream::app);

    logFile << message << "\n";

    logFile.close();
}