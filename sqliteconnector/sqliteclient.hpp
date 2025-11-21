#ifndef SQLITE_CLIENT_HPP
#define SQLITE_CLIENT_HPP

#include <sqlite3.h>
#include <vector>
#include <unordered_map>
#include <string>

class SqliteClient
{
	public:
    SqliteClient(const char *fileName): fileName_(fileName){};
		void connect();

        void executeQuery(const char *query, std::vector<std::unordered_map<std::string, std::string>> *queryResult=nullptr);
		void closeConnection();
		~SqliteClient();
	private:
		sqlite3 *db;
        const char* fileName_;
		bool connected=false;
};

#endif
