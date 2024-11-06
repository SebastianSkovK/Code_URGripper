#include <iostream>
#include <fstream>
#include <string.h>
#include <chrono>
#include "sqlite3.h"
#include <array>
#include <vector>
#include <thread>
//#include "writeDriver.cpp"
using namespace std::this_thread;
using namespace std::chrono;
std::vector<std::string> data;
static int out = 15;
static bool lastValue = false;
static double guageValue = 0;
static double gaugeMax = 100;
static bool stop = false;
bool guageCheck(int guageValue)
{
    if(guageValue < gaugeMax && guageValue > 0)
        return true;
    else
        return false;

}
bool openCloseCheck(bool input)
{

    if(input != lastValue)
    {
        out++;
        return true;

    }
    else
        return false;
}
bool stopCheck(bool stop)
{
    if(stop)
    {
        return true;
    }
    else
         return guageCheck(guageValue);

}
void guageDriver()
{
    if(guageValue > 0 && gaugeMax > guageValue)
    {

        data.push_back(std::to_string(guageValue));
    }
}
void startDriverTimer ()
{
    auto start = std::chrono::steady_clock::now();

    

void endDriverTimer ()
{
    if(guageCheck(guageValue) || stopCheck(stop))
        {
            auto end = std::chrono::steady_clock::now();
            std::chrono::duration<double> elapsed_seconds = end-start;
            std::string duration = std::to_string(elapsed_seconds.count());
            std:: cout << "elapsed time: " << duration << "s \n" << std::endl;
            data.push_back(duration);
        }
    }
}

void writeDriver()
{
        sqlite3* DB;
        // Tjekker om DB fil eksistere
        std::fstream dbCheck;
        dbCheck.open("robot.db");
        char* messaggeError;
        if(dbCheck.fail())
        {
            std::string createDB = "CREATE TABLE robot("
                              "RUNTIMEA DOUBLE(255,15) NOT NULL,FORCEA DOUBLE(255,15) NOT NULL,RUNTIMEB DOUBLE(255,15) NOT NULL,FORCEB DOUBLE(255,15) NOT NULL);";

            int dbAction = sqlite3_open("robot.db", &DB);

            dbAction = sqlite3_exec(DB, createDB.c_str(), NULL, 0, &messaggeError);

        if (dbAction != SQLITE_OK) {
            std::cout << "Error Create Table" << std::endl;
            sqlite3_free(messaggeError);
        }
        else
            std::cout << "Table created Successfully" << std::endl;
        }
        dbCheck.close();
        std::string driveTime;
        for(int i = 0; i < data.size(); i++)
        {

            if(data.size() > 1 && data.at(i) != data.back())
            {
                data.at(i) += ",";
            }
            driveTime += data.at(i);
        }


        int dbAction = sqlite3_open("robot.db", &DB);
        std::string sql = "INSERT INTO robot(RUNTIMEA,FORCEA,RUNTIMEB,FORCEB)VALUES(";
        sql+=driveTime+");";
        std::cout << sql << std::endl;
        dbAction = sqlite3_exec(DB, sql.c_str(), NULL, 0, &messaggeError);
        if(dbAction != SQLITE_OK)
        {
            std::cout << "Error inserting data" << std::endl;
            sqlite3_free(messaggeError);
        }
        else
        std::cout << "Insertion complete" << std::endl;
        sqlite3_close(DB);
}
static int callback(void *data, int argc, char **argv, char **azColName){
   int i;
   fprintf(stderr, "%s", (const char*)data);

   for(i = 0; i<argc; i++){
      printf("%s = %ss \n", azColName[i], argv[i] ? argv[i] : "NULL");
   }

   printf("\n");
   return 0;
}
void graphData()
{
    sqlite3* DB;
    // Tjekker om DB fil eksistere
     const char* data = "Database rows: \n";
    std::fstream dbCheck;
    dbCheck.open("robot.db");
    char* messaggeError;
    if(dbCheck.fail())
    {
        std::cout << "Database not found" << std::endl;

    }
    else
    {
        int dbAction = sqlite3_open("robot.db", &DB);
        std::string sql = "SELECT * FROM robot";

        dbAction = sqlite3_exec(DB, sql.c_str(), callback, (void*)data, &messaggeError);

    }

}