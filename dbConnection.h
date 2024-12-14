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
#include "tc.h"
class dbConnection {
public:
    dbConnection();

    ~dbConnection();

    void executeQuery(const std::string& query);

    void insertData(const std::string& name, int quantity);

    void showData(const std::string& table);

    bool login(const std::string& userId, const std::string& password);

    void setupDatabase();

private:
    const std::string server = "tcp://127.0.0.1:3306";
    const std::string username = "root";
    const std::string password = "030903";
    const std::string database = "library_system";

    sql::Connection* con;
    sql::Driver* driver;
};

#endif // DBCONNECTION_H
