#ifndef DBCONNECTION_H
#define DBCONNECTION_H

#include <iostream>
#include <string>
#include <cppconn/driver.h>
#include <cppconn/exception.h>
#include <cppconn/prepared_statement.h>
#include <cppconn/statement.h>
#include <cppconn/resultset.h>
#include <conio.h>

class dbConnection {
public:
    dbConnection();

    ~dbConnection();

    void executeQuery(const std::string& query);
    void fetchAndDisplayData(const std::string& query);
    void fetchAndDisplayDataPivot(const std::string& query);
    std::vector<std::map<std::string, std::string>> fetchResults(const std::string& query);
    bool recordExists(const std::string& query);
    int getInt(const std::string& query, const std::vector<std::string>& params);
    std::string getString(const std::string& query, const std::vector<std::string>& params);
    sql::Connection* getConnection();
    int fetchSingleResult(const std::string& query);

private:
    const std::string server = "tcp://127.0.0.1:3306";
    const std::string username = "root";
    const std::string password = "030903";
    const std::string database = "library_system";

     sql::Driver* driver;
     sql::Connection* con;
};

#endif // DBCONNECTION_H
