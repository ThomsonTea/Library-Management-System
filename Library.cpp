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
#include "fine.h"
#include "time.h"
#include <tabulate/table.hpp>
#include "table.h"
// Constructor that takes a dbConnection pointer
Library::Library(dbConnection* db) 
{
    this->db = db; // Assuming you have a member variable db  
}

void Library::inputUserData()
{
    int selected = 0;  // Keeps track of which option is selected.
    bool borrowing = true;
    std::string query;
    std::string userID;
    std::string displayUserQuery;
    std::string roleQuery;
    std::string checkUserQuery;
    bool userSelected = false;
    User user(db);
    std::vector<std::map<std::string, std::string>> roleResult;
    std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n'); // Clear the input buffer
    std::string userRole;
    do
    {
        system("cls");


        std::cout << CYAN << "Welcome to Library !\n" << RESET << std::endl;
        std::cout << "Borrowing:" << std::endl;
        std::cout << (selected == 0 ? "-> " : "   ") << (selected == 0 ? BG_YELLOW : "") << "User ID: " << RESET << std::endl;
        std::cout << (selected == 1 ? "-> " : "   ") << (selected == 1 ? BG_GREEN : "") << "CONFIRM" << RESET << std::endl;
        std::cout << (selected == 2 ? "-> " : "   ") << (selected == 2 ? BG_RED : "") << "RETURN BACK" << RESET << std::endl;

        std::cout << "\n\n\nUse arrow keys to navigate, press Enter to select or press Esc to quit.\n";

        if (userSelected)
        {
            // Fetch user role  
            roleQuery = "SELECT role FROM User WHERE userID='" + user.getUserID() + "'";
            roleResult = db->fetchResults(roleQuery);
            if (roleResult.empty()) {
                std::cout << RED << "Error: Unable to fetch user role!" << RESET << std::endl;
                std::this_thread::sleep_for(std::chrono::seconds(2));
                break;
            }
            userRole = roleResult[0]["role"];

            displayUserQuery = 
                "SELECT b.bookID, b.title, b.author, l.borrow_date, l.due_date, l.bookStatus "
                "FROM Loan l "
                "JOIN Book b ON l.bookID = b.bookID "
                "WHERE l.userID = '" + user.getUserID() + "' "
                "AND l.bookStatus IN('Borrowing','Overdue') "
                "AND l.return_date IS NULL";

            db->fetchAndDisplayData(displayUserQuery);
        }

        char c = _getch(); // Use _getch() to get key press without waiting for enter.
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
                std::cout << "\x1b[4;13H";
                getline(std::cin, userID);

                user.retrieveUserFromDB(userID);
                // Validate User ID  
                checkUserQuery = "SELECT * FROM User WHERE userID='" + user.getUserID() + "'";
                if (!db->recordExists(checkUserQuery))
                {
                    std::cout << RED << "Error: User not found!" << RESET << std::endl;
                    std::this_thread::sleep_for(std::chrono::seconds(2));
                    continue; // Allow re-entry of User ID  
                }
                
                userSelected = true;
                break;
            case 1:
                ModulesMenu(user);
                break;
            case 2:
                borrowing = false;
                break;
            default:
                std::cout << "\nInvalid Input, please try again..." << std::endl;
                break;
            }
            break;
        case KEY_ESC:
            borrowing = false;
            break;
        }
    } while (borrowing);
}

