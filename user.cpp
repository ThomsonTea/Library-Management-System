#include <iostream>
#include <conio.h>
#include <thread>
#include <chrono>
#include <string>
#include <sstream>
#include "user.h"
#include "dbConnection.h"
#include "tc.h"

User::User() : con(nullptr) {}

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
    this->address = address;
}
void User::setPassword(std::string password)
{
    this->password = password;
}
void User::setRole(std::string role)
{
    this->role = role;
}

bool User::userVerify(dbConnection db)
{
    while (true)
    {
        system("cls");
        std::cout << CYAN << "User Login" << RESET << std::endl;
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
        std::string query = "SELECT * FROM User WHERE BINARY userID = ? AND BINARY password = ?";

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
void User::addUser(const std::string& name, const std::string& ic, const std::string& phoneNum, const std::string& email, const std::string& address, const std::string& password, const std::string& role)
{
    if (!db.getConnection()) {  // Make sure db connection is available
        std::cerr << "No database connection available!" << std::endl;
        return;
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
        return;
    }
    catch (sql::SQLException& e) {
        std::cerr << "Error during login: " << e.what() << std::endl;
        return;
    }
}

void User::editProfile()
{
    if (!db.getConnection()) {  // Make sure db connection is available
        std::cerr << "No database connection available!" << std::endl;
        std::this_thread::sleep_for(std::chrono::milliseconds(500));
        return;
    }
    try
    {
        bool quitEditProfile = false;
        do
        {
            system("cls");
            retrieveUserFromDB(getUserID());
            int option;
            const int labelWidth = 20;   // Width for labels
            const int valueWidth = 40;   // Width for values
            std::cout << CYAN << "Edit " << YELLOW << getName() << CYAN << "'s Profile:\n" << RESET << std::endl;
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
            std::cout << "+--------------------------------------------------------------+" << std::endl;

            std::cout << "\nUpdate:"
                << "\n1. Name "
                << "\n2. IC Number"
                << "\n3. Phone Number"
                << "\n4. Email"
                << "\n5. Home Address"
                << "\n0. Return back" << std::endl;

            std::cout << "\nGo To: ";
            std::cin >> option;

            std::cin.ignore();

            std::string query;
            std::string data;
            int confirmKey;
            switch (option)
            {
            case 1:
                std::cout << "\x1b[10;1H";
                std::cout << "\x1b[K";
                std::cout << "\x1b[0J";
                std::cout << "\nEnter New name:" << std::endl;
                std::cout << "\x1b[11;17H";
                std::cout << "\x1b[K";
                std::cout << "\x1b[0J";
                std::getline(std::cin, data);

                std::cout << "\nConfirm to change your name " << CYAN << UNDERLINE << getName() << RESET << " to " << CYAN << data << RESET << " ?"
                    << "\nPlease press \"" << GREEN << "ENTER" << RESET "\" to save the change, other keys to return:\n";
                confirmKey = _getch();
                if (confirmKey == 13)
                {
                    query = "UPDATE User SET name='" + data + "' WHERE userId='" + getUserID() + "'";
                    db.executeQuery(query);
                }
                else
                {
                    std::cout << RED << "\nFailed to save changes." << RESET << std::endl;
                    std::cout << "\nReturning Back..." << std::endl;
                    std::this_thread::sleep_for(std::chrono::seconds(1));
                }
                break;
            case 2:
                std::cout << "\x1b[10;1H";
                std::cout << "\x1b[K";
                std::cout << "\x1b[0J";
                std::cout << "\nEnter New IC Number:" << std::endl;
                std::cout << "\x1b[11;22H";
                std::cout << "\x1b[K";
                std::cout << "\x1b[0J";
                std::getline(std::cin, data);
                std::cout << "\nConfirm to change your IC Number " << CYAN << UNDERLINE << getIc() << RESET << " to " << CYAN << data << RESET << " ?"
                    << "\nPlease press \"" << GREEN << "ENTER" << RESET "\" to save the change, other keys to return:\n";
                confirmKey = _getch();
                if (confirmKey == 13)
                {
                    query = "UPDATE User SET ic='" + data + "' WHERE userId='" + getUserID() + "'";
                    db.executeQuery(query);
                }
                else
                {
                    std::cout << RED << "\nFailed to save changes." << RESET << std::endl;
                    std::cout << "\nReturning Back..." << std::endl;
                    std::this_thread::sleep_for(std::chrono::seconds(1));
                }
                db.executeQuery(query);
                break;
            case 3:
                std::cout << "\x1b[10;1H";
                std::cout << "\x1b[K";
                std::cout << "\x1b[0J";
                std::cout << "\nEnter New Phone Number:" << std::endl;
                std::cout << "\x1b[11;25H";
                std::cout << "\x1b[K";
                std::cout << "\x1b[0J";
                std::getline(std::cin, data);
                std::cout << "\nConfirm to change your phone number " << CYAN << UNDERLINE << getPhoneNum() << RESET << " to " << CYAN << data << RESET << " ?"
                    << "\nPlease press \"" << GREEN << "ENTER" << RESET "\" to save the change, other keys to return:\n";
                confirmKey = _getch();
                if (confirmKey == 13)
                {
                    query = "UPDATE User SET phoneNum='" + data + "' WHERE userId='" + getUserID() + "'";
                    db.executeQuery(query);
                }
                else
                {
                    std::cout << RED << "\nFailed to save changes." << RESET << std::endl;
                    std::cout << "\nReturning Back..." << std::endl;
                    std::this_thread::sleep_for(std::chrono::seconds(1));
                }
                db.executeQuery(query);
                break;
            case 4:
                std::cout << "\x1b[10;1H";
                std::cout << "\x1b[K";
                std::cout << "\x1b[0J";
                std::cout << "\nEnter New Email:" << std::endl;
                std::cout << "\x1b[11;18H";
                std::cout << "\x1b[K";
                std::cout << "\x1b[0J";
                std::getline(std::cin, data);
                std::cout << "\nConfirm to change your email " << CYAN << UNDERLINE << getEmail() << RESET << " to " << CYAN << data << RESET << " ?"
                    << "\nPlease press \"" << GREEN << "ENTER" << RESET "\" to save the change, other keys to return:\n";
                confirmKey = _getch();
                if (confirmKey == 13)
                {
                    query = "UPDATE User SET email='" + data + "' WHERE userId='" + getUserID() + "'";
                    db.executeQuery(query);
                }
                else
                {
                    std::cout << RED << "\nFailed to save changes." << RESET << std::endl;
                    std::cout << "\nReturning Back..." << std::endl;
                    std::this_thread::sleep_for(std::chrono::seconds(1));
                }
                db.executeQuery(query);
                break;
            case 5:
                std::cout << "\x1b[10;1H";
                std::cout << "\x1b[K";
                std::cout << "\x1b[0J";
                std::cout << "\nEnter New Home Address:" << std::endl;
                std::cout << "\x1b[11;25H";
                std::cout << "\x1b[K";
                std::cout << "\x1b[0J";
                std::getline(std::cin, data);
                std::cout << "\nConfirm to change your home address " << CYAN << UNDERLINE << getName() << RESET << " to " << CYAN << data << RESET << " ?"
                    << "\nPlease press \"" << GREEN << "ENTER" << RESET "\" to save the change, other keys to return:\n";
                confirmKey = _getch();
                if (confirmKey == 13)
                {
                    query = "UPDATE User SET address='" + data + "' WHERE userId='" + getUserID() + "'";
                    db.executeQuery(query);
                }
                else
                {
                    std::cout << RED << "\nFailed to save changes." << RESET << std::endl;
                    std::cout << "\nReturning Back..." << std::endl;
                    std::this_thread::sleep_for(std::chrono::seconds(1));
                }
                db.executeQuery(query);
                break;
            case 0:
                quitEditProfile = true;
                break;
            default:
                std::cout << "\nInvalid Input, please try again..." << std::endl;
                std::this_thread::sleep_for(std::chrono::milliseconds(500));
                system("cls");
                break;
            }
        } while (!quitEditProfile);
    }
    catch (sql::SQLException& e) {
        std::cerr << "Error during login: " << e.what() << std::endl;
        return;
    }
}

void User::userProfile()
{
    bool quitProfile = false;
    do 
    {
        system("cls");
        int option = 0;
        const int labelWidth = 20;   // Width for labels
        const int valueWidth = 40;   // Width for values
        std::cout << CYAN << "Welcome, " << YELLOW << getName() << CYAN << "!\n" << RESET << std::endl;
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

        std::cout << "\nSettings:"
            << "\n1. Edit Profile"
            << "\n2. Change Password"
            << "\n0. Return Back" << std::endl;

        std::cout << "\nGo To:";
        std::cin >> option;

        switch (option)
        {
        case 1:
            editProfile();
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
        std::this_thread::sleep_for(std::chrono::milliseconds(500));
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
            setUserID(res->getString("userID"));
            setName(res->getString("name"));
            setIc(res->getString("ic"));
            setPhoneNum(res->getString("phoneNum"));
            setEmail(res->getString("email"));
            setAddress(res->getString("address"));
            setRole(res->getString("role"));
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
        std::this_thread::sleep_for(std::chrono::milliseconds(500));
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

        if (isUser(getUserID(), oldPassword))
        {
            std::string query = "UPDATE User SET password = ? WHERE userID = ? AND password = ?";
            // Get the connection from dbConnection
            sql::PreparedStatement* pstmt = db.getConnection()->prepareStatement(query);
            pstmt->setString(1, newPassword);
            pstmt->setString(2, getUserID());
            pstmt->setString(3, oldPassword);

            int rowsAffected = pstmt->executeUpdate();

            if (rowsAffected > 0)
            {
                std::cout << GREEN << "\n\nPassword updated successfully for user with ID: " << CYAN << getUserID() << RESET << std::endl;
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
