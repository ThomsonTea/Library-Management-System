#include <iostream>
#include <conio.h>
#include <thread>
#include <chrono>
#include <sstream>
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
    return this->userID;
}

std::string User::getName()
{
    return this->name;
}

std::string User::getIc()
{
    return this->ic;
}

std::string User::getPhoneNum()
{
    return this->phoneNum;
}

std::string User::getEmail()
{
    return this->email;
}

std::string User::getAddress()
{
    return this->address;
}

std::string User::getPassword()
{
    return this->password;
}

std::string User::getRole()
{
    return this->role;
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

bool User::userVerify(dbConnection db)
{
    while (true)
    {
        system("cls");
        std::cout << "/*User Login*/" << std::endl;
        std::cout << "Enter id: ";
        std::cin >> userID;
        std::cout << "Enter password: ";
        password = hiddenInput();

        if (isUser(userID, password))
        {
            std::cout << GREEN << "\n\nLogin successful!" << RESET << std::endl;
            std::this_thread::sleep_for(std::chrono::milliseconds(500));
            system("cls");
            retrieveUserFromDB(userID);
            std::cout << "Welcome, " <<  CYAN << name << RESET << "!" << std::endl;
            std::this_thread::sleep_for(std::chrono::seconds(2));
            return true;
        }
        else
        {
            std::cout << RED << "\n\nLogin failed. Please try again." << RESET << std::endl;
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
            changePassword();
            break;
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
            this->userID = res->getString("userID");
            this->name = res->getString("name");
            this->ic = res->getString("ic");
            this->phoneNum = res->getString("phoneNum");
            this->email = res->getString("email");
            this->address = res->getString("address");
            this->role = res->getString("role");
        }
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

void User::changePassword()
{
    if (!db.getConnection()) {  // Make sure db connection is available
        std::cerr << "No database connection available!" << std::endl;
        return;
    }
    try
    {
        system("cls");
        std::string oldPassword;
        std::string newPassword;
        std::string confirmPassword;
        std::string msgEnterCurrPassword = "Enter your current password: ";
        std::cout << msgEnterCurrPassword;
        oldPassword = hiddenInput();

        while (oldPassword != getPassword())
        {
            std::cout << RED << "\t\tPassword is incorrect, pls try again" << RESET << std::endl;
            std::this_thread::sleep_for(std::chrono::seconds(1));
            std::cout << "\x1b[A"; // Move the cursor up one line
            std::cout << "\x1b[A"; // Move the cursor up one line
            std::cout << "\x1b[" << msgEnterCurrPassword.length() << "C"; // Move the cursor to the msg characters to the right
            std::cout << "\x1b[K"; // Clear from the cursor's position to the right of user input
            oldPassword = hiddenInput();
        }

        std::string msgEnterNewPassword = "\nEnter your new password: ";
        std::cout << msgEnterNewPassword;
        newPassword = hiddenInput();
        std::cout << "\nConfirm your new password: ";
        confirmPassword = hiddenInput();

        while (newPassword != confirmPassword)
        {
            std::cout << RED << "\t\tNew password and confirmed password is different, pls try again" << RESET << std::endl;
            std::this_thread::sleep_for(std::chrono::seconds(1));
            std::cout << "\x1b[2;" << msgEnterNewPassword.length() << "H";
            std::cout << "\x1b[K"; // Clear from the cursor's position to the right of user input
            std::cout << "\x1b[0J"; // Clears from "\nEnter your new password: " to the end of the screen
            newPassword = hiddenInput();
            std::cout << "\nConfirm your new password: ";
            confirmPassword = hiddenInput();
        }

        if (isUser(this->userID, oldPassword))
        {
            std::string query = "UPDATE User SET password = ? WHERE userID = ? AND password = ?";
            // Get the connection from dbConnection
            sql::PreparedStatement* pstmt = db.getConnection()->prepareStatement(query);
            pstmt->setString(1, newPassword);
            pstmt->setString(2, this->userID);
            pstmt->setString(3, oldPassword);

            int rowsAffected = pstmt->executeUpdate();

            if (rowsAffected > 0)
            {
                std::cout << GREEN << "\n\nPassword updated successfully for user with ID: " << CYAN << this->userID << RESET << std::endl;
                std::cout << "\nPress any key to continue..." << std::endl;
                _getch();
            }
            else
            {
                std::cout << RED << "\nWrong Password, pls check your password" << RESET << std::endl;
                std::this_thread::sleep_for(std::chrono::seconds(2));
            }
            // Clean up
            delete pstmt;
        }
        else
        {
            std::cout << RED << "\nWrong Password, pls check your password" << RESET << std::endl;
            std::this_thread::sleep_for(std::chrono::seconds(2));
            system("cls");
        }
        
    }
    catch (sql::SQLException& e)
    {
        std::cerr << "Error executing query: " << e.what() << std::endl;
    }
}

std::string User::hiddenInput()
{
    std::string input = "";
    char tmp = '\0';

    while (true)
    {
        tmp = _getch();
        switch (tmp)
        {
        case 13: //ASCII Value for Enter key
            return input;
            break;
        case 27: //ASCII value for ESC key
            return "\0"; //return the previous value instead to cancel
            break;
        case 8:
            if (input.length() > 0)
            {
                input.erase(input.size() - 1); // erase last iundex
                std::cout << "\b \b";
                //print  this which will move back caret and replace with space character with space and move back caret 1 more time to initiate backspace
            }
            break;
        default: // for any insert key
            if (tmp >= 32 == tmp <= 126)
            {
                input += tmp;
                // display "*" instead
                std::cout << "*";
            }
            // if the key press is outside our allowed range simply skips it to ignore
            break;
        }
    }
    std::cout << std::endl;
    return input;
}