void Library::borrowBook(User user)
{
    int selected = 0;  // Keeps track of which option is selected.  
    bool borrowing = true;
    std::string bookID;
    std::string checkBookQuery;
    std::string insertLoanQuery;
    std::string bookQuery;
    bool bookConfirmed = false;
    Book book(db);
    std::vector<std::map<std::string, std::string>> result;

    // Declare quantity outside the switch statement and initialize it to 0  
    int quantity = 0;

    do
    {
        system("cls");
        std::cout << "Borrowing book for User: " << user.getUserID() << std::endl;
        std::cout << (selected == 0 ? "-> " : "   ") << (selected == 0 ? BG_YELLOW : "") << "Enter Book ID: " << RESET << std::endl;
        std::cout << (selected == 1 ? "-> " : "   ") << (selected == 1 ? BG_GREEN : "") << "CONFIRM" << RESET << std::endl;
        std::cout << (selected == 2 ? "-> " : "   ") << (selected == 2 ? BG_RED : "") << "RETURN BACK" << RESET << std::endl;

        std::cout << "\n\n\nUse arrow keys to navigate, press Enter to select, or press Esc to quit.\n";

        if (bookConfirmed)
        {
            // Fetch user role  
            bookQuery = "SELECT * FROM Book WHERE bookID='" + book.getBookID() + "'";

            db->fetchAndDisplayData(bookQuery);
        }

        char c = _getch(); // Use _getch() to get key press without waiting for enter.  
        switch (c) {
        case KEY_UP:
            selected = (selected - 1 + 3) % 3; // Wrap around to the last option if at the top.  
            break;
        case KEY_DOWN:
            selected = (selected + 1) % 3; // Wrap around to the first option if at the bottom.  
            break;
        case KEY_ENTER:
            switch (selected) {
            case 0:  // Enter Book ID
                std::cout << "\x1b[2;19H"; // Move cursor to the input position
                std::getline(std::cin, bookID);

                try {
                    // Check if the user has any overdue books
                    std::string overdueCheckQuery =
                        "SELECT l.loanID, l.bookID, b.title, l.due_date "
                        "FROM Loan l "
                        "JOIN Book b ON l.bookID = b.bookID "
                        "WHERE l.userID='" + user.getUserID() + "' AND l.bookStatus = 'Overdue' AND l.return_date IS NULL";

                    auto overdueResult = db->fetchResults(overdueCheckQuery);

                    if (!overdueResult.empty()) {
                        std::cout << "\x1b[11;1H" << RED << "Error: You have overdue books and cannot borrow more!" << RESET << std::endl;
                        std::cout << "Overdue Book Details:" << std::endl;

                        tabulate::Table table;

                        table.add_row({ "Loan ID", "Book ID", "Title", "Due Date" });
                        for (const auto& overdueBook : overdueResult) {

                            table.add_row({ overdueBook.at("loanID"), overdueBook.at("bookID"), overdueBook.at("title"), overdueBook.at("due_date")});
                        }
                        tableFormat(table);
                        system("pause");
                        break; // Exit case without proceeding further
                    }

                    // Check book status and quantity
                    checkBookQuery = "SELECT quantity, title, status FROM Book WHERE bookID='" + bookID + "'";
                    result = db->fetchResults(checkBookQuery);

                    if (result.empty()) {
                        std::cout << RED << "Error: Book does not exist!" << RESET << std::endl;
                        std::this_thread::sleep_for(std::chrono::seconds(2));
                        continue; // Allow re-entry of Book ID
                    }

                    // Check status explicitly
                    if (result[0]["status"] != "Available") {
                        std::cout << RED << "Error: Book is not available!" << RESET << std::endl;
                        std::this_thread::sleep_for(std::chrono::seconds(2));
                        continue;
                    }

                    // Ensure quantity is checked correctly
                    quantity = std::stoi(result[0]["quantity"]); // Update the pre-declared quantity
                    if (quantity <= 0) {
                        std::cout << RED << "Error: Book is out of stock!" << RESET << std::endl;
                        std::this_thread::sleep_for(std::chrono::seconds(2));
                        continue; // Allow re-entry of Book ID
                    }

                    // Retrieve book details
                    book.retrieveBookFromDB(bookID);
                    bookConfirmed = true;  // Book is valid
                }
                catch (const std::exception& e) {
                    std::cout << RED << "Error processing book: " << e.what() << RESET << std::endl;
                    std::cerr << "Debug: Query was: " << checkBookQuery << std::endl;
                    std::this_thread::sleep_for(std::chrono::seconds(2));
                    break;
                }
                break;
            case 1:  // CONFIRM  
                if (bookConfirmed && !book.getBookID().empty()) {
                    try {
                        if (user.getUserID().empty()) {
                            std::cout << RED << "Error: User ID is not set!" << RESET << std::endl;
                            std::this_thread::sleep_for(std::chrono::seconds(2));
                            continue;
                        }


                        // Check user's borrowing limit  
                        std::string checkCountBorrowingQuery =
                            "SELECT COUNT(*) AS borrowed_count FROM Loan WHERE userID=? AND bookStatus='Borrowing'";

                        std::vector<std::string> countBorrowingParams = { user.getUserID() };
                        int borrowedCount = db->getInt(checkCountBorrowingQuery, countBorrowingParams);

                        std::string maxBorrowQuery = "SELECT maxBorrowable FROM rolePrivelage WHERE role = ?";
                        std::vector<std::string> maxBorrowParams = { user.getRole() };
                        int maxBorrowCount = db->getInt(maxBorrowQuery, maxBorrowParams);
                        // Assuming max borrow limit is 3 for this example  
                        if (borrowedCount >= maxBorrowCount) {
                            std::cout << RED << "Error: You have reached the maximum borrowing limit!" << RESET << std::endl;
                            std::this_thread::sleep_for(std::chrono::seconds(2));
                            continue;
                        }

                        // Generate Loan ID  
                        std::string loanID = generateLoanID(db->getConnection());

                        // Get borrow duration based on user role  
                        int duration = getBorrowDuration(user.getRole());

                        // Generate dates  
                        std::string borrowDate = getCurrentTime();
                        std::string dueDate = generateDueDate(duration);

                        // Insert into Loan table  
                        insertLoanQuery =
                            "INSERT INTO Loan (loanID, userID, bookID, borrow_date, due_date, bookStatus) VALUES ('" +
                            loanID + "', '" + user.getUserID() + "', '" + book.getBookID() + "', '" + borrowDate + "', '" + dueDate + "', 'Borrowing')";

                        db->executeQuery(insertLoanQuery);

                        // Update book quantity and status if necessary  
                        std::string updateBookQuery = "UPDATE Book SET quantity = quantity - 1 WHERE bookID='" + book.getBookID() + "'";
                        db->executeQuery(updateBookQuery);

                        std::cout << GREEN << "Book borrowed successfully!" << std::endl;
                        std::cout << "Book: " << book.getBookID() << RESET << std::endl;
                        std::this_thread::sleep_for(std::chrono::seconds(2));
                        borrowing = false;  // Exit the borrowing module  
                    }
                    catch (const std::exception& e) {
                        std::cout << RED << "Error: " << e.what() << RESET << std::endl;
                        std::cerr << "Debug: Query was: " << insertLoanQuery << std::endl;
                        std::this_thread::sleep_for(std::chrono::seconds(2));
                    }
                }
                else {
                    std::cout << RED << "Please select a book before confirming!" << RESET << std::endl;
                    std::this_thread::sleep_for(std::chrono::seconds(2));
                }
                break;

            case 2:  // RETURN BACK  
                borrowing = false;
                break;

            default:
                std::cout << "\nInvalid Input, please try again..." << std::endl;
                std::this_thread::sleep_for(std::chrono::seconds(2));
                break;
            }
            break;

        case KEY_ESC:
            borrowing = false;
            break;
        }
    } while (borrowing);
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
            if (lastLoanID.length() != 9 || lastLoanID[0] != 'L') {
                throw std::runtime_error("Invalid loan ID format in database");
            }

            // Extract the numeric part of the loanID and increment it
            int lastIDNumber = std::stoi(lastLoanID.substr(1));  // Extract number after 'L'
            int newIDNumber = lastIDNumber + 1;

            // Generate the new loanID with leading zeros for 6 digits
            std::ostringstream oss;
            oss << "L" << std::setw(8) << std::setfill('0') << newIDNumber;
            return oss.str();
        }

        // If no previous loanID exists, generate the first loanID
        return "L00000001";  // Default first loanID if no loans are present
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
    Fine fineManager(db);
    do {
        system("cls");
        std::cout << CYAN << "Welcome to Library Management System! " << RESET << std::endl;
        std::cout << "\Settings: " << std::endl;
        std::cout << (selected == 0 ? "-> " : "   ") << (selected == 0 ? BG_YELLOW : "") << "Change Max Book Borrowed" << RESET << std::endl;
        std::cout << (selected == 1 ? "-> " : "   ") << (selected == 1 ? BG_YELLOW : "") << "Change Borrowing Duration" << RESET << std::endl;
        std::cout << (selected == 2 ? "-> " : "   ") << (selected == 2 ? BG_YELLOW : "") << "Change Overdue Fine Rate" << RESET << std::endl;
        std::cout << (selected == 3 ? "-> " : "   ") << (selected == 3 ? BG_YELLOW : "") << "Change Max Overdue Fine" << RESET << std::endl;
        std::cout << (selected == 4 ? "-> " : "   ") << (selected == 4 ? BG_YELLOW : "") << "Change Damage Fine Offer" << RESET << std::endl;

        std::cout << "\n\n\nUse arrow keys to navigate, press Enter to select, or press Esc to quit.\n";

        // Capture user input for navigation
        char c = _getch(); // Use _getch() to get key press without waiting for enter.
        std::string exitpass;
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
                library.changeMaxBookBorrow();
                break;
            case 1:
                library.changeBorrowingDuration();
                break;
            case 2:
                fineManager.changeOverdueFineRate();
                break;
            case 3:
                fineManager.changeMaxOverdueFine();
                break;
            case 4:
                fineManager.changeDamageFinePercentage();
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

void Library::changeMaxBookBorrow() {
    dbConnection* db = new dbConnection();
    int selected = 0;  // Keeps track of which option is selected.  
    bool selecting = true;
    std::string roleQuery = "SELECT role, MAX(maxBorrowable) AS maxBorrowable "
        "FROM rolePrivelage "
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
            std::cin.ignore();
            getline(std::cin, data);

            // Validate the input  
            try {
                int maxBooks = std::stoi(data); // Convert input string to integer
                if (maxBooks <= 0) {
                    std::cout << "Error: The maximum borrowing limit must be a positive integer!" << std::endl;
                    std::this_thread::sleep_for(std::chrono::seconds(2)); // Pause for 2 seconds
                    break; // Go back to the menu
                }
            }
            catch (std::invalid_argument& e) {
                std::cout << "Error: Invalid input! Please enter a valid number." << std::endl;
                std::this_thread::sleep_for(std::chrono::seconds(2)); // Pause for 2 seconds
                break; // Go back to the menu
            }
            catch (std::out_of_range& e) {
                std::cout << "Error: The number is too large!" << std::endl;
                std::this_thread::sleep_for(std::chrono::seconds(2)); // Pause for 2 seconds
                break; // Go back to the menu
            }

            // Prepare the update query based on the selected role  
            switch (selected) {
            case 0:
                updateQuery = "UPDATE rolePrivelage SET maxBorrowable='" + data + "' WHERE role = 'User'";
                break;
            case 1:
                updateQuery = "UPDATE rolePrivelage SET maxBorrowable='" + data + "' WHERE role = 'Staff'";
                break;
            case 2:
                updateQuery = "UPDATE rolePrivelage SET maxBorrowable='" + data + "' WHERE role = 'Admin'";
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

void Library::changeBorrowingDuration() {
    dbConnection* db = new dbConnection();
    int selected = 0;  // Keeps track of which option is selected.  
    bool selecting = true;
    std::string durationQuery = "SELECT role, borrowDuration "
        "FROM rolePrivelage "
        "ORDER BY FIELD(role, 'User', 'Staff', 'Admin')";

    std::string updateQuery;
    std::cin.ignore();
    do{
        std::string data;
        system("cls");
        std::cout << "Welcome to Library Management System! " << std::endl;

        // Fetch and display current borrowing durations  
        db->fetchAndDisplayData(durationQuery);

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
            std::cout << YELLOW << "\n\nEnter new borrowing duration (in days): " << RESET << std::endl;
            std::cout << "\x1b[22;40H";
            getline(std::cin, data);

            // Validate the input  
            try {
                int borrowDuration = std::stoi(data); // Convert input string to integer
                if (borrowDuration <= 0) {
                    std::cout << "Error: The borrowing duration must be a positive integer!" << std::endl;
                    std::this_thread::sleep_for(std::chrono::seconds(2)); // Pause for 2 seconds
                    break; // Go back to the menu
                }
            }
            catch (std::invalid_argument& e) {
                std::cout << "Error: Invalid input! Please enter a valid number." << std::endl;
                std::this_thread::sleep_for(std::chrono::seconds(2)); // Pause for 2 seconds
                break; // Go back to the menu
            }
            catch (std::out_of_range& e) {
                std::cout << "Error: The number is too large!" << std::endl;
                std::this_thread::sleep_for(std::chrono::seconds(2)); // Pause for 2 seconds
                break; // Go back to the menu
            }

            // Prepare the update query based on the selected role  
            switch (selected) {
            case 0:
                updateQuery = "UPDATE rolePrivelage SET borrowDuration='" + data + "' WHERE role = 'User'";
                break;
            case 1:
                updateQuery = "UPDATE rolePrivelage SET borrowDuration='" + data + "' WHERE role = 'Staff'";
                break;
            case 2:
                updateQuery = "UPDATE rolePrivelage SET borrowDuration='" + data + "' WHERE role = 'Admin'";
                break;
            default:
                std::cout << "Invalid Input, please try again..." << std::endl;
                break;
            }

            // Execute the update query  
            db->executeQuery(updateQuery);
            std::cout << GREEN << "Successfully updated the borrowing duration." << RESET << std::endl;
            std::this_thread::sleep_for(std::chrono::seconds(2)); // Pause for 2 seconds  
            break;
        case KEY_ESC:
            selecting = false; // Exit the menu  
            break;
        }

        std::this_thread::sleep_for(std::chrono::milliseconds(100)); // Small delay for better UX  
    } while (selecting);

    delete db; // Clean up the database connection  
}


int Library::getBorrowDuration(const std::string& userRole)
{
    try
    {
        std::string query = "SELECT borrowDuration FROM rolePrivelage WHERE role='" + userRole + "'";
        std::vector<std::map<std::string, std::string>> result = db->fetchResults(query);

        if (result.empty())
        {
            throw std::runtime_error("Role not found in rolePrivelage table.");
        }

        return std::stoi(result[0]["borrowDuration"]);
    }
    catch (const std::exception& e)
    {
        std::cerr << RED << "Error retrieving borrow duration: " << e.what() << RESET << std::endl;
        return 0; // Return 0 as a default/fallback value
    }
}

void Library::returnBook(User user) {
    int selected = 0;  // Keeps track of which option is selected.  
    bool borrowing = true;
    std::string bookID;
    bool bookConfirmed = false;
    Book book(db);
    std::vector<std::map<std::string, std::string>> result;

    do {
        system("cls");
        std::cout << "Returning book for User: " << user.getUserID() << std::endl;
        std::cout << (selected == 0 ? "-> " : "   ") << (selected == 0 ? BG_YELLOW : "") << "Enter Book ID: " << RESET << std::endl;
        std::cout << (selected == 1 ? "-> " : "   ") << (selected == 1 ? BG_GREEN : "") << "CONFIRM" << RESET << std::endl;
        std::cout << (selected == 2 ? "-> " : "   ") << (selected == 2 ? BG_RED : "") << "RETURN BACK" << RESET << std::endl;
        std::cout << "\n\n\nUse arrow keys to navigate, press Enter to select, or press Esc to quit.\n";

        if (bookConfirmed) {
            // Fetch and display book details  
            std::string bookQuery = "SELECT "
                "b.bookID, "
                "b.title, "
                "b.author, "
                "b.category, "
                "b.price, "
                "l.bookStatus, "
                "l.borrow_date, "
                "l.due_date "
                "FROM Loan l "
                "JOIN Book b ON l.bookID = b.bookID "
                "WHERE userid ='" + user.getUserID() + "' AND l.bookStatus = 'Borrowing' AND l.return_date IS NULL ORDER BY due_date ASC LIMIT 1" ;

            db->fetchAndDisplayData(bookQuery);
        }

        char c = _getch(); // Use _getch() to get key press without waiting for enter.  
        switch (c) {
        case KEY_UP:
            selected = (selected - 1 + 3) % 3; // Wrap around to the last option if at the top.  
            break;
        case KEY_DOWN:
            selected = (selected + 1) % 3; // Wrap around to the first option if at the bottom.  
            break;
        case KEY_ENTER:
            switch (selected) {
            case 0:  // Enter Book ID  
                std::cout << "\x1b[2;19H"; // Move cursor to the input position
                std::getline(std::cin, bookID);

                try {
                    // Check if the book is borrowed by the user
                    std::string checkLoanQuery = "SELECT loanID, due_date FROM Loan WHERE bookID = '" + bookID + "' AND userID = '" + user.getUserID() + "' AND bookStatus = 'Borrowing' ORDER BY due_date ASC LIMIT 1";
                    result = db->fetchResults(checkLoanQuery);

                    if (result.empty()) {
                        std::cout << "Error: The book is not currently borrowed or doesn't exist!" << std::endl;
                        return;  // Exit the function if the book is not borrowed
                    }

                    // Retrieve book details and confirm the book
                    book.retrieveBookFromDB(bookID);
                    bookConfirmed = true;  // Book is valid  
                }
                catch (const std::exception& e) {
                    std::cout << RED << "Error processing book: " << e.what() << RESET << std::endl;
                    std::this_thread::sleep_for(std::chrono::seconds(2));
                    break;
                }
                break;

            case 1:  // CONFIRM  
                if (bookConfirmed && !book.getBookID().empty()) {
                    try {
                        // Fetch the loanID and dueDate for the earliest book borrowed (the first one in the result set)
                        std::string loanID = result[0]["loanID"];  // Assuming `loanID` and `dueDate` are in the result map
                        std::string dueDate = result[0]["due_date"];
                        std::string currentDate = getCurrentTime();  // Get current date/time

                        // If the book is returned after due date, handle the logic for overdue if needed

                        // Return the book: Update the Loan record and set the book status to returned
                        std::string updateLoanQuery = "UPDATE Loan SET bookStatus = 'Returned', return_date ='" + currentDate + "' WHERE loanID ='" + loanID + "'";
                        db->executeQuery(updateLoanQuery);

                        // Update the Book's quantity (make it available again)
                        std::string updateBookQuery = "UPDATE Book SET quantity = quantity + 1 WHERE bookID ='" + book.getBookID() + "'";
                        db->executeQuery(updateBookQuery);

                        std::cout << GREEN << "Book Returned successfully!" << std::endl;
                        std::cout << "Book: " << book.getBookID() << RESET << std::endl;
                        std::this_thread::sleep_for(std::chrono::seconds(2));
                        borrowing = false;  // Exit the returning module  
                    }
                    catch (const std::exception& e) {
                        std::cout << RED << "Error: " << e.what() << RESET << std::endl;
                        std::this_thread::sleep_for(std::chrono::seconds(2));
                    }
                }
                else {
                    std::cout << RED << "Please select a book before confirming!" << RESET << std::endl;
                    std::this_thread::sleep_for(std::chrono::seconds(2));
                }
                break;

            case 2:  // RETURN BACK  
                borrowing = false;
                break;

            default:
                std::cout << "\nInvalid Input, please try again..." << std::endl;
                std::this_thread::sleep_for(std::chrono::seconds(2));
                break;
            }
            break;

        case KEY_ESC:
            borrowing = false;
            break;
        }
    } while (borrowing);
}

void Library::updateOverdueStatus()
{
    dbConnection* db = new dbConnection();

    // Get current date and time
    std::string currentDatetime = getCurrentTime(); // Use the getCurrentDatetime function

    std::string query = "SELECT loanID, userID, bookID, due_date FROM Loan WHERE bookStatus = 'Borrowing'";
    std::vector<std::map<std::string, std::string>> results = db->fetchResults(query);

    for (const auto& result : results)
    {
        std::string loanID = result.at("loanID");
        std::string dueDate = result.at("due_date");

        // Compare current date and time with due date
        if (currentDatetime > dueDate)  // If the current date and time is after the due date
        {
            // Update status to 'Overdue'
            std::string updateQuery = "UPDATE Loan SET bookStatus = 'Overdue' WHERE loanID = '" + loanID + "'";
            db->executeQuery(updateQuery);
        }
    }

    delete db;
}

void Library::ModulesMenu(User user)
{
    dbConnection* db = new dbConnection();
    bool loggedIn = false;
    int selected = 0;  // Keeps track of which option is selected.
    bool selecting = true;
    Library library(db);
    Fine fineManager(db);
    do {
        system("cls");
        std::cout << CYAN << "Welcome to Library Management System! " << RESET << std::endl;
        std::cout << "\Settings: " << std::endl;
        std::cout << (selected == 0 ? "-> " : "   ") << (selected == 0 ? BG_YELLOW : "") << "Borrowing" << RESET << std::endl;
        std::cout << (selected == 1 ? "-> " : "   ") << (selected == 1 ? BG_YELLOW : "") << "Returning" << RESET << std::endl;
        std::cout << (selected == 2 ? "-> " : "   ") << (selected == 2 ? BG_YELLOW : "") << "Apply Fine" << RESET << std::endl;

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
                library.borrowBook(user);
                break;
            case 1:
                library.returnBook(user);
                break;
            case 2:
                fineManager.fineMenu(user);
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
