#include "Library.h"
#include "dbConnection.h"
#include "validation.h"
#include <iostream>
#include <string>
#include <limits>
#include <thread>
#include <chrono>
#include <conio.h>
#include "tc.h"
// Constructor that takes a dbConnection pointer
Library::Library(dbConnection* db) {
    this->db = db; // Assuming you have a member variable db  
}


void Library::borrowBookMenu()
{
    std::string userID, bookID;
    bool isBorrowing = true;

    while (isBorrowing) // Keep the menu active until the user decides to exit  
    {
        system("cls");
        std::cout << CYAN << "Borrowing Module" << RESET << std::endl;
        std::cout << "Insert User ID:\n";

        // Input User ID  
        std::cout << "User ID: ";
        std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n'); // Clear the input buffer  
        std::getline(std::cin, userID);

        // Check if the user wants to exit  
        if (userID.empty() && _kbhit() && _getch() == KEY_ESC) {
            isBorrowing = false; // Exit if Esc is pressed  
            continue;
        }

        // Validate User ID  
        std::string checkUserQuery = "SELECT * FROM User WHERE userID='" + userID + "'";
        if (!db->recordExists(checkUserQuery))
        {
            std::cout << RED << "Error: User not found!" << RESET << std::endl;
            std::this_thread::sleep_for(std::chrono::seconds(2));
            continue; // Allow re-entry of User ID  
        }

        // Fetch user role  
        std::string roleQuery = "SELECT role FROM User WHERE userID='" + userID + "'";
        std::vector<std::map<std::string, std::string>> roleResult = db->fetchResults(roleQuery);
        if (roleResult.empty()) {
            std::cout << RED << "Error: Unable to fetch user role!" << RESET << std::endl;
            std::this_thread::sleep_for(std::chrono::seconds(2));
            continue;
        }
        std::string userRole = roleResult[0]["role"];

        // Fetch max borrowable books for the user's role  
        std::string limitQuery = "SELECT maxBorrowable FROM BorrowingLimits WHERE role='" + userRole + "'";
        std::vector<std::map<std::string, std::string>> limitResult = db->fetchResults(limitQuery);
        if (limitResult.empty()) {
            std::cout << RED << "Error: Unable to fetch borrowing limit for role!" << RESET << std::endl;
            std::this_thread::sleep_for(std::chrono::seconds(2));
            continue;
        }
        int maxBorrowable = std::stoi(limitResult[0]["maxBorrowable"]);

        // Count currently borrowed books  
        std::string countBorrowedQuery = "SELECT COUNT(*) AS borrowedCount FROM Loan WHERE userID='" + userID + "'";
        std::vector<std::map<std::string, std::string>> countResult = db->fetchResults(countBorrowedQuery);
        int borrowedCount = countResult.empty() ? 0 : std::stoi(countResult[0]["borrowedCount"]);

        // Check if the user has reached the borrowing limit  
        if (borrowedCount >= maxBorrowable) {
            std::cout << RED << "Error: You have reached the maximum borrowing limit of " << maxBorrowable << " books!" << RESET << std::endl;
            std::this_thread::sleep_for(std::chrono::seconds(2));
            continue; // Allow re-entry of User ID  
        }

        // Display user details  
        std::string displayUserQuery = "SELECT "
            "userID AS 'User ID', "
            "name AS 'Full Name', "
            "ic AS 'IC', "
            "phoneNum AS 'Phone Number', "
            "email AS 'Email', "
            "address AS 'Address', "
            "role AS 'Role' "
            "FROM User WHERE userID='" + userID + "'";
        db->fetchAndDisplayData(displayUserQuery);

        // Input Book ID  
        std::cout << "\nInsert Book ID (or press Esc to exit): ";
        std::getline(std::cin, bookID);

        // Check if the user wants to exit  
        if (bookID.empty() && _kbhit() && _getch() == KEY_ESC) {
            isBorrowing = false; // Exit if Esc is pressed  
            continue;
        }

        // Validate Book ID  
        std::string checkBookQuery = "SELECT * FROM Book WHERE bookID='" + bookID + "'";
        if (!db->recordExists(checkBookQuery))
        {
            std::cout << RED << "Error: Book not found!" << RESET << std::endl;
            std::this_thread::sleep_for(std::chrono::seconds(2));
            continue; // Allow re-entry of Book ID  
        }

        // Check book status and quantity  
        std::string checkAvailabilityQuery = "SELECT status, quantity FROM Book WHERE bookID='" + bookID + "'";

        // Use the fetchResults method to get the results  
        std::vector<std::map<std::string, std::string>> result = db->fetchResults(checkAvailabilityQuery);

        if (result.empty()) {
            std::cout << RED << "Error: Book not found!" << RESET << std::endl;
            std::this_thread::sleep_for(std::chrono::seconds(2));
            continue; // Allow re-entry of Book ID  
        }

        std::string status = result[0]["status"]; // Assuming result is a map or similar structure  
        int quantity = std::stoi(result[0]["quantity"]); // Convert quantity to int  

        if (status == "Unavailable" || quantity <= 0) {
            std::cout << RED << "Error: Book is not available for borrowing!" << RESET << std::endl;
            std::this_thread::sleep_for(std::chrono::seconds(2));
            continue; // Allow re-entry of Book ID  
        }

        // Generate loan ID and proceed to borrow the book  
        std::string loanID = generateLoanID(db->getConnection());
        std::string currentTime = generateDateTime();
        std::string dueTime = generateDueDate(14); // Assuming 14 days for due date  

        std::string borrowQuery = "INSERT INTO Loan (loanID, userID, bookID, borrow_date, due_date) VALUES ('" + loanID + "','" +
            userID + "', '" + bookID + "', '" + currentTime + "', '" + dueTime + "')";

        // Print the query for debugging  
        std::cout << "Executing query: " << borrowQuery << std::endl;

        // Execute the query  
        db->executeQuery(borrowQuery);

        // Update the quantity of the book  
        std::string updateQuantityQuery = "UPDATE Book SET quantity = quantity - 1 WHERE bookID='" + bookID + "'";
        db->executeQuery(updateQuantityQuery);

        std::cout << GREEN << "Book borrowed successfully!" << RESET << std::endl;
        std::this_thread::sleep_for(std::chrono::seconds(2));
    }
}

std::string Library::generateDateTime()
{
    std::time_t now = std::time(nullptr);
    struct tm tm_now;
    localtime_s(&tm_now, &now);  // Convert time to local time  

    // Format the current date and time as a string in SQL format  
    std::ostringstream datetime_oss;
    datetime_oss << std::put_time(&tm_now, "%Y-%m-%d %H:%M:%S");
    return datetime_oss.str();  // Return the formatted datetime as string  
}

std::string Library::generateDueDate(int days)
{
    std::time_t now = std::time(nullptr);
    struct tm tm_now;
    localtime_s(&tm_now, &now);  // Convert time to local time  

    // Add days to the current date  
    tm_now.tm_mday += days;  // Add the specified number of days  
    mktime(&tm_now);  // Normalize the tm structure after adding days  

    // Format the due date as a string in SQL format  
    std::ostringstream due_date_oss;
    due_date_oss << std::put_time(&tm_now, "%Y-%m-%d %H:%M:%S");
    return due_date_oss.str();  // Return the formatted due date as string  
}

std::string Library::generateLoanID(sql::Connection* conn)
{
    try {
        if (!conn) {
            throw std::runtime_error("Database connection not available");
        }

        // Query to get the last loanID from the Loan table
        std::string query = "SELECT loanID FROM Loan ORDER BY loanID DESC LIMIT 1";

        std::unique_ptr<sql::PreparedStatement> pstmt(conn->prepareStatement(query));

        if (!pstmt) {
            throw std::runtime_error("Failed to create prepared statement");
        }

        std::unique_ptr<sql::ResultSet> res(pstmt->executeQuery());

        if (!res) {
            throw std::runtime_error("Failed to execute query");
        }

        // If the last loanID exists, we extract and increment it
        if (res->next()) {
            std::string lastLoanID = res->getString("loanID");

            // Check if the loanID follows the format (e.g., L000001)
            if (lastLoanID.length() != 7 || lastLoanID[0] != 'L') {
                throw std::runtime_error("Invalid loan ID format in database");
            }

            // Extract the numeric part of the loanID and increment it
            int lastIDNumber = std::stoi(lastLoanID.substr(1));  // Extract number after 'L'
            int newIDNumber = lastIDNumber + 1;

            // Generate the new loanID with leading zeros for 6 digits
            std::ostringstream oss;
            oss << "L" << std::setw(6) << std::setfill('0') << newIDNumber;
            return oss.str();
        }

        // If no previous loanID exists, generate the first loanID
        return "L000001";  // Default first loanID if no loans are present
    }
    catch (const sql::SQLException& e) {
        throw std::runtime_error("Database error in generateLoanID: " + std::string(e.what()));
    }
    catch (const std::exception& e) {
        throw std::runtime_error("Error in generateLoanID: " + std::string(e.what()));
    }
}

