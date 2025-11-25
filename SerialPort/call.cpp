#include "call.hpp"
#include "../sqliteconnector/sqliteclient.hpp"

#include <chrono>
#include <sstream>
#include <iomanip>

Call::Call(CallType type): type_(type)
{
    auto now=std::chrono::system_clock::now();
    auto timet=std::chrono::system_clock::to_time_t(now);
    auto tm=*std::localtime(&timet);
    std::stringstream ss;
    ss<<std::put_time(&tm, "%d/%m/%Y %H:%M:%S");
    datetime_=ss.str();
}

void Call::saveToDB()
{
    auto *dbInstance=SqliteClient::instance();
    dbInstance->connect();
    std::string query="INSERT INTO calls(isReceived, datetime, number, length, isMissed) VALUES("+
                        std::to_string(static_cast<unsigned>(type_))+",\""+datetime_+"\","
                        "\""+number_+"\",\""+callLength_+"\",";
    query+=missed_;
    query+=";";
    dbInstance->executeQuery(query.c_str());

}

Call::~Call()
{
    saveToDB();
}
