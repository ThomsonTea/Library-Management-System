#include <iostream>
#include <conio.h>
#include <thread>
#include <chrono>
#include "user.h"

User::User() {}

User::~User() {}

bool User::userVerify(dbConnection& db, std::string& username, std::string& password)
{
    while (true)
    {
        system("cls");
        std::cout << "/*User Login*/" << std::endl;
        std::cout << "Enter id: ";
        std::cin >> username;
        std::cout << "Enter password: ";
        std::cin >> password;

        if (isUser(username, password))
        {
            system("cls");
            std::cout << "Welcome, " << username << "!" << std::endl;
            std::this_thread::sleep_for(std::chrono::seconds(2));
            return true;
        }
        else
        {
            system("cls");
            std::cout << "Login failed. Please try again." << std::endl;
            std::this_thread::sleep_for(std::chrono::seconds(1));
        }
    }
}

bool User::isUser(const std::string& userId, const std::string& password)
{
    if (!db.getConnection()) {  // Make sure db connection is available
        std::cerr << "No database connection available!" << std::endl;
        return false;
    }

    try {
        // Use a prepared statement to prevent SQL injection
        std::string query = "SELECT * FROM User WHERE userID = ? AND password = ?";

        // Get the connection from dbConnection
        sql::PreparedStatement* pstmt = db.getConnection()->prepareStatement(query);

        // Bind parameters to the prepared statement
        pstmt->setString(1, userId);
        pstmt->setString(2, password);

        // Execute the query
        sql::ResultSet* res = pstmt->executeQuery();

        // Check if the user is authenticated
        bool isAuthenticated = false;
        if (res->next()) {
            isAuthenticated = true;
            std::cout << "Login successful!" << std::endl;
        }
        else {
            std::cout << "Invalid Id or password. Please try again..." << std::endl;
            _getch();  // Optional, wait for user input
        }

        // Clean up
        delete res;
        delete pstmt;

        return isAuthenticated;

    }
    catch (sql::SQLException& e) {
        std::cerr << "Error during login: " << e.what() << std::endl;
        return false;
    }
}


