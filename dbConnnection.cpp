#include <iostream>
#include <string>
#include <iomanip>
#include <sstream>
#include <cppconn/driver.h>
#include <cppconn/exception.h>
#include <cppconn/prepared_statement.h>
#include <cppconn/statement.h>
#include <cppconn/resultset.h>
#include <conio.h>
#include "tc.h"
#include "dbConnection.h"
#include "tabulate/table.hpp"
#include "table.h"

using namespace tabulate;

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
        con = nullptr; // Set the pointer to nullptr to prevent accidental use after deletion
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

void dbConnection::fetchAndDisplayData(const std::string& query)
{
    if (!con)
    {
        std::cerr << "No database connection available!" << std::endl;
        return;
    }
    try
    {
        sql::Statement* stmt = con->createStatement();
        sql::ResultSet* res = stmt->executeQuery(query);

        // Get the metadata for column information
        sql::ResultSetMetaData* meta = res->getMetaData();
        int columnCount = meta->getColumnCount();

        // Create a tabulate Table object
        tabulate::Table table;

        // Add column names to the table as header
        tabulate::Table::Row_t header;
        for (int i = 1; i <= columnCount; ++i)
        {
            header.push_back(meta->getColumnName(i));  // Convert column name to std::string
        }
        table.add_row(header);  // Add header row to the table

        // Add data rows to the table
        while (res->next())
        {
            tabulate::Table::Row_t row;
            for (int i = 1; i <= columnCount; ++i)
            {
                row.push_back(res->getString(i));  // Convert each data cell to std::string
            }
            table.add_row(row);  // Add the row to the table
        }

        // Print the table using tabulate
        tableFormat(table);

        delete res;
        delete stmt;
    }
    catch (sql::SQLException& e)
    {
        std::cerr << "Error fetching data: " << e.what() << std::endl;
    }
}

std::string dbConnection::extractUserIDFromQuery(const std::string& query)
{
    // This function assumes the query is in the form of:
    // "SELECT userID, name, ic, phoneNum, email, address, role FROM User WHERE userID='some_id'"
    std::string userID;

    // Find the position of "userID='"
    size_t pos = query.find("userID='");

    if (pos != std::string::npos)
    {
        // Move position to the start of the ID (after "userID='")
        pos += 8;

        // Find the closing quote
        size_t endPos = query.find("'", pos);

        if (endPos != std::string::npos)
        {
            // Extract the substring between the quotes (the userID)
            userID = query.substr(pos, endPos - pos);
        }
    }

    return userID;
}

bool dbConnection::recordExists(const std::string& query) {
    try {
        sql::Statement* stmt = con->createStatement();
        sql::ResultSet* res = stmt->executeQuery(query);
        bool exists = res->next();
        delete res;
        delete stmt;
        return exists;
    }
    catch (sql::SQLException& e) {
        std::cerr << "Error checking record existence: " << e.what() << std::endl;
        return false;
    }
}