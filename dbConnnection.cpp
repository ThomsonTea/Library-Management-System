#include <iostream>
#include <cppconn/driver.h>
#include <cppconn/exception.h>
#include <cppconn/prepared_statement.h>
#include <cppconn/statement.h>
#include <cppconn/resultset.h>
#include <conio.h>
#include "tc.h"
#include "dbConnection.h"

dbConnection::dbConnection()
{
    try
    {
        driver = get_driver_instance();
        con = driver->connect(server, username, password);
        con->setSchema(database);
        std::cout << GREEN << "Connected to the database successfully." << RESET << std::endl;
    }
    catch (sql::SQLException& e)
    {
        std::cerr << BG_YELLOW << "Error connecting to the database: "<< RED << e.what() << RESET << std::endl;
        con = nullptr;
    }
}

dbConnection::~dbConnection()
{
    if (con)
    {
        delete con;
        std::cout << "Database connection closed." << std::endl;
    }
}

void dbConnection::executeQuery(const std::string& query)
{
    if (!con)
    {
        std::cerr << "No database connection available!" << std::endl;
        return;
    }
    try
    {
        sql::Statement* stmt = con->createStatement();
        stmt->execute(query);
        delete stmt;
    }
    catch (sql::SQLException& e)
    {
        std::cerr << RED << YELLOW << "Error executing query: " << query << std::endl;
        std::cerr << "SQLException: " << e.what()  << RESET << std::endl;
    }
}

sql::Connection* dbConnection::getConnection() 
{
    return con;  // Return the connection object
}