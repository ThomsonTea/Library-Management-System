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

void dbConnection::insertData(const std::string& name, int quantity)
{
    if (!con)
    {
        std::cerr << RED << YELLOW << "No database connection available!" << std::endl;
        return;
    }

    try
    {
        sql::PreparedStatement* pstmt = con->prepareStatement("INSERT INTO inventory(name, quantity) VALUES(?, ?)");
        pstmt->setString(1, name);
        pstmt->setInt(2, quantity);
        pstmt->execute();
        std::cout << "Inserted: " << name << " - " << quantity <<std::endl;
        delete pstmt;
    }
    catch (sql::SQLException& e)
    {
        std::cerr << "Error inserting data: " << name <<std::endl;
        std::cerr << "SQLException: " << e.what() <<std::endl;
    }
}

// Display all data from a specified table
void dbConnection::showData(const std::string& table)
{
    if (!con)
    {
        std::cerr << "No database connection available!" <<std::endl;
        return;
    }

    try
    {
        std::string query = "SELECT * FROM " + table;
        sql::Statement* stmt = con->createStatement();
        sql::ResultSet* res = stmt->executeQuery(query);

        if (!res->next())
        {
            std::cout << "No data found in table: " << table << "." <<std::endl;
        }
        else
        {
            sql::ResultSetMetaData* metadata = res->getMetaData();
            int columnCount = metadata->getColumnCount();

            // Print column headers
            for (int i = 1; i <= columnCount; i++)
            {
                std::cout << metadata->getColumnName(i) << "\t";
            }
            std::cout <<std::endl;

            // Print rows
            do
            {
                for (int i = 1; i <= columnCount; i++)
                {
                    std::cout << res->getString(i) << "\t";
                }
                std::cout <<std::endl;
            } while (res->next());
        }

        delete res;
        delete stmt;
    }
    catch (sql::SQLException& e)
    {
        std::cerr << "Error showing data: " << e.what() <<std::endl;
    }
}

void dbConnection::setupDatabase()
{
    executeQuery("DROP TABLE IF EXISTS inventory");
    std::cout << "Dropped table (if existed)." <<std::endl;

    executeQuery("CREATE TABLE inventory (id INT AUTO_INCREMENT PRIMARY KEY, name VARCHAR(50), quantity INT);");
    std::cout << "Created table 'inventory'." <<std::endl;

    insertData("banana", 150);
    insertData("orange", 154);
    insertData("apple", 100);
}

bool dbConnection::login(const std::string& userId, const std::string& password) {
    if (!con) {
        std::cerr << "No database connection available!" <<std::endl;
        return false;
    }

    try {
        // Prepare the SQL query to validate user credentials
        sql::PreparedStatement* pstmt = con->prepareStatement(
            "SELECT * FROM user WHERE userId = ? AND password = ?");
        pstmt->setString(1, userId);
        pstmt->setString(2, password);

        sql::ResultSet* res = pstmt->executeQuery();

        bool isAuthenticated = false;
        if (res->next()) {
            isAuthenticated = true;
            std::cout << "Login successful!" <<std::endl;
        }
        else {
            std::cout << "Invalid Id or password. Please try again..." <<std::endl;
            _getch();
        }

        delete res;
        delete pstmt;
        return isAuthenticated;

    }
    catch (sql::SQLException& e) {
        std::cerr << "Error during login: " << e.what() <<std::endl;
        return false;
    }
}
