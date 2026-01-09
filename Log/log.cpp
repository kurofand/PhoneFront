#include "log.hpp"
#include <fstream>
#include <unordered_map>
#include <chrono>

static const std::unordered_map<LogLevel, std::string> levelMap=
{
    {LogLevel::Error, "--[ERROR]--"},
    {LogLevel::Warning, "--[WARNING]--"},
    {LogLevel::Info, "--[INFO]--"},
    {LogLevel::Debug, "--[DEBUG]--"}
};

Log::Log(LogLevel lvl): logLevel_(lvl)
{
    static bool logFileCreated=false;
    if(!logFileCreated)
    {
        std::ofstream logFile("log.txt");
        logFileCreated=true;
        logFile.close();
    }
}

Log::~Log()
{
    stream<<std::endl;
    auto now=std::chrono::system_clock::now();
    auto timet=std::chrono::system_clock::to_time_t(now);
    auto tm=*std::localtime(&timet);
    std::ofstream logFile("log.txt", std::ofstream::app);
    //log format 00/00/0000 00:00:00 --[LEVEL]-- msg
    logFile<<std::put_time(&tm, "%d/%m/%Y %H:%M:%s")<<" "<<levelMap.at(logLevel_)<<stream.str();
    logFile.close();
}
