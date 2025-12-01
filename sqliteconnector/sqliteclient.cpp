#include <cstring>
#include <filesystem>
#include <fstream>
#include "sqliteclient.hpp"

void SqliteClient::connect(const char* fileName)
{
    fileName_=fileName;
    connect();
}

SqliteClient* SqliteClient::instance()
{
    if(!instance_)
        instance_=new SqliteClient();
    return instance_;
}

void SqliteClient::connect(bool createDB)
{
    if(connected)
        return;
    const std::filesystem::path dbPath="db";
    if(!std::filesystem::exists(dbPath))
    {
        prepareDB();
        return;
    }
	uint8_t rc;
    rc=sqlite3_open("db/phone.db", &db);
	if(rc)
	{
		fprintf(stderr, "Can't open database: %s\n", sqlite3_errmsg(db));
		return;
	}
	else
		fprintf(stderr, "Opened db successfully\n");
	this->connected=true;
    if(createDB)
    {
        for(unsigned i=0;i<(sizeof(tablesToCreate)/sizeof(char*));i++)
            executeQuery(tablesToCreate[i]);
    }
}


void SqliteClient::executeQuery(const char *query, std::vector<std::unordered_map<std::string, std::string>> *queryResult)
{
	if(!connected)
		return;

	uint8_t i=0;
	std::string queryType="";
	size_t querySize=strlen(query);
	while(query[i]!=' '&&i<querySize)
		queryType+=query[i++];
	if(queryType=="SELECT")
	{
        if(queryResult==nullptr)
            return;
		sqlite3_stmt *stmt;
		sqlite3_prepare_v2(db, query, -1, &stmt, NULL);
		auto colCount=sqlite3_column_count(stmt);
		sqlite3_step(stmt);
		while(sqlite3_column_text(stmt, 0))
		{
			std::unordered_map<std::string, std::string> row;
			for(uint8_t i=0;i<colCount;i++)
				row[std::string((char *)sqlite3_column_name(stmt, i))]=std::string((char *)sqlite3_column_text(stmt, i));
			sqlite3_step(stmt);
            queryResult->push_back(row);
		}
	}
	else
		sqlite3_exec(db, query, NULL, 0, 0);

}


void SqliteClient::closeConnection()
{
	if(this->connected)
		sqlite3_close(db);
    connected=false;
}

void SqliteClient::prepareDB()
{
    std::filesystem::create_directory("db");
    std::ofstream{"db/phone.db"};
    connect(true);
}

SqliteClient::~SqliteClient()
{
    this->closeConnection();
}

