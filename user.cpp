#include <iostream>
#include <conio.h>
#include <thread>
#include <chrono>
#include "user.h"
#include "dbConnection.h"

User::User() : con(nullptr) 
{

}

User::~User()
{
    if (con)
    {
        delete con;
        con = nullptr;
    }
}

std::string User::getUserID()
{
    return userID;
}

std::string User::getName()
{
    return name;
}

std::string User::getIc()
{
    return ic;
}

std::string User::getPhoneNum()
{
    return phoneNum;
}

std::string User::getEmail()
{
    return email;
}

std::string User::getAddress()
{
    return address;
}

std::string User::getPassword()
{
    return password;
}

std::string User::getRole()
{
    return role;
}

void User::setUserID(std::string userID)
{
    this->userID = userID;
}
void User::setName(std::string name)
{
    this->name = name;
}
void User::setIc(std::string ic)
{
    this->ic = ic;
}
void User::setPhoneNum(std::string phoneNum)
{
    this->phoneNum = phoneNum;
}
void User::setEmail(std::string email)
{
    this->email = email;
}
void User::setAddress(std::string address)
{
    this->address = email;
}
void User::setPassword(std::string password)
{
    this->password = password;
}
void User::setRole(std::string role)
{
    this->password = password;
}

bool User::userVerify(dbConnection db, std::string& userID, std::string& password)
{
    while (true)
    {
        system("cls");
        std::cout << "/*User Login*/" << std::endl;
        std::cout << "Enter id: ";
        std::cin >> userID;
        std::cout << "Enter password: ";
        std::cin >> password;

        if (isUser(userID, password))
        {
            system("cls");
            retrieveUserFromDB(userID);
            std::cout << "Welcome, " << name << "!" << std::endl;
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
bool User::addUser(const std::string& name, const std::string& ic, const std::string& phoneNum, const std::string& email, const std::string& address, const std::string& password, const std::string& role)
{
    if (!db.getConnection()) {  // Make sure db connection is available
        std::cerr << "No database connection available!" << std::endl;
        return false;
    }
    try
    {
        
        std::string query = "INSERT INTO User (name, ic, phoneNum, email, address, password, role) "
            "VALUES (?, ?, ?, ?, ?, ?, ?)";

        // Get the connection from dbConnection and prepare the statement
        sql::PreparedStatement* pstmt = db.getConnection()->prepareStatement(query);

        // Bind parameters to the prepared statement
        pstmt->setString(1, name);          // Bind name
        pstmt->setString(2, ic);            // Bind IC
        pstmt->setString(3, phoneNum);      // Bind phone number
        pstmt->setString(4, email);         // Bind email
        pstmt->setString(5, address);       // Bind address
        pstmt->setString(6, password);      // Bind password
        pstmt->setString(7, role);          // Bind role

        // Execute the insert query
        pstmt->executeUpdate();

        // Clean up
        delete pstmt;

        std::cout << "User added successfully!" << std::endl;
        return true;
    }
    catch (sql::SQLException& e) {
        std::cerr << "Error during login: " << e.what() << std::endl;
        return false;
    }
}

void User::userProfile()
{
    bool quitProfile = false;
    do 
    {
        system("cls");
        int userChoice = 0;

        const int labelWidth = 20;   // Width for labels
        const int valueWidth = 40;   // Width for values
        // Print each row of the table
        std::cout << "+--------------------------------------------------------------+" << std::endl;
        std::cout << "| " << std::left << std::setw(labelWidth) << "Name:"
            << std::setw(valueWidth) << (getName() + " (" + getRole() + ")") << " |" << std::endl;
        std::cout << "| " << std::left << std::setw(labelWidth) << "IC Number:"
            << std::setw(valueWidth) << getIc() << " |" << std::endl;
        std::cout << "| " << std::left << std::setw(labelWidth) << "Phone Number:"
            << std::setw(valueWidth) << getPhoneNum() << " |" << std::endl;
        std::cout << "| " << std::left << std::setw(labelWidth) << "Email:"
            << std::setw(valueWidth) << getEmail() << " |" << std::endl;
        std::cout << "| " << std::left << std::setw(labelWidth) << "Home Address:"
            << std::setw(valueWidth) << getAddress() << " |" << std::endl;

        // Print table footer
        std::cout << "+--------------------------------------------------------------+" << std::endl;

        std::cout << "Settings:"
            << "\n1. Edit Profile"
            << "\n2. Change Password"
            << "\n0. Return Back" << std::endl;

        std::cout << "Go To:";
        std::cin >> userChoice;


        switch (userChoice)
        {
        case 1:
            //EditProfile;
            break;
        case 2:
            //ChangePassword();
        case 0:
            quitProfile = true;
            break;
        default:
            std::cout << "\nInvalid Input, please try again..." << std::endl;
            std::this_thread::sleep_for(std::chrono::milliseconds(500));
            system("cls");
            break;
        }
    }while (!quitProfile);

}

void User::retrieveUserFromDB(const std::string& userID)
{
    if (!db.getConnection()) {  // Make sure db connection is available
        std::cerr << "No database connection available!" << std::endl;
        return;
    }
    try
    {
        std::string query = "SELECT * FROM User WHERE userID = ?";

        // Get the connection from dbConnection
        sql::PreparedStatement* pstmt = db.getConnection()->prepareStatement(query);
        pstmt->setString(1, userID);          // Bind name
        
        sql::ResultSet* res = pstmt->executeQuery();

        if (res->next()) 
        {
            // Fetch data from the result set
            this->name = res->getString("name");
            this->ic = res->getString("ic");
            this->phoneNum = res->getString("phoneNum");
            this->email = res->getString("email");
            this->address = res->getString("address");
            this->role = res->getString("role");
        }\
        else {
            std::cerr << "No user found with the provided userID." << std::endl;
        }

        // Clean up
        delete res;
        delete pstmt;
    }
    catch (sql::SQLException& e)
    {
        std::cerr << "Error executing query: " << e.what() << std::endl;
    }
}