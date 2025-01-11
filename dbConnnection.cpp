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
        int maxWidth = 15;
        sql::Statement* stmt = con->createStatement();
        sql::ResultSet* res = stmt->executeQuery(query);

        // Get the metadata for column information
        sql::ResultSetMetaData* meta = res->getMetaData();
        int columnCount = meta->getColumnCount();

        // Calculate column widths dynamically based on header size
        std::vector<int> columnWidths(columnCount, maxWidth); // Default width for each column
        for (int i = 1; i <= columnCount; ++i)
        {
            int headerWidth = meta->getColumnName(i).length();
            columnWidths[i - 1] = std::max(headerWidth, maxWidth); // Minimum width is `maxWidth`
        }

        // Utility function to wrap text into multiple lines
        auto wrapText = [](const std::string& text, int width) {
            std::vector<std::string> lines;
            std::istringstream words(text);
            std::string word, line;

            while (words >> word)
            {
                if (line.length() + word.length() + 1 > static_cast<size_t>(width))
                {
                    lines.push_back(line);
                    line.clear();
                }
                line += (line.empty() ? "" : " ") + word;
            }
            if (!line.empty())
            {
                lines.push_back(line);
            }
            return lines;
            };

        // Print the top border
        std::cout << "+";
        for (const auto& width : columnWidths)
        {
            std::cout << std::string(width, '-') << "+";
        }
        std::cout << std::endl;

        // Print table header
        std::cout << "|";
        for (int i = 1; i <= columnCount; ++i)
        {
            std::cout << YELLOW << std::setw(columnWidths[i - 1]) << std::left << meta->getColumnName(i) << RESET << "|";
        }
        std::cout << std::endl;

        // Print the header separator
        std::cout << "+";
        for (const auto& width : columnWidths)
        {
            std::cout << std::string(width, '-') << "+";
        }
        std::cout << std::endl;

        // Print each row
        while (res->next())
        {
            // Wrap text for each column
            std::vector<std::vector<std::string>> wrappedColumns(columnCount);
            int maxLines = 1;
            for (int i = 1; i <= columnCount; ++i)
            {
                wrappedColumns[i - 1] = wrapText(res->getString(i), columnWidths[i - 1]);
                maxLines = std::max(maxLines, static_cast<int>(wrappedColumns[i - 1].size()));
            }

            // Print wrapped rows (handle multi-line cells)
            for (int line = 0; line < maxLines; ++line)
            {
                std::cout << "|";
                for (int i = 0; i < columnCount; ++i)
                {
                    if (line < static_cast<int>(wrappedColumns[i].size()))
                    {
                        std::cout << std::setw(columnWidths[i]) << std::left << wrappedColumns[i][line];
                    }
                    else
                    {
                        std::cout << std::setw(columnWidths[i]) << " "; // Empty padding for non-existent lines
                    }
                    std::cout << "|";
                }
                std::cout << std::endl;
            }

            // Print a separator after each row to visually separate data
            std::cout << "+";
            for (const auto& width : columnWidths)
            {
                std::cout << std::string(width, '-') << "+";
            }
            std::cout << std::endl;
        }

        delete res;
        delete stmt;
    }
    catch (sql::SQLException& e)
    {
        std::cerr << "Error fetching data: " << e.what() << std::endl;
    }
}
