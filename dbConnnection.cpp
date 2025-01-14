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
        system("pause");
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
            // Use getColumnLabel() instead of getColumnName() to get aliases
            header.push_back(meta->getColumnLabel(i));
        }
        table.add_row(header);

        // Add data rows to the table
        while (res->next())
        {
            tabulate::Table::Row_t row;
            for (int i = 1; i <= columnCount; ++i)
            {
                row.push_back(res->getString(i));
            }
            table.add_row(row);
        }

        // Print the table using tabulate
        tableFormat(table);

        delete res;
        delete stmt;
    }
    catch (sql::SQLException& e)
    {
        std::cerr << "Error fetching data: " << e.what() << std::endl;
        system("pause");
    }
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
        system("pause");
        return false;
    }
}

std::vector<std::map<std::string, std::string>> dbConnection::fetchResults(const std::string& query)
{
    try {
        std::unique_ptr<sql::PreparedStatement> pstmt(con->prepareStatement(query));
        std::unique_ptr<sql::ResultSet> res(pstmt->executeQuery());

        std::vector<std::map<std::string, std::string>> result;

        while (res->next()) {
            std::map<std::string, std::string> row;
            sql::ResultSetMetaData* meta = res->getMetaData();
            int columnCount = meta->getColumnCount();

            for (int i = 1; i <= columnCount; ++i) {
                std::string columnName = meta->getColumnName(i);
                std::string value = res->getString(i);
                row[columnName] = value;
            }

            result.push_back(row);
        }

        return result;
    }
    catch (const sql::SQLException& e) {
        throw std::runtime_error("SQL Error: " + std::string(e.what()));
        system("pause");
    }
    catch (const std::exception& e) {
        throw std::runtime_error("Error: " + std::string(e.what()));
        system("pause");
    }
}

int dbConnection::getInt(const std::string& query, const std::vector<std::string>& params)
{
    try
    {
        std::unique_ptr<sql::PreparedStatement> pstmt(con->prepareStatement(query));

        // Bind parameters to the prepared statement
        for (size_t i = 0; i < params.size(); ++i)
        {
            pstmt->setString(i + 1, params[i]);
        }

        // Execute the query
        std::unique_ptr<sql::ResultSet> res(pstmt->executeQuery());

        // Retrieve and return the integer result
        if (res->next())
        {
            return res->getInt(1); // Assuming the integer is the first column in the result set
        }
    }
    catch (sql::SQLException& e)
    {
        std::cerr << "SQL Error: " << e.what() << std::endl;
    }

    // Return -1 or an appropriate error value if no result was found
    return -1;
}