void Library::settings()
{
    dbConnection* db = new dbConnection();
    bool loggedIn = false;
    int selected = 0;  // Keeps track of which option is selected.
    bool selecting = true;
    Library library(db);
    while (true) {
        do {
            system("cls");
            std::cout << CYAN << "Welcome to Library Management System! " << RESET << std::endl;
            std::cout << "\Settings: " << std::endl;
            std::cout << (selected == 0 ? "-> " : "   ") << (selected == 0 ? BG_YELLOW : "") << "Change Max Book Borrowed" << RESET << std::endl;
            std::cout << (selected == 1 ? "-> " : "   ") << (selected == 1 ? BG_YELLOW : "") << "Change Borrowing Duration" << RESET << std::endl;

            std::cout << "\n\n\nUse arrow keys to navigate, press Enter to select, or press Esc to quit.\n";

            // Capture user input for navigation
            char c = _getch(); // Use _getch() to get key press without waiting for enter.
            std::string exitpass;
            switch (c) {
            case KEY_UP:
                selected = (selected - 1 + 2) % 2; // Wrap around to the last option if at the top.
                break;
            case KEY_DOWN:
                selected = (selected + 2) % 2; // Wrap around to the first option if at the bottom.
                break;
            case KEY_ENTER:
                switch (selected) {
                case 0:
                    library.changeMaxBookBorrow();
                    break;
                case 1:
                    //library.changeBorrowingDuration();
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
}

void Library::changeMaxBookBorrow() {
    dbConnection* db = new dbConnection();
    int selected = 0;  // Keeps track of which option is selected.  
    bool selecting = true;
    std::string roleQuery = "SELECT role, MAX(maxBorrowable) AS maxBorrowable "
        "FROM BorrowingLimits "
        "GROUP BY role "
        "ORDER BY FIELD(role, 'User', 'Staff', 'Admin')";

    std::string updateQuery, data;

    while (selecting) {
        system("cls");
        std::cout << "Welcome to Library Management System! " << std::endl;

        // Fetch and display current borrowing limits  
        db->fetchAndDisplayData(roleQuery);

        std::cout << "\nSelect Role: " << std::endl;
        std::cout << (selected == 0 ? "-> " : "   ") << (selected == 0 ? BG_YELLOW : "") << "User" << RESET << std::endl;
        std::cout << (selected == 1 ? "-> " : "   ") << (selected == 1 ? BG_YELLOW : "") << "Staff" << RESET << std::endl;
        std::cout << (selected == 2 ? "-> " : "   ") << (selected == 2 ? BG_YELLOW : "") << "Admin" << RESET << std::endl;

        std::cout << "\n\n\nUse arrow keys to navigate, press Enter to select, or press Esc to quit.\n";

        // Capture user input for navigation  
        char c = _getch(); // Use _getch() to get key press without waiting for enter.  
        switch (c) {
        case KEY_UP:
            selected = (selected - 1 + 3) % 3; // Wrap around to the last option if at the top.  
            break;
        case KEY_DOWN:
            selected = (selected + 1) % 3; // Wrap around to the first option if at the bottom.  
            break;
        case KEY_ENTER:
            std::cout << YELLOW << "\n\nEnter new maximum books that can be borrowed: " << RESET << std::endl;
            std::cout << "\x1b[22;47H";
            getline(std::cin, data);

            // Validate the input  
            if (stoi(data) <= 0 || !isNumber(data)) {
                std::cout << "Error: The maximum borrowing limit must be a positive integer!" << std::endl;
                std::this_thread::sleep_for(std::chrono::seconds(2)); // Pause for 2 seconds  
                break; // Go back to the menu  
            }

            // Prepare the update query based on the selected role  
            switch (selected) {
            case 0:
                updateQuery = "UPDATE BorrowingLimits SET maxBorrowable='" + data + "' WHERE role='User'";
                break;
            case 1:
                updateQuery = "UPDATE BorrowingLimits SET maxBorrowable='" + data + "' WHERE role='Staff'";
                break;
            case 2:
                updateQuery = "UPDATE BorrowingLimits SET maxBorrowable='" + data + "' WHERE role='Admin'";
                break;
            default:
                std::cout << "Invalid Input, please try again..." << std::endl;
                break;
            }

            // Execute the update query  
            db->executeQuery(updateQuery);
            std::cout << "Successfully updated the maximum borrowable limit." << std::endl;
            std::this_thread::sleep_for(std::chrono::seconds(2)); // Pause for 2 seconds  
            break;
        case KEY_ESC:
            selecting = false; // Exit the menu  
            break;
        }

        std::this_thread::sleep_for(std::chrono::milliseconds(100)); // Small delay for better UX  
    }

    delete db; // Clean up the database connection  
}