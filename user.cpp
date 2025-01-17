#include <iostream>
#include <conio.h>
#include <thread>
#include <chrono>
#include <string>
#include <sstream>
#include "user.h"
#include <tabulate/table.hpp>
#include "dbConnection.h"
#include "table.h"
#include "time.h"
#include "tc.h"
#include "validation.h"

User::User(dbConnection* connection) : db(connection) {}

User::User(const std::string& userID, const std::string& name, const std::string& ic, const std::string& phoneNum, const std::string& email, const std::string& address, const std::string& password, const std::string& role)
{
    this->userID = userID;
    this->name = name;
    this->ic = ic;
    this->phoneNum = phoneNum;
    this->email = email;
    this->address = address;
    this->password = password;
    this->role = role;
}

User::~User() = default;

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

dbConnection* User::getDB()
{
    return db;
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

bool User::userVerify()
{
    while (true)
    {
        system("cls");
        std::string id;
        std::cout << CYAN << "User Login" << RESET << std::endl;
        std::cout << "Enter id: ";
        std::cin >> id;
        std::cout << "Enter password: ";
        std::string password = hiddenInput();

        if (isUser(id, password))
        {
            std::cout << GREEN << "\n\nLogin successful!" << RESET << std::endl;
            std::this_thread::sleep_for(std::chrono::milliseconds(500));
            system("cls");
            retrieveUserFromDB(id);
            std::cout << "Welcome, " <<  CYAN << getName() << RESET << "!" << std::endl;
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

    if (!db->getConnection()) {  // Make sure db connection is available
        std::cerr << "No database connection available!" << std::endl;
        return false;
    }

    try {
        // Use a prepared statement to prevent SQL injection
        std::string query = "SELECT * FROM User WHERE BINARY userID = ? AND BINARY password = ?";

        // Get the connection from dbConnection
        sql::PreparedStatement* pstmt = db->getConnection()->prepareStatement(query);

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

void User::editProfile()
{
    try
    {
        bool quitEditProfile = false;
        do
        {
            system("cls");
            // User user = retrieveUserFromDB(getUserID());
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
                    db->executeQuery(query);
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
                    db->executeQuery(query);
                }
                else
                {
                    std::cout << RED << "\nFailed to save changes." << RESET << std::endl;
                    std::cout << "\nReturning Back..." << std::endl;
                    std::this_thread::sleep_for(std::chrono::seconds(1));
                }
                db->executeQuery(query);
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
                    db->executeQuery(query);
                }
                else
                {
                    std::cout << RED << "\nFailed to save changes." << RESET << std::endl;
                    std::cout << "\nReturning Back..." << std::endl;
                    std::this_thread::sleep_for(std::chrono::seconds(1));
                }
                db->executeQuery(query);
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
                    db->executeQuery(query);
                }
                else
                {
                    std::cout << RED << "\nFailed to save changes." << RESET << std::endl;
                    std::cout << "\nReturning Back..." << std::endl;
                    std::this_thread::sleep_for(std::chrono::seconds(1));
                }
                db->executeQuery(query);
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
                    db->executeQuery(query);
                }
                else
                {
                    std::cout << RED << "\nFailed to save changes." << RESET << std::endl;
                    std::cout << "\nReturning Back..." << std::endl;
                    std::this_thread::sleep_for(std::chrono::seconds(1));
                }
                db->executeQuery(query);
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

void User::userProfile(User& user)
{
    bool quitProfile = false;
    int selectedOption = 0;  // Default selection starts from the first option

    do
    {
        system("cls");
        std::cout << CYAN << "Welcome, " << YELLOW << user.getName() << CYAN << "!\n" << RESET << std::endl;
        tabulate::Table table;

        // Set the header for the table
        table.add_row({ "Label", "Info" });

        // Add rows with the label and the corresponding value
        table.add_row({ "Name:", user.getName() + " (" + user.getRole() + ")" });
        table.add_row({ "IC Number:", user.getIc() });
        table.add_row({ "Phone Number:", user.getPhoneNum() });
        table.add_row({ "Email:", user.getEmail() });
        table.add_row({ "Home Address:", user.getAddress() });

        pivotTableFormat(table);

        // Settings Menu with arrow key navigation
        std::cout << "\nSettings:\n";
        std::cout << (selectedOption == 0 ? "-> " : "   ") << (selectedOption == 0 ? BG_YELLOW : "") << "Edit Profile" << RESET << std::endl;
        std::cout << (selectedOption == 1 ? "-> " : "   ") << (selectedOption == 1 ? BG_YELLOW : "") << "Change Password" << RESET << std::endl;
        std::cout << (selectedOption == 2 ? "-> " : "   ") << (selectedOption == 2 ? BG_RED : "") << "Return Back" << RESET << std::endl;

        std::cout << "\nUse arrow keys to navigate, press Enter to select, Esc to return to the main menu.\n";

        // Wait for key press
        char c = _getch();
        switch (c)
        {
        case KEY_UP:
            selectedOption = (selectedOption - 1 + 3) % 3;  // Wrap around navigation for 3 options
            break;
        case KEY_DOWN:
            selectedOption = (selectedOption + 1) % 3;  // Wrap around navigation for 3 options
            break;
        case KEY_ENTER:
            switch (selectedOption)
            {
            case 0:
                editProfile();  // Call the editProfile method
                break;
            case 1:
                changePassword(user);  // Call the changePassword method
                break;
            case 2:
                quitProfile = true;  // Exit to main menu
                break;
            }
            break;
        case KEY_ESC:
            quitProfile = true;  // Exit to main menu on Esc key
            break;
        }
    } while (!quitProfile);
}


void User::retrieveUserFromDB(const std::string& userID)
{
    if (!db->getConnection()) {  // Make sure db connection is available
        std::cerr << "No database connection available!" << std::endl;
        std::this_thread::sleep_for(std::chrono::milliseconds(500));
        return;
    }
    try
    {
        std::string query = "SELECT * FROM User WHERE userID = ?";

        // Get the connection from dbConnection
        sql::PreparedStatement* pstmt = db->getConnection()->prepareStatement(query);
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
            setPassword(res->getString("password"));
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

void User::changePassword(User& user)
{
    if (!db->getConnection()) {  // Make sure db connection is available
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
        bool selecting = true;
        int selected = 0; // Start at the "current password" field

        // Use arrow keys to navigate between password fields
        do
        {
            system("cls");
            std::cout << "Change Password:\n\n";
            std::cout << (selected == 0 ? "-> " : "   ") << (selected == 0 ? BG_YELLOW : "") << "Current Password: " << RESET << (oldPassword.empty() ? "" : "[Hidden]") << std::endl;
            std::cout << (selected == 1 ? "-> " : "   ") << (selected == 1 ? BG_YELLOW : "") << "New Password: " << RESET << (newPassword.empty() ? "" : "[Hidden]") << std::endl;
            std::cout << (selected == 2 ? "-> " : "   ") << (selected == 2 ? BG_YELLOW : "") << "Confirm New Password: " << RESET << (confirmPassword.empty() ? "" : "[Hidden]") << std::endl;
            std::cout << (selected == 3 ? "-> " : "   ") << (selected == 3 ? BG_GREEN : "") << "Save and Exit" << RESET << std::endl;
            std::cout << (selected == 4 ? "-> " : "   ") << (selected == 4 ? BG_RED : "") << "Cancel" << RESET << std::endl;

            std::cout << "\nUse arrow keys to navigate, press Enter to select, or press Esc to cancel.\n";

            char c = _getch(); // Use _getch() to get key press without waiting for Enter.

            switch (c) {
            case KEY_UP:
                selected = (selected - 1 + 5) % 5; // Wrap around to the last option if at the top.
                break;
            case KEY_DOWN:
                selected = (selected + 1) % 5; // Wrap around to the first option if at the bottom.
                break;
            case KEY_ENTER:
                switch (selected) {
                case 0:
                    std::cout << "\x1b[3;22H";
                    oldPassword = hiddenInput(); // Input current password

                    // Check if the entered old password matches the actual password in the database
                    if (oldPassword != user.getPassword()) {
                        std::cout << RED << "Current password is incorrect. Please try again." << RESET << std::endl;
                        std::this_thread::sleep_for(std::chrono::seconds(2));
                        oldPassword.clear(); // Clear the entered password to prompt the user to re-enter
                        break;
                    }
                    break;
                case 1:
                    std::cout << "\x1b[4;18H";
                    newPassword = hiddenInput(); // Input new password
                    break;
                case 2:
                    std::cout << "\x1b[5;26H";
                    confirmPassword = hiddenInput(); // Input confirm new password
                    if (newPassword != confirmPassword) {
                        std::cout << RED << "Passwords do not match. Please try again." << RESET << std::endl;
                        std::this_thread::sleep_for(std::chrono::seconds(2));
                        confirmPassword.clear(); // Clear the confirmation password to prompt the user to re-enter
                    }
                    break;
                case 3:
                    // Save and exit
                    if (oldPassword == user.getPassword() && newPassword == confirmPassword) {
                        std::string query = "UPDATE User SET password = ? WHERE userID = ? AND password = ?";
                        sql::PreparedStatement* pstmt = db->getConnection()->prepareStatement(query);
                        pstmt->setString(1, newPassword);
                        pstmt->setString(2, user.getUserID());
                        pstmt->setString(3, oldPassword);
                        int rowsAffected = pstmt->executeUpdate();

                        if (rowsAffected > 0)
                        {
                            std::cout << GREEN << "Password updated successfully." << RESET << std::endl;
                        }
                        else
                        {
                            std::cout << RED << "Error: Could not update password." << RESET << std::endl;
                        }
                        delete pstmt;
                    }
                    else {
                        std::cout << RED << "Error: Current password is incorrect or passwords do not match." << RESET << std::endl;
                    }
                    std::this_thread::sleep_for(std::chrono::seconds(2));
                    selecting = false;
                    break;
                case 4:
                    selecting = false;
                    break;
                }
                break;
            case KEY_ESC:
                selecting = false;  // Cancel operation
                break;
            }
        } while(selecting);
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
        std::cout << (selected == 0 ? "-> " : "   ") << (selected == 0 ? BG_YELLOW : "") << "Search User" << RESET << std::endl;
        std::cout << (selected == 1 ? "-> " : "   ") << (selected == 1 ? BG_YELLOW : "") << "Register User" << RESET << std::endl;
        std::cout << (selected == 2 ? "-> " : "   ") << (selected == 2 ? BG_YELLOW : "") << "Edit User" << RESET << std::endl;
        std::cout << (selected == 3 ? "-> " : "   ") << (selected == 3 ? BG_YELLOW : "") << "Delete User" << RESET << std::endl;

        std::cout << "\n\n\nUse arrow keys to navigate, press Enter to select, or press Esc to quit.\n";

        // Capture user input for navigation
        char c = _getch(); // Use _getch() to get key press without waiting for enter.
        switch (c) {
        case KEY_UP:
            selected = (selected - 1 + 4) % 4; // Wrap around to the last option if at the top.
            break;
        case KEY_DOWN:
            selected = (selected + 1) % 4; // Wrap around to the first option if at the bottom.
            break;
        case KEY_ENTER:
            switch (selected) 
            {
            case 0:
                searchUser();
                break;
            case 1:
                registerUser();
                break;
            case 2:
                editUser();
                break;
            case 3:
                deleteUser();
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

void User::deleteUser()
{
    int selected = 0;  // Keeps track of which option is selected.
    bool selecting = true;
    bool userSelected = false;  // Flag to track if a user is selected for deletion
    bool deleteConfirmed = false;  // Flag to track if deletion is confirmed
    std::string query = "SELECT userID AS 'User ID', "
        "name AS 'Name', "
        "ic AS 'IC', "
        "phoneNum AS 'Phone Number', "
        "email AS 'Email', "
        "address AS 'Address', "
        "role AS 'Role' "
        "FROM User LIMIT 5";

    std::string deletePassKey = "CONFIRM"; // Store name of the selected user (for passkey confirmation)
    std::string enteredPasskey;
    std::string selectedSearchCriteria = ""; // To store the search criterion (name or IC or userID)
    std::string searchData;
    do
    {
        system("cls");  // Clear screen to start fresh
        std::cout << CYAN << "Welcome to the Library System!\n" << RESET << std::endl;
        std::cout << "Search by:\n";
        std::cout << (selected == 0 ? "-> " : "   ") << (selected == 0 ? BG_YELLOW : "") << "ID: " << RESET << std::endl;
        std::cout << (selected == 1 ? "-> " : "   ") << (selected == 1 ? BG_YELLOW : "") << "Name: " << RESET << std::endl;
        std::cout << (selected == 2 ? "-> " : "   ") << (selected == 2 ? BG_YELLOW : "") << "IC: " << RESET << std::endl;

        std::cout << "\n\n\n\nUse arrow keys to navigate, press Enter to select, press R to reset searching, or press Esc to quit.\n";

        if (userSelected && !deleteConfirmed)
        {
            std::cout << GREEN << "Press Enter to confirm deletion, or Esc to cancel." << RESET << std::endl;
            char c = _getch();
            switch (c)
            {
            case KEY_ENTER:
                std::cout << GREEN << "Please enter the \"CONFIRM\" to confirm deletion: " << RESET;
                std::cin >> enteredPasskey;
                if (enteredPasskey == deletePassKey) {
                    // Construct the delete query here, after confirmation
                    std::string deleteQuery = "DELETE FROM User WHERE " + selectedSearchCriteria + "='" + searchData + "'";
                    db->executeQuery(deleteQuery);
                    std::cout << GREEN << "User deleted successfully!" << RESET << std::endl;
                }
                else {
                    std::cout << RED << "Failed to delete user!" << RESET << std::endl;
                }
                userSelected = false;  // Reset user selection
                deleteConfirmed = false; // Reset passkey confirmation
                query = "SELECT userID AS 'User ID', "
                    "name AS 'Name', "
                    "ic AS 'IC', "
                    "phoneNum AS 'Phone Number', "
                    "email AS 'Email', "
                    "address AS 'Address', "
                    "role AS 'Role' "
                    "FROM User LIMIT 5";
                break;
            case KEY_ESC:  // Cancel deletion with Esc
                std::cout << RED << "Deletion cancelled." << RESET << std::endl;
                userSelected = false;  // Reset user selection
                deleteConfirmed = false; // Reset deletion confirmation
                query = "SELECT userID AS 'User ID', "
                    "name AS 'Name', "
                    "ic AS 'IC', "
                    "phoneNum AS 'Phone Number', "
                    "email AS 'Email', "
                    "address AS 'Address', "
                    "role AS 'Role' "
                    "FROM User LIMIT 5";
                std::cout << YELLOW << "Press \"R\" to Refresh the list." << std::endl;
                break;
            }
        }

        db->fetchAndDisplayData(query);  // Fetch and display data

        char c = _getch(); // Use _getch() to get key press without waiting for enter.
        switch (c)
        {
        case KEY_UP:
            selected = (selected - 1 + 3) % 3; // Wrap around to the last option if at the top.
            break;
        case KEY_DOWN:
            selected = (selected + 1) % 3; // Wrap around to the first option if at the bottom.
            break;
        case KEY_UPPER_R:
        case KEY_LOWER_R:
           query = "SELECT userID AS 'User ID', "
                "name AS 'Name', "
                "ic AS 'IC', "
                "phoneNum AS 'Phone Number', "
                "email AS 'Email', "
                "address AS 'Address', "
                "role AS 'Role' "
                "FROM User LIMIT 5";
            break;
        case KEY_ENTER:
            switch (selected) {
            case 0:
                std::cout << "\x1b[4;8H";
                std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n'); // avoid '' data save inside and couse error
                getline(std::cin, searchData);
                query = "SELECT userID AS 'User ID', "
                    "name AS 'Name', "
                    "ic AS 'IC', "
                    "phoneNum AS 'Phone Number', "
                    "email AS 'Email', "
                    "address AS 'Address', "
                    "role AS 'Role' "
                    "FROM User WHERE userID = '" + searchData + "'";
                if (!db->recordExists(query)) {
                    std::cout << "\x1b[4;8H" << RED << "Error: User with ID '" << searchData << "' does not exist." << RESET << std::endl;
                    std::cin.ignore();
                    std::this_thread::sleep_for(std::chrono::milliseconds(2));
                    break;
                }
                selectedSearchCriteria = "userID";  // Set search criteria to userID
                userSelected = true;
                break;
            case 1:
                std::cout << "\x1b[5;10H";
                std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n'); // avoid '' data save inside and couse error
                getline(std::cin, searchData);
                query = "SELECT userID AS 'User ID', "
                    "name AS 'Name', "
                    "ic AS 'IC', "
                    "phoneNum AS 'Phone Number', "
                    "email AS 'Email', "
                    "address AS 'Address', "
                    "role AS 'Role' "
                    "FROM User WHERE name = '" + searchData + "'";
                if (!db->recordExists(query)) {
                    std::cout << "\x1b[5;10H" << RED << "Error: User with name '" << searchData << "' does not exist." << RESET << std::endl;
                    std::cin.ignore();
                    std::this_thread::sleep_for(std::chrono::milliseconds(2));
                    break;
                }
                selectedSearchCriteria = "name";  // Set search criteria to name
                userSelected = true;
                break;
            case 2:
                std::cout << "\x1b[6;8H";
                std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n'); // avoid '' data save inside and couse error
                getline(std::cin, searchData);
                query = "SELECT userID AS 'User ID', "
                    "name AS 'Name', "
                    "ic AS 'IC', "
                    "phoneNum AS 'Phone Number', "
                    "email AS 'Email', "
                    "address AS 'Address', "
                    "role AS 'Role' "
                    "FROM User WHERE ic = '" + searchData + "'";
                if (!db->recordExists(query)) {
                    std::cout << "\x1b[6;8H" << RED << "Error: User with IC '" << searchData << "' does not exist." << RESET << std::endl;
                    std::this_thread::sleep_for(std::chrono::seconds(2));
                    break;
                }
                selectedSearchCriteria = "ic";  // Set search criteria to IC
                userSelected = true;
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

    } while (selecting);  // End of loop
}


void User::searchUser()
{
    int selected = 0;  // Keeps track of which option is selected.
    bool selecting = true;
    std::string query = "SELECT userID, name, ic, phoneNum, email, address, role FROM User LIMIT 10";
    do
    {
        system("cls");
        std::string data;

        std::cout << CYAN << "Welcome to Library !\n" << RESET << std::endl;
        std::cout << "Search:" << std::endl;
        std::cout << (selected == 0 ? "-> " : "   ") << (selected == 0 ? BG_YELLOW : "") << "ID: " << RESET << std::endl;
        std::cout << (selected == 1 ? "-> " : "   ") << (selected == 1 ? BG_YELLOW : "") << "Name: " << RESET << std::endl;
        std::cout << (selected == 2 ? "-> " : "   ") << (selected == 2 ? BG_YELLOW : "") << "IC: " << RESET << std::endl;
        std::cout << (selected == 3 ? "-> " : "   ") << (selected == 3 ? BG_YELLOW : "") << "Phone Number: " << RESET << std::endl;
        std::cout << (selected == 4 ? "-> " : "   ") << (selected == 4 ? BG_YELLOW : "") << "Email: " << RESET << std::endl;
        std::cout << (selected == 5 ? "-> " : "   ") << (selected == 5 ? BG_YELLOW : "") << "Address: " << RESET << std::endl;
        std::cout << (selected == 6 ? "-> " : "   ") << (selected == 6 ? BG_YELLOW : "") << "Role: " << RESET << std::endl;

        std::cout << "\n\n\nUse arrow keys to navigate, press Enter to select, press R to reset searching, or press Esc to quit.\n";

        db->fetchAndDisplayData(query);


        char c = _getch(); // Use _getch() to get key press without waiting for enter.
        switch (c) {
        case KEY_UP:
            selected = (selected - 1 + 7) % 7; // Wrap around to the last option if at the top.
            break;
        case KEY_DOWN:
            selected = (selected + 1) % 7; // Wrap around to the first option if at the bottom.
            break;
        case KEY_UPPER_R:
        case KEY_LOWER_R:
            query = "SELECT userID, name, ic, phoneNum, email, address, role FROM User LIMIT 10"; // Reset the searching
            break;
        case KEY_ENTER:
            switch (selected) {
            case 0:
                std::cout << "\x1b[4;8H";
                getline(std::cin, data);
                query = "SELECT userID, name, ic, phoneNum, email, address, role FROM User WHERE userID LIKE '%" + data + "%'";
                break;
            case 1:
                std::cout << "\x1b[5;10H";
                getline(std::cin, data);
                query = "SELECT userID, name, ic, phoneNum, email, address, role FROM User WHERE name LIKE '%" + data + "%'";
                break;
            case 2:
                std::cout << "\x1b[6;8H";
                getline(std::cin, data);
                query = "SELECT userID, name, ic, phoneNum, email, address, role FROM User WHERE ic LIKE '%" + data + "%'";
                break;
            case 3:
                std::cout << "\x1b[7;18H";
                getline(std::cin, data);
                query = "SELECT userID, name, ic, phoneNum, email, address, role FROM User WHERE phoneNum LIKE '%" + data + "%'";
                break;
            case 4:
                std::cout << "\x1b[8;11H";
                getline(std::cin, data);
                query = "SELECT userID, name, ic, phoneNum, email, address, role FROM User WHERE email LIKE '%" + data + "%'";
                break;
            case 5:
                std::cout << "\x1b[9;13H";
                getline(std::cin, data);
                query = "SELECT userID, name, ic, phoneNum, email, address, role FROM User WHERE address LIKE '%" + data + "%'";
                break;
            case 6:
                std::cout << "\x1b[10;10H";
                getline(std::cin, data);
                query = "SELECT userID, name, ic, phoneNum, email, address, role FROM User WHERE role LIKE '%" + data + "%'";
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

void User::editUser()
{
    dbConnection* db = new dbConnection();
    int selected = 0;  // Keeps track of which menu option is selected.
    bool selecting = true;  // Controls the main loop for searching and selecting a user.
    std::string data;  // User input for search.
    std::string searchColumn = "";  // Column to search by (userID, name, or IC).
    std::string query;  // Query for database search.
    sql::PreparedStatement* pstmt = nullptr;  // Declare outside the switch.
    User user(db);  // Object to hold user data during editing.
    char prefix = 'U';
    do
    {

        system("cls");  // Clear the screen.

        std::cout << CYAN << "Edit User\n" << RESET << std::endl;
        std::cout << "Search by:\n";
        std::cout << (selected == 0 ? "-> " : "   ") << (selected == 0 ? BG_YELLOW : "") << "ID: " << RESET << std::endl;
        std::cout << (selected == 1 ? "-> " : "   ") << (selected == 1 ? BG_YELLOW : "") << "Name: " << RESET << std::endl;
        std::cout << (selected == 2 ? "-> " : "   ") << (selected == 2 ? BG_YELLOW : "") << "IC: " << RESET << std::endl;
        std::cout << "\nUse arrow keys to navigate, press Enter to select, or Esc to quit.\n";

        char c = _getch();  // Get user input.
        switch (c)
        {
        case KEY_UP:
            selected = (selected - 1 + 3) % 3;  // Navigate up in the menu.
            break;
        case KEY_DOWN:
            selected = (selected + 1) % 3;  // Navigate down in the menu.
            break;
        case KEY_ENTER:
        {
            switch (selected)
            {
            case 0:
                std::cout << "\x1b[4;8H";
                std::cin >> data;
                searchColumn = "userID";
                break;
            case 1:
                std::cout << "\x1b[5;10H";
                std::cin >> data;
                searchColumn = "name";
                break;
            case 2:
                std::cout << "\x1b[6;8H";
                std::cin >> data;
                searchColumn = "ic";
                break;
            default:
                std::cout << RED << "Invalid selection!" << RESET << std::endl;
                continue;
            }

            query = "SELECT * FROM User WHERE " + searchColumn + " = '" + data + "'";
            sql::Statement* stmt = nullptr;
            sql::ResultSet* res = nullptr;

            if (!db->recordExists(query))
            {
                std::cout << "\x1b[4;8H" << "\033[K" <<  RED << "Error: User not found!" << RESET << std::endl;
                continue;
            }

            try
            {
                stmt = db->getConnection()->createStatement();
                res = stmt->executeQuery(query);

                if (res->next())
                {
                    user.setUserID(res->getString("userID"));
                    user.setName(res->getString("name"));
                    user.setIc(res->getString("ic"));
                    user.setPhoneNum(res->getString("phoneNum"));
                    user.setEmail(res->getString("email"));
                    user.setAddress(res->getString("address"));
                    user.setRole(res->getString("role"));
                }
            }
            catch (const sql::SQLException& e)
            {
                std::cerr << "SQL error: " << e.what() << std::endl;
            }

            // Clean up resources
            delete res;
            delete stmt;

            // Edit loop.
            bool editing = true;
            while (editing)
            {

                system("cls");
                tabulate::Table table;
                std::cout << CYAN << "Edit User Details\n" << RESET << std::endl;

                table.add_row({ "User ID", user.getUserID() });
                table.add_row({ "Name", user.getName() });
                table.add_row({ "IC", user.getIc() });
                table.add_row({ "Phone Number", user.getPhoneNum() });
                table.add_row({ "Email", user.getEmail() });
                table.add_row({ "Address", user.getAddress() });
                table.add_row({ "Role", user.getRole() });
                table.add_row({ "Password", user.getPassword() });

                paraTableFormat(table);

                std::cout << "\x1b[18;1H";
                std::cout << "\x1b[K";
                std::cout << "\x1b[0J";

                std::cout << "\nSelect the field to edit:\n";
                std::cout << (selected == 0 ? "-> " : "   ") << (selected == 0 ? BG_YELLOW : "") << "Name: " << RESET << std::endl;
                std::cout << (selected == 1 ? "-> " : "   ") << (selected == 1 ? BG_YELLOW : "") << "Phone Number: " << RESET << std::endl;
                std::cout << (selected == 2 ? "-> " : "   ") << (selected == 2 ? BG_YELLOW : "") << "Email: " << RESET << std::endl;
                std::cout << (selected == 3 ? "-> " : "   ") << (selected == 3 ? BG_YELLOW : "") << "Address: " << RESET << std::endl;
                std::cout << (selected == 4 ? "-> " : "   ") << (selected == 4 ? BG_YELLOW : "") << "Password: " << RESET << std::endl;
                std::cout << (selected == 5 ? "-> " : "   ") << (selected == 5? BG_GREEN : "") << "Save and Exit" << RESET << std::endl;

                char fieldInput = _getch();
                switch (fieldInput)
                {
                case KEY_UP:
                    selected = (selected - 1 + 6) % 6;
                    break;
                case KEY_DOWN:
                    selected = (selected + 1) % 6;
                    break;
                case KEY_ENTER:
                    switch (selected)
                    {
                    case 0:
                        std::cout << "\x1b[20;10H";
                        getline(std::cin, data);
                        user.setName(data);
                        break;
                    case 1:
                        std::cout << "\x1b[22;18H";
                        getline(std::cin, data);
                        user.setPhoneNum(data);
                        break;
                    case 2:
                        std::cout << "\x1b[23;11H";
                        getline(std::cin, data);
                        if (checkEmail(data))
                        {
                            user.setEmail(data);

                        }
                        else
                        {
                            std::cerr << "\x1b[23;11H" << RED << "Invalid email format. Please try again." << RESET << std::endl;
                        }
                        break;
                    case 3:
                        std::cout << "\x1b[24;13H";
                        getline(std::cin, data);
                        user.setAddress(data);
                        break;
                    case 4:
                        std::cout << "\x1b[25;14H";
                        getline(std::cin, data);
                        user.setPassword(data);
                        break;
                    case 5:
                        query = "UPDATE User SET "
                            "name = '" + user.getName() + "', "
                            "ic = '" + user.getIc() + "', "
                            "phoneNum = '" + user.getPhoneNum() + "', "
                            "email = '" + user.getEmail() + "', "
                            "address = '" + user.getAddress() + "' "
                            "WHERE userID = '" + user.getUserID() + "'";
                        db->executeQuery(query);
                        std::cout << GREEN << "User " << user.getName() << "updated successfully!" << RESET << std::endl;
                        system("pause");
                        editing = false;
                        break;
                    }
                    break;
                case KEY_ESC:
                    editing = false;
                    break;
                }
            }
            selecting = false;  // Exit main loop after editing.
            break;
        }
        case KEY_ESC:
            selecting = false;
            break;
        }
    } while (selecting);
}

void User::registerUser()
{
    try
    {
        dbConnection* db = new dbConnection();
        User newUser(db);
        int selected = 0;  // Keeps track of which option is selected.
        bool selecting = true;
        std::string checkQuery;
        std::string query;
        std::string password;
        std::string confirmPassword;
        sql::PreparedStatement* pstmt = nullptr;  // Declare outside the switch.
        sql::ResultSet* res = nullptr;  // Declare outside the switch.

        do
        {
            system("cls");
            std::string data;
            char prefix;
            int roleChoice;
            std::cout << CYAN << "Welcome to Library !\n" << RESET << std::endl;

            tabulate::Table table;
            table.add_row({ "Name", newUser.getName() });
            table.add_row({ "IC", newUser.getIc() });
            table.add_row({ "Phone Number", newUser.getPhoneNum() });
            table.add_row({ "Email", newUser.getEmail() });
            table.add_row({ "Address", newUser.getAddress() });
            table.add_row({ "Role", newUser.getRole() });
            table.add_row({ "Password", newUser.getPassword().empty() ? "" : "[Hidden]" });

            paraTableFormat(table);

            std::cout << "Insert Data:" << std::endl;
            std::cout << (selected == 0 ? "-> " : "   ") << (selected == 0 ? BG_YELLOW : "") << "Name: " << RESET << std::endl;
            std::cout << (selected == 1 ? "-> " : "   ") << (selected == 1 ? BG_YELLOW : "") << "IC: " << RESET << std::endl;
            std::cout << (selected == 2 ? "-> " : "   ") << (selected == 2 ? BG_YELLOW : "") << "Phone Number: " << RESET << std::endl;
            std::cout << (selected == 3 ? "-> " : "   ") << (selected == 3 ? BG_YELLOW : "") << "Email: " << RESET << std::endl;
            std::cout << (selected == 4 ? "-> " : "   ") << (selected == 4 ? BG_YELLOW : "") << "Address: " << RESET << std::endl;
            std::cout << (selected == 5 ? "-> " : "   ") << (selected == 5 ? BG_YELLOW : "") << "Password: " << RESET << std::endl;
            std::cout << (selected == 6 ? "-> " : "   ") << (selected == 6 ? BG_YELLOW : "") << "Confirmed Password: " << RESET << std::endl;
            std::cout << (selected == 7 ? "-> " : "   ") << (selected == 7 ? BG_YELLOW : "") << "Select Role [1 for User, 2 for Staff, 3 for Admin]: " << RESET << std::endl;
            std::cout << (selected == 8 ? "-> " : "   ") << (selected == 8 ? BG_GREEN : "") << "Save and Exit " << RESET << std::endl;

            std::cout << "\n\n\nUse arrow keys to navigate, press Enter to select, or press Esc to quit.\n";

            char c = _getch(); // Use _getch() to get key press without waiting for enter.

            switch (c) {
            case KEY_UP:
                selected = (selected - 1 + 9) % 9; // Wrap around to the last option if at the top.
                break;
            case KEY_DOWN:
                selected = (selected + 1) % 9; // Wrap around to the first option if at the bottom.
                break;
            case KEY_ENTER:
                switch (selected) {
                case 0:
                    std::cout << "\x1b[19;10H";
                    std::getline(std::cin, data);
                    newUser.setName(data);
                    break;
                case 1:
                    std::cout << "\x1b[20;8H";
                    std::getline(std::cin, data);
                    if (!checkICFormat(data))
                    {
                        std::cerr << "\x1b[20;8H" << RED << "Invalid IC format. Please try again." << RESET << std::endl;
                    }
                        
                    if (!newUser.isICUnique(data))
                    {
                        std::cerr << "This IC is already registered. Please enter a different IC." << std::endl;
                        continue;
                    }
                    newUser.setIc(data);
                    break;
                case 2:
                    std::cout << "\x1b[21;18H";
                    getline(std::cin, data);
                    newUser.setPhoneNum(data);
                    break;
                case 3:
                    std::cout << "\x1b[22;11H";
                    std::getline(std::cin, data);
                    if (checkEmail(data))
                    {
                        newUser.setEmail(data);

                    }
                    else
                    {
                        std::cerr << "\x1b[21;18H" << RED << "Invalid email format. Please try again." << RESET << std::endl;
                    }
                    break;
                case 4:
                    std::cout << "\x1b[23;13H";
                    getline(std::cin, data);
                    newUser.setAddress(data);
                    break;
                case 5:
                    std::cout << "\x1b[24;14H";
                    data = hiddenInput();
                    password = data;  // Set the password
                    break;
                case 6:
                    std::cout << "\x1b[25;24H";
                    confirmPassword = hiddenInput();
                    if (password != confirmPassword) {
                        std::cout << RED << "Passwords do not match. Please try again." << RESET << std::endl;
                        std::this_thread::sleep_for(std::chrono::seconds(2));
                        std::cout << "\x1b[0J";  // Clear the console
                    }
                    else {
                        newUser.setPassword(password);
                    }
                    break;
                case 7:
                    std::cout << "\x1b[26;55H";
                    std::cout << "";
                    std::cin >> roleChoice;

                    switch (roleChoice)
                    {
                    case 1:
                        data = "User";
                        break;
                    case 2:
                        data = "Staff";
                        break;
                    case 3:
                        data = "Admin";
                        break;
                    default:
                        std::cout << RED << "Invalid choice. Please select 1, 2, or 3." << RESET << std::endl;
                        std::this_thread::sleep_for(std::chrono::seconds(3));
                        std::cout << "\x1b[8;1H";
                        std::cout << "\x1b[K";
                        std::cout << "\x1b[0J";
                        break;
                    }
                    newUser.setRole(data);
                    break;
                case 8:
                    prefix = (newUser.getRole() == "Admin") ? 'A' : (newUser.getRole() == "Staff") ? 'S' : 'U';
                    query = "SELECT userID FROM User WHERE userID LIKE ? ORDER BY userID DESC LIMIT 1";
                    pstmt = db->getConnection()->prepareStatement(query);

                    // Set the prefix for the query
                    pstmt->setString(1, std::string(1, prefix) + "%");
                    res = pstmt->executeQuery();

                    if (res->next())
                    {
                        // Get the last userID and increment the numeric part
                        std::string lastUserID = res->getString("userID");
                        int lastIDNumber = std::stoi(lastUserID.substr(1)); // Remove prefix and convert to integer
                        int newIDNumber = lastIDNumber + 1;

                        // Format the new userID with leading zeros
                        std::ostringstream oss;
                        oss << prefix << std::setw(8) << std::setfill('0') << newIDNumber;
                        std::string id = oss.str();
                        newUser.setUserID(id);
                    }
                    else
                    {
                        // Start with the first userID if none exist
                        std::string id = std::string(1, prefix) + "00000001";
                        newUser.setUserID(id);
                    }

                    // Insert the new user into the database
                    query = "INSERT INTO User (userID, name, ic, email, phoneNum, address, password, role) VALUES (?, ?, ?, ?, ?, ?, ?, ?)";
                    pstmt = db->getConnection()->prepareStatement(query);

                    pstmt->setString(1, newUser.getUserID());
                    pstmt->setString(2, newUser.getName());
                    pstmt->setString(3, newUser.getIc());
                    pstmt->setString(4, newUser.getEmail());
                    pstmt->setString(5, newUser.getPhoneNum());
                    pstmt->setString(6, newUser.getAddress());
                    pstmt->setString(7, newUser.getPassword());
                    pstmt->setString(8, newUser.getRole());

                    pstmt->executeUpdate();
                    std::cout << GREEN << "User registered successfully with userID: " << RESET << newUser.getUserID() << std::endl;
                    std::this_thread::sleep_for(std::chrono::seconds(3));
                    selecting = false;
                    break;
                default:
                    std::cout << "\nInvalid Input, please try again..." << std::endl;
                    break;
                }
                break;
            case KEY_ESC:
                selecting = false;  // Exit the loop when ESC is pressed.
                break;
            }
        } while (selecting);
    }catch (const std::exception& e)
    {
        std::cerr << "Error: " << e.what() << std::endl;
        system("pause");
    }
    catch (sql::SQLException& e) {
        std::cerr << "Error executing insert query: " << e.what() << std::endl;
        system("pause");
    }
}


bool User::isICUnique(const std::string& ic)
{
    std::string query = "SELECT COUNT(*) FROM Users WHERE IC = '" + ic + "';";
    int count = db->fetchSingleResult(query);
    return count == 0;
}