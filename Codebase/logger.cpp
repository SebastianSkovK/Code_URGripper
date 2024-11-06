#include "logger.h"

#include <string>
#include <iostream>
#include <fstream>
#include <stdexcept>
#include <ctime>
#include "sqlite3.h"

Logger::Logger()
{
    // Tjekker om DB fil eksistere
    std::fstream dbCheck;
    dbCheck.open("robot.db");
    char* messaggeError;

    if(dbCheck.fail())
    {
        std::string createDB = "CREATE TABLE robot("
                        "OPEN BIGINT UNSIGNED,"
                        "CLOSE BIGINT UNSIGNED,"
                        "RUNTIME INT"
                        ");";
        
        int dbAction = sqlite3_open("robot.db", &db);

        dbAction = sqlite3_exec(db, createDB.c_str(), NULL, 0, &messaggeError);

        if (dbAction != SQLITE_OK)
        {
            std::cerr << "Error Create Table" << std::endl;
            sqlite3_free(messaggeError);
        } else
        std::cerr << "Table created Successfully" << std::endl;
    } else
    {
        int dbAction = sqlite3_open("robot.db", &db);
    }
    dbCheck.close();
}

Logger::~Logger()
{
    sqlite3_close (db);
}

void Logger::logGripperOpen()
{
    if (hasLoggedOpen)
        return;

    openTime = std::time (nullptr);
    hasLoggedOpen = true;
}

void Logger::logGripperClosed ()
{
    if (!hasLoggedOpen)
       return;

    time_t closeTime = std::time (nullptr);
    hasLoggedOpen = false;

    std::string sql = "INSERT INTO robot (OPEN, CLOSE, RUNTIME) VALUES ("
        + std::to_string (openTime)
        + ", "
        + std::to_string (closeTime)
        + ", "
        + std::to_string (closeTime - openTime)
        + ");";

    std::cout << "Executing SQL: " << sql << std::endl;

    char* messageError;
    int dbAction = sqlite3_exec(db, sql.c_str(), NULL, 0, &messageError);

    if(dbAction != SQLITE_OK)
    {
        std::cerr << "Error inserting data" << std::endl;
        sqlite3_free(messageError);
    } else
        std::cout << "Insertion complete" << std::endl;
}