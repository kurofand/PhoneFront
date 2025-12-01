#ifndef SQLITE_CLIENT_HPP
#define SQLITE_CLIENT_HPP

#include <sqlite3.h>
#include <vector>
#include <unordered_map>
#include <string>

class SqliteClient
{
	public:
        SqliteClient(){};
        static SqliteClient* instance();
        void connect(const char *fileName);
        void connect(bool createDB=false);

        void executeQuery(const char *query, std::vector<std::unordered_map<std::string, std::string>> *queryResult=nullptr);
		void closeConnection();
		~SqliteClient();
	private:
        void prepareDB();
        inline static SqliteClient *instance_=nullptr;
		sqlite3 *db;
        const char* fileName_;
		bool connected=false;

        //static because to lazy to edit array length on adding/removing tables
        inline static const char* smsTableSQL="CREATE TABLE sms(id INTEGER PRIMARY KEY AUTOINCREMENT,"
                                  "isReceived BOOLEAN NOT NULL CHECK(isReceived IN (0,1)),"
                                  "datetime CHARACTER(19) NOT NULL,"
                                  "number VARCHAR(20) NOT NULL,"
                                  "msg TEXT,"
                                  "isRead BOOLEAN NOT NULL CHECK(isRead IN (0,1)));";

        inline static const char* callsTableSQL="CREATE TABLE calls(id INTEGER PRIMARY KEY AUTOINCREMENT,"
                                    "isReceived BOOLEAN NOT NULL CHECK(isReceived IN (0,1)),"
                                    "datetime CHARACTER(19) NOT NULL,"
                                    "number VARCHAR(20) NOT NULL,"
                                    "length CHARACTER(5) NOT NULL,"
                                    "isMissed BOOLEAN NOT NULL CHECK(isMissed IN (0,1)));";

        inline static const char* contactsTableSQL="CREATE TABLE contacts(id INTEGER PRIMARY KEY AUTOINCREMENT,"
                                        "name VARCHAR(50) NOT NULL,"
                                        "memo TEXT);";

        inline static const char* savedNumbersTableSQL="CREATE TABLE savedNumbers(id INTEGER PRIMARY KEY AUTOINCREMENT,"
                                           "contactsId INTEGER NOT NULL,"
                                           "number VARCHAR(20) NOT NULL);";

        inline static const char* tablesToCreate[]={smsTableSQL, callsTableSQL, contactsTableSQL, savedNumbersTableSQL};
};

#endif
