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
                        static_cast<unsinged>(type_)+",\""+datetime_+"\","
                        "\""+number_+"\",\""+callLength_+"\","+missed_+";";

}
