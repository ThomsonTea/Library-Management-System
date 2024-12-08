#include <iostream>
#include <sstream>
#include <cppconn/driver.h>
#include <cppconn/exception.h>
#include <cppconn/prepared_statement.h>
#include <cppconn/statement.h>
#include <cppconn/resultset.h>
#include <conio.h>
#include "tc.h"
using namespace std;

class dbConnection
{
private:
    const string server = "tcp://127.0.0.1:3306";
    const string username = "root";
    const string password = "030903";
    const string database = "library_system";

    sql::Connection* con;
    sql::Driver* driver;

public:
    // Constructor: Establish connection to the database server
    dbConnection()
    {
        try
        {
            driver = get_driver_instance();
            con = driver->connect(server, username, password);
            con->setSchema(database);
            cout << "Connected to the database successfully." << endl;
        }
        catch (sql::SQLException& e)
        {
            cerr << "Error connecting to the database: " << e.what() << endl;
            con = nullptr;
        }
    }

    // Destructor: Release resources
    ~dbConnection()
    {
        if (con)
        {
            delete con;
            cout << "Database connection closed." << endl;
        }
    }

    void executeQuery(const string& query)
    {
        if (!con)
        {
            cerr << "No database connection available!" << endl;
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
            cerr << RED << YELLOW << "Error executing query: " << query << endl;
            cerr << "SQLException: " << e.what()  << RESET << endl;
        }
    }

    void insertData(const string& name, int quantity)
    {
        if (!con)
        {
            cerr << RED << YELLOW << "No database connection available!" << endl;
            return;
        }

        try
        {
            sql::PreparedStatement* pstmt = con->prepareStatement("INSERT INTO inventory(name, quantity) VALUES(?, ?)");
            pstmt->setString(1, name);
            pstmt->setInt(2, quantity);
            pstmt->execute();
            cout << "Inserted: " << name << " - " << quantity << endl;
            delete pstmt;
        }
        catch (sql::SQLException& e)
        {
            cerr << "Error inserting data: " << name << endl;
            cerr << "SQLException: " << e.what() << endl;
        }
    }

    // Display all data from a specified table
    void showData(const string& table)
    {
        if (!con)
        {
            cerr << "No database connection available!" << endl;
            return;
        }

        try
        {
            string query = "SELECT * FROM " + table;
            sql::Statement* stmt = con->createStatement();
            sql::ResultSet* res = stmt->executeQuery(query);

            if (!res->next())
            {
                cout << "No data found in table: " << table << "." << endl;
            }
            else
            {
                sql::ResultSetMetaData* metadata = res->getMetaData();
                int columnCount = metadata->getColumnCount();

                // Print column headers
                for (int i = 1; i <= columnCount; i++)
                {
                    cout << metadata->getColumnName(i) << "\t";
                }
                cout << endl;

                // Print rows
                do
                {
                    for (int i = 1; i <= columnCount; i++)
                    {
                        cout << res->getString(i) << "\t";
                    }
                    cout << endl;
                } while (res->next());
            }

            delete res;
            delete stmt;
        }
        catch (sql::SQLException& e)
        {
            cerr << "Error showing data: " << e.what() << endl;
        }
    }

    // Set up the database and populate initial data
    void setupDatabase()
    {
        executeQuery("DROP TABLE IF EXISTS inventory");
        cout << "Dropped table (if existed)." << endl;

        executeQuery("CREATE TABLE inventory (id INT AUTO_INCREMENT PRIMARY KEY, name VARCHAR(50), quantity INT);");
        cout << "Created table 'inventory'." << endl;

        insertData("banana", 150);
        insertData("orange", 154);
        insertData("apple", 100);
    }

    bool login(const string& userId, const string& password) {
        if (!con) {
            cerr << "No database connection available!" << endl;
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
                cout << "Login successful!" << endl;
            }
            else {
                cout << "Invalid Id or password. Please try again..." << endl;
                _getch();
            }

            delete res;
            delete pstmt;
            return isAuthenticated;

        }
        catch (sql::SQLException& e) {
            cerr << "Error during login: " << e.what() << endl;
            return false;
        }
    }
};
