#pragma once

#include <string>

class Logger
{
    public:
    /** writes the message to the log file */
    void static pushToLog(std::string message);
};