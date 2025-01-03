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
void User::registerUser()
{
    try
    {
        system("cls");
        std::string username, password, email, role, userID, name, ic, phoneNum, address;

        std::cout << "Register User" << std::endl;

        // Prompt for username
        std::cout << "Username: ";
        std::cin.ignore();
        std::getline(std::cin, username);

        // Prompt for password
        std::cout << "Password: ";
        std::getline(std::cin, password);

        // Prompt for email
        std::cout << "Email: ";
        std::getline(std::cin, email);

        // Prompt for name
        std::cout << "Name: ";
        std::getline(std::cin, name);

        // Prompt for IC
        std::cout << "IC: ";
        std::getline(std::cin, ic);

        // Prompt for phone number
        std::cout << "Phone Number: ";
        std::getline(std::cin, phoneNum);

        // Prompt for address
        std::cout << "Address: ";
        std::getline(std::cin, address);

        // Prompt for role selection with validation
        int roleChoice;
        do
        {
            std::cout << "Select Role [1 for User, 2 for Admin, 3 for Staff]: ";
            std::cin >> roleChoice;

            switch (roleChoice)
            {
            case 1:
                role = "user";
                break;
            case 2:
                role = "admin";
                break;
            case 3:
                role = "staff";
                break;
            default:
                std::cout << "Invalid choice. Please select 1, 2, or 3." << std::endl;
            }
        } while (roleChoice < 1 || roleChoice > 3);

        // Generate userID based on role
        char prefix = (role == "admin") ? 'A' : (role == "staff") ? 'S' : 'U';
        std::string query = "SELECT userID FROM User WHERE userID LIKE ? ORDER BY userID DESC LIMIT 1";
        sql::PreparedStatement* pstmt = db.getConnection()->prepareStatement(query);

        // Set the prefix for the query
        pstmt->setString(1, std::string(1, prefix) + "%");
        sql::ResultSet* res = pstmt->executeQuery();

        if (res->next())
        {
            // Get the last userID and increment the numeric part
            std::string lastUserID = res->getString("userID");
            int lastIDNumber = std::stoi(lastUserID.substr(1)); // Remove prefix and convert to integer
            int newIDNumber = lastIDNumber + 1;

            // Format the new userID with leading zeros
            std::ostringstream oss;
            oss << prefix << std::setw(8) << std::setfill('0') << newIDNumber;
            userID = oss.str();
        }
        else
        {
            // Start with the first userID if none exist
            userID = std::string(1, prefix) + "00000001";
        }

        // Clean up
        delete pstmt;
        delete res;

        // Insert the new user into the database
        query = "INSERT INTO User (userID, name, ic, email, phoneNum, address, password, role) VALUES (?, ?, ?, ?, ?, ?, ?, ?)";
        pstmt = db.getConnection()->prepareStatement(query);

        pstmt->setString(1, userID);
        pstmt->setString(2, name);
        pstmt->setString(3, ic);
        pstmt->setString(4, email);
        pstmt->setString(5, phoneNum);
        pstmt->setString(6, address);
        pstmt->setString(7, password);
        pstmt->setString(8, role);

        pstmt->executeUpdate();

        delete pstmt;

        std::cout << GREEN << "User registered successfully with userID: " << RESET << userID <<  std::endl;
        std::this_thread::sleep_for(std::chrono::seconds(1));
    }
    catch (sql::SQLException& e)
    {
        std::cerr << "Database error while registering user: " << e.what() << std::endl;
        std::this_thread::sleep_for(std::chrono::seconds(2));
    }
    catch (std::exception& e)
    {
        std::cerr << "Unexpected error: " << e.what() << std::endl;
        std::this_thread::sleep_for(std::chrono::seconds(2));
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

void User::userManagementMenu()
{
    int selected = 0;  // Keeps track of which option is selected.
    bool selecting = true;

    do {
        system("cls");
        std::cout << CYAN << "Library Management System" << RESET << std::endl;

        std::cout << "\nModule: " << std::endl;
        std::cout << (selected == 0 ? "-> " : "   ") << (selected == 0 ? BG_YELLOW : "") << "Register User" << RESET << std::endl;
        std::cout << (selected == 1 ? "-> " : "   ") << (selected == 1 ? BG_YELLOW : "") << "Edit User" << RESET << std::endl;
        std::cout << (selected == 2 ? "-> " : "   ") << (selected == 2 ? BG_YELLOW : "") << "Delete User" << RESET << std::endl;

        std::cout << "\n\n\nUse arrow keys to navigate, press Enter to select, or press Esc to quit.\n";

        // Capture user input for navigation
        char c = _getch(); // Use _getch() to get key press without waiting for enter.
        std::string exitpass;
        switch (c) {
        case KEY_UP:
            selected = (selected - 1 + 3) % 3; // Wrap around to the last option if at the top.
            break;
        case KEY_DOWN:
            selected = (selected + 1) % 3; // Wrap around to the first option if at the bottom.
            break;
        case KEY_ENTER:
            switch (selected) {
            case 0:
                //Register User
                break;
            case 1:
                //Edit User
                break;
            case 2:
                //Delete User
                break;
            default:
                std::cout << "\nInvalid Input, please try again..." << std::endl;
                break;
            }
            break;
        case KEY_ESC:
            selecting = false;
            break;
        }

        std::this_thread::sleep_for(std::chrono::milliseconds(1));
    } while (selecting);
}
