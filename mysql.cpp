#include <stdlib.h>
#include <iostream>
#include "mysql_connection.h"
#include <cppconn/driver.h>
#include <cppconn/exception.h>
#include <cppconn/prepared_statement.h>

using namespace std;

// for demonstration only. never save your password in the code!
const string server = "tcp://127.0.0.1:3306";
const string username = "root";
const string password = "030903";

// Function to set up and demonstrate SQL operations
void setupDatabase()
{
    sql::Driver* driver;
    sql::Connection* con;
    sql::Statement* stmt;
    sql::PreparedStatement* pstmt;

    try
    {
        driver = get_driver_instance();
        con = driver->connect(server, username, password);

        cout << "Connected to server successfully" << endl;
    }
    catch (sql::SQLException e)
    {
        cout << "Could not connect to server. Error message: " << e.what() << endl;
        exit(1);
    }

    // Select the database
    con->setSchema("library");

    stmt = con->createStatement();
    stmt->execute("DROP TABLE IF EXISTS inventory");
    cout << "Finished dropping table (if existed)" << endl;
    stmt->execute("CREATE TABLE inventory (id serial PRIMARY KEY, name VARCHAR(50), quantity INTEGER);");
    cout << "Finished creating table" << endl;
    delete stmt;

    pstmt = con->prepareStatement("INSERT INTO inventory(name, quantity) VALUES(?,?)");
    pstmt->setString(1, "banana");
    pstmt->setInt(2, 150);
    pstmt->execute();
    cout << "One row inserted." << endl;

    pstmt->setString(1, "orange");
    pstmt->setInt(2, 154);
    pstmt->execute();
    cout << "One row inserted." << endl;

    pstmt->setString(1, "apple");
    pstmt->setInt(2, 100);
    pstmt->execute();
    cout << "One row inserted." << endl;

    delete pstmt;
    delete con;
}
