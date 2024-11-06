#ifndef LOGGER_H
#define LOGGER_H

#include <chrono>
#include <vector>
#include "sqlite3.h"

class Logger
{
public:
    Logger ();
    ~Logger ();
    void logGripperOpen ();
    void logGripperClosed ();
private:
    bool hasLoggedOpen = false;
    time_t openTime;
    sqlite3* db;
};

#endif // LOGGER_H