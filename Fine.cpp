// Fine.cpp
#include "Fine.h"
#include "tc.h"
#include "user.h"
#include <iostream>
#include <conio.h>
#include <thread>
#include <sstream>
#include <iomanip>
#include "time.h"
#include <tabulate/table_internal.hpp>
#include <tabulate/table.hpp>
#include "table.h"

// Constructor
Fine::Fine(dbConnection* dbConnection) : db(dbConnection) {}

// Constructor
Fine::Fine(const std::string& fineID, const std::string& loanID, const std::string& payment_date,
    int amount, const std::string& reason, int dueDay, const std::string& paymentStatus)
    : fineID(fineID), loanID(loanID), payment_date(payment_date), amount(amount),
    reason(reason), dueDay(dueDay), paymentStatus(paymentStatus)
{
}

// Getters (optional, if you want access to private members)
std::string Fine::getLoanID() const {
    return loanID;
}

std::string Fine::getFineID() const {
    return fineID;
}

int Fine::getAmount() const {
    return amount;
}

std::string Fine::getPaymentDate() const {
    return payment_date;
}

const std::string& Fine::getReason() const
{
    return reason;
}

void Fine::setLoanID(const std::string& id) {
    this->loanID = id;
}

void Fine::setFineID(const std::string& id) {
    this->fineID = id;
}

void Fine::setAmount(int amount) {
    this->amount = amount;
}

void Fine::setPaymentDate(const std::string& date) {
    this->payment_date = date;
}

void Fine::setReason(const std::string& newReason)
{
    this->reason = newReason;
}

void Fine::setDueDay(const int dueDay)
{
    this->dueDay = dueDay;
}

const int Fine::getDueDay() const {
    return dueDay;
}

const std::vector<Fine>& Fine::getFines() const {
    return fineBooks;
}

// Method to clear all fines (e.g., after payment is made)
void Fine::clearFines() {
    fineBooks.clear();
}
void Fine::changeOverdueFineRate()
{
    dbConnection* db = new dbConnection();
    int selected = 0;
    bool selecting = true;
    std::string fineRateQuery = "SELECT role, overdueFineRate "
        "FROM rolePrivelage "
        "ORDER BY FIELD(role, 'User', 'Staff', 'Admin')";

    std::string updateQuery;
    std::cin.ignore();
    do {
        std::string data;
        system("cls");
        std::cout << "Welcome to Library Management System!" << std::endl;

        // Fetch and display current overdue fine rates
        db->fetchAndDisplayData(fineRateQuery);

        std::cout << "\nSelect Role: " << std::endl;
        std::cout << (selected == 0 ? "-> " : "   ") << (selected == 0 ? BG_YELLOW : "") << "User" << RESET << std::endl;
        std::cout << (selected == 1 ? "-> " : "   ") << (selected == 1 ? BG_YELLOW : "") << "Staff" << RESET << std::endl;
        std::cout << (selected == 2 ? "-> " : "   ") << (selected == 2 ? BG_YELLOW : "") << "Admin" << RESET << std::endl;

        std::cout << "\n\n\nUse arrow keys to navigate, press Enter to select, or press Esc to quit.\n";

        // Capture user input for navigation
        char c = _getch();
        switch (c) {
        case KEY_UP:
            selected = (selected - 1 + 3) % 3;
            break;
        case KEY_DOWN:
            selected = (selected + 1) % 3;
            break;
        case KEY_ENTER:
            std::cout << YELLOW << "\n\nEnter new overdue fine rate: " << RESET << std::endl;
            std::cout << "\x1b[22;40H";
            getline(std::cin, data);

            // Validate the input
            try {
                double fineRate = std::stod(data);
                if (fineRate <= 0) {
                    std::cout << "Error: The fine rate must be a positive value!" << std::endl;
                    std::this_thread::sleep_for(std::chrono::seconds(2));
                    break;
                }
            }
            catch (std::invalid_argument& e) {
                std::cout << "Error: Invalid input! Please enter a valid number." << std::endl;
                std::this_thread::sleep_for(std::chrono::seconds(2));
                break;
            }

            // Prepare the update query based on the selected role
            switch (selected) {
            case 0:
                updateQuery = "UPDATE rolePrivelage SET overdueFineRate='" + data + "' WHERE role = 'User'";
                break;
            case 1:
                updateQuery = "UPDATE rolePrivelage SET overdueFineRate='" + data + "' WHERE role = 'Staff'";
                break;
            case 2:
                updateQuery = "UPDATE rolePrivelage SET overdueFineRate='" + data + "' WHERE role = 'Admin'";
                break;
            default:
                std::cout << "Invalid Input, please try again..." << std::endl;
                break;
            }

            // Execute the update query
            db->executeQuery(updateQuery);
            std::cout << GREEN << "Successfully updated the overdue fine rate." << RESET << std::endl;
            std::this_thread::sleep_for(std::chrono::seconds(2));
            break;
        case KEY_ESC:
            selecting = false;
            break;
        }

        std::this_thread::sleep_for(std::chrono::milliseconds(100));
    } while (selecting);

    delete db;
}

void Fine::changeMaxOverdueFine() {
    dbConnection* db = new dbConnection();
    int selected = 0;
    bool selecting = true;
    std::string maxFineQuery = "SELECT role, maxOverdueFine "
        "FROM rolePrivelage "
        "ORDER BY FIELD(role, 'User', 'Staff', 'Admin')";

    std::string updateQuery;
    std::cin.ignore();
    do {
        std::string data;
        system("cls");
        std::cout << "Welcome to Library Management System!" << std::endl;

        // Fetch and display current max overdue fines
        db->fetchAndDisplayData(maxFineQuery);

        std::cout << "\nSelect Role: " << std::endl;
        std::cout << (selected == 0 ? "-> " : "   ") << (selected == 0 ? BG_YELLOW : "") << "User" << RESET << std::endl;
        std::cout << (selected == 1 ? "-> " : "   ") << (selected == 1 ? BG_YELLOW : "") << "Staff" << RESET << std::endl;
        std::cout << (selected == 2 ? "-> " : "   ") << (selected == 2 ? BG_YELLOW : "") << "Admin" << RESET << std::endl;

        std::cout << "\n\n\nUse arrow keys to navigate, press Enter to select, or press Esc to quit.\n";

        // Capture user input for navigation
        char c = _getch();
        switch (c) {
        case KEY_UP:
            selected = (selected - 1 + 3) % 3;
            break;
        case KEY_DOWN:
            selected = (selected + 1) % 3;
            break;
        case KEY_ENTER:
            std::cout << YELLOW << "\n\nEnter new maximum overdue fine: " << RESET << std::endl;
            std::cout << "\x1b[22;40H";
            getline(std::cin, data);

            // Validate the input
            try {
                double maxFine = std::stod(data);
                if (maxFine <= 0) {
                    std::cout << "Error: The maximum fine must be a positive value!" << std::endl;
                    std::this_thread::sleep_for(std::chrono::seconds(2));
                    break;
                }
            }
            catch (std::invalid_argument& e) {
                std::cout << "Error: Invalid input! Please enter a valid number." << std::endl;
                std::this_thread::sleep_for(std::chrono::seconds(2));
                break;
            }

            // Prepare the update query based on the selected role
            switch (selected) {
            case 0:
                updateQuery = "UPDATE rolePrivelage SET maxOverdueFine='" + data + "' WHERE role = 'User'";
                break;
            case 1:
                updateQuery = "UPDATE rolePrivelage SET maxOverdueFine='" + data + "' WHERE role = 'Staff'";
                break;
            case 2:
                updateQuery = "UPDATE rolePrivelage SET maxOverdueFine='" + data + "' WHERE role = 'Admin'";
                break;
            default:
                std::cout << "Invalid Input, please try again..." << std::endl;
                break;
            }

            // Execute the update query
            db->executeQuery(updateQuery);
            std::cout << GREEN << "Successfully updated the maximum overdue fine." << RESET << std::endl;
            std::this_thread::sleep_for(std::chrono::seconds(2));
            break;
        case KEY_ESC:
            selecting = false;
            break;
        }

        std::this_thread::sleep_for(std::chrono::milliseconds(100));
    } while (selecting);

    delete db;
}

void Fine::changeDamageFinePercentage()
{
    dbConnection* db = new dbConnection();
    int selected = 0;
    bool selecting = true;
    std::string damageFineQuery = "SELECT role, damageFinePercentage "
        "FROM rolePrivelage "
        "ORDER BY FIELD(role, 'User', 'Staff', 'Admin')";

    std::string updateQuery;
    std::cin.ignore();
    do {
        std::string data;
        system("cls");
        std::cout << "Welcome to Library Management System!" << std::endl;

        // Fetch and display current damage fine percentages
        db->fetchAndDisplayData(damageFineQuery);

        std::cout << "\nSelect Role: " << std::endl;
        std::cout << (selected == 0 ? "-> " : "   ") << (selected == 0 ? BG_YELLOW : "") << "User" << RESET << std::endl;
        std::cout << (selected == 1 ? "-> " : "   ") << (selected == 1 ? BG_YELLOW : "") << "Staff" << RESET << std::endl;
        std::cout << (selected == 2 ? "-> " : "   ") << (selected == 2 ? BG_YELLOW : "") << "Admin" << RESET << std::endl;

        std::cout << "\n\n\nUse arrow keys to navigate, press Enter to select, or press Esc to quit.\n";

        // Capture user input for navigation
        char c = _getch();
        switch (c) {
        case KEY_UP:
            selected = (selected - 1 + 3) % 3;
            break;
        case KEY_DOWN:
            selected = (selected + 1) % 3;
            break;
        case KEY_ENTER:
            std::cout << YELLOW << "\n\nEnter new damage fine percentage: " << RESET << std::endl;
            std::cout << "\x1b[22;40H";
            getline(std::cin, data);

            // Validate the input
            try {
                double damageFine = std::stod(data);
                if (damageFine <= 0) {
                    std::cout << "Error: The fine percentage must be a positive value!" << std::endl;
                    std::this_thread::sleep_for(std::chrono::seconds(2));
                    break;
                }
            }
            catch (std::invalid_argument& e) {
                std::cout << "Error: Invalid input! Please enter a valid number." << std::endl;
                std::this_thread::sleep_for(std::chrono::seconds(2));
                break;
            }

            // Prepare the update query based on the selected role
            switch (selected) {
            case 0:
                updateQuery = "UPDATE rolePrivelage SET damageFinePercentage='" + data + "' WHERE role = 'User'";
                break;
            case 1:
                updateQuery = "UPDATE rolePrivelage SET damageFinePercentage='" + data + "' WHERE role = 'Staff'";
                break;
            case 2:
                updateQuery = "UPDATE rolePrivelage SET damageFinePercentage='" + data + "' WHERE role = 'Admin'";
                break;
            default:
                std::cout << "Invalid Input, please try again..." << std::endl;
                break;
            }

            // Execute the update query
            db->executeQuery(updateQuery);
            std::cout << GREEN << "Successfully updated the damage fine percentage." << RESET << std::endl;
            std::this_thread::sleep_for(std::chrono::seconds(2));
            break;
        case KEY_ESC:
            selecting = false;
            break;
        }

        std::this_thread::sleep_for(std::chrono::milliseconds(100));
    } while (selecting);

    delete db;
}

std::string Fine::generateFineID(sql::Connection* conn)
{
    try
    {
        if (!conn)
        {
            throw std::runtime_error("Database connection not available");
        }

        std::string query = "SELECT fineID FROM Fine ORDER BY fineID DESC LIMIT 1";

        std::unique_ptr<sql::PreparedStatement> pstmt(conn->prepareStatement(query));

        if (!pstmt)
        {
            throw std::runtime_error("Failed to create prepared statement");
        }

        std::unique_ptr<sql::ResultSet> res(pstmt->executeQuery());

        if (!res)
        {
            throw std::runtime_error("Failed to execute query");
        }

        if (res->next())
        {
            std::string lastFineID = res->getString("fineID");

            // Check if the fineID follows the format F########
            if (lastFineID.length() != 9 || lastFineID[0] != 'F')
            {
                throw std::runtime_error("Invalid fine ID format in database");
            }

            try
            {
                // Extract the number part (8 digits)
                int lastIDNumber = std::stoi(lastFineID.substr(1));
                int newIDNumber = lastIDNumber + 1;

                // Format with 8 digits
                std::ostringstream oss;
                oss << "F" << std::setw(8) << std::setfill('0') << newIDNumber;
                return oss.str();
            }
            catch (const std::exception& e)
            {
                throw std::runtime_error("Failed to parse fine ID number: " + std::string(e.what()));
            }
        }

        return "F00000001";  // Default first ID if no fines are present
    }
    catch (const sql::SQLException& e)
    {
        throw std::runtime_error("Database error in generateFineID: " + std::string(e.what()));
    }
    catch (const std::exception& e)
    {
        throw std::runtime_error("Error in generateFineID: " + std::string(e.what()));
    }
}


void Fine::fineMenu(User user) {
    dbConnection* db = new dbConnection();
    bool selecting = true;
    int selected = 0;
    std::string loanID, bookStatus, bookID;
    std::string query;
    char confirmation;
    Book book(db);
    std::vector<std::map<std::string, std::string>> result;
    struct FineRecord {
        std::string loanID;
        std::string bookID;
        std::string bookStatus;
        int dueDays;
        std::string fineReason;
    };
    FineRecord newFineRecord;
    std::vector<FineRecord> totalFines;
    std::string dueDateStr, bookIdStr;
    std::string statusQuery =
        "SELECT l.loanID, l.bookID, b.title AS bookName, l.borrow_date, l.due_date, l.bookStatus "
        "FROM Loan l "
        "JOIN Book b ON l.bookID = b.bookID "
        "WHERE l.bookStatus IN('Overdue', 'Lost', 'Damaged') "
        "AND l.return_date IS NULL "
        "AND l.userID = '" + user.getUserID() + "'";

    do {
        system("cls");
        std::cout << "=== Fine Management Menu ===" << std::endl;
        std::cout << (selected == 0 ? "-> " : "   ") << (selected == 0 ? BG_YELLOW : "") << "Insert Loan ID: " << RESET << std::endl;
        std::cout << (selected == 1 ? "-> " : "   ") << (selected == 1 ? BG_GREEN : "") << "Pay Selected Fines" << RESET << std::endl;
        std::cout << (selected == 2 ? "-> " : "   ") << (selected == 2 ? BG_RED : "") << "Return to Main Menu" << RESET << std::endl;

        std::cout << "\n\nUse arrow keys to navigate, press Enter to select, or press Esc to quit.\n";

        std::cout << "=== Fine Books ===" << std::endl;

        // Fetch and display data of books with 'Overdue', 'Lost', or 'Damaged' status
        db->fetchAndDisplayData(statusQuery);  // This method uses the query and fetches/display the data

        std::cout << "=== Total Fine List ===" << std::endl;
        tabulate::Table fineTable;
        
        fineTable.add_row({ "Loan ID", "Book ID", "Book Status", "Due Days", "Fine Reason" });

        for (auto& fine : totalFines)
        {
            fineTable.add_row({ fine.loanID, fine.bookID, fine.bookStatus, std::to_string(fine.dueDays), fine.fineReason });
        }

        tableFormat(fineTable);

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
            switch (selected) {
            case 0:  // Insert Book ID
                std::cout << "\x1b[2;20H"; // Move cursor to the input position
                std::getline(std::cin, loanID);

                // Query to check if the book is borrowed by the user and if it's in 'Overdue', 'Lost', or 'Damaged' status.
                query = "SELECT loanID, userID, bookID, bookStatus FROM Loan WHERE loanID = '" + loanID + "' AND userID='" + user.getUserID() + "' AND bookStatus IN ('Overdue', 'Lost', 'Damaged') AND return_date IS NULL ORDER BY due_date ASC LIMIT 1";
                result = db->fetchResults(query);

                if (result.empty()) {
                    // Error if the book is not borrowed by the user or not in 'Overdue/Lost/Damaged' status
                    std::cout << RED << "Error: This book is not currently borrowed or not in Overdue/Lost/Damaged status." << RESET << std::endl;
                    std::this_thread::sleep_for(std::chrono::seconds(2)); // Wait before going back to the menu
                    break;
                }

                // Error handling if the book is borrowed by another user
                if (result[0]["userID"] != user.getUserID()) {
                    std::cout << RED << "Error: This book is borrowed by another user, not by you." << RESET << std::endl;
                    std::this_thread::sleep_for(std::chrono::seconds(2)); // Wait before going back to the menu
                    break;
                }

                // Error handling if the book is still in "Borrowing" status
                if (result[0]["bookStatus"] == "Borrowing") {
                    std::cout << RED << "Error: This book is still in Borrowing status, cannot change status." << RESET << std::endl;
                    std::this_thread::sleep_for(std::chrono::seconds(2)); // Wait before going back to the menu
                    break;
                }

                loanID = result[0]["loanID"];
                bookStatus = result[0]["bookStatus"];

                std::cout << "\x1b[6;1H" << "Are you sure you want to change the status? (Y/N): ";
                confirmation = _getch();
                if (confirmation == 'Y' || confirmation == 'y') {
                    // Proceed to change the status if confirmed
                    bool statusSelecting = true;
                    int statusSelected = 0;

                    while (statusSelecting) {
                        system("cls"); // Clear the screen
                        std::cout << "Select the new status:\n";
                        std::cout << (statusSelected == 0 ? "-> " : "   ") << (statusSelected == 0 ? BG_YELLOW : "") << "Damaged" << RESET << std::endl;
                        std::cout << (statusSelected == 1 ? "-> " : "   ") << (statusSelected == 1 ? BG_YELLOW : "") << "Overdue" << RESET << std::endl;
                        std::cout << (statusSelected == 2 ? "-> " : "   ") << (statusSelected == 2 ? BG_YELLOW : "") << "Lost" << RESET << std::endl;

                        std::cout << "\nUse arrow keys to navigate, press Enter to confirm, or press Esc to cancel.\n";

                        char statusKey = _getch(); // Capture key press for status selection
                        switch (statusKey) {
                        case KEY_UP:
                            statusSelected = (statusSelected - 1 + 3) % 3; // Wrap around to the last option if at the top
                            break;
                        case KEY_DOWN:
                            statusSelected = (statusSelected + 1) % 3; // Wrap around to the first option if at the bottom
                            break;
                        case KEY_ENTER:
                            // Change the status based on selection
                            switch (statusSelected) {
                            case 0: // Damaged
                                db->executeQuery("UPDATE Loan SET bookStatus = 'Damaged' WHERE loanID = '" + loanID + "' AND return_date IS NULL");
                                bookStatus = "Damaged";
                                statusSelecting = false;
                                break;
                            case 1: // Overdue
                                db->executeQuery("UPDATE Loan SET bookStatus = 'Overdue' WHERE loanID = '" + loanID + "' AND return_date IS NULL");
                                bookStatus = "Overdue";
                                statusSelecting = false;
                                break;
                            case 2: // Lost
                                db->executeQuery("UPDATE Loan SET bookStatus = 'Lost' WHERE loanID = '" + loanID + "' AND return_date IS NULL");
                                bookStatus = "Lost";
                                statusSelecting = false;
                                break;
                            default:
                                std::cout << "Invalid choice. No status updated.\n";
                                statusSelecting = false;
                                break;
                            }
                            break;
                        case KEY_ESC:
                            statusSelecting = false; // Cancel status change
                            break;
                        }
                    }
                }

                // Now insert the data into the struct and vector
                newFineRecord.loanID = loanID;
                bookIdStr = result[0]["bookID"];
                newFineRecord.bookID = bookIdStr;
                newFineRecord.bookStatus = bookStatus;

                // Calculate dueDays using the due_date from the Loan table
                dueDateStr = result[0]["due_date"];
                newFineRecord.dueDays = getDueTime(dueDateStr);

                // Set fine reason based on the book status
                newFineRecord.fineReason = "Book is " + bookStatus;

                // Add to totalFines vector
                totalFines.push_back(newFineRecord);

                std::cout << "Fine added to total fines." << std::endl;

                break;

            case 1:  // Pay Selected Fines
                break;

            case 2:  // Return to Main Menu
                selecting = false; // Exit the menu
                break;

            default:
                std::cout << "\nInvalid Input, please try again..." << std::endl;
                break;
            }
            break;

        case KEY_ESC:
            selecting = false; // Exit the menu
            break;
        }

        std::this_thread::sleep_for(std::chrono::milliseconds(1));
    } while (selecting);

    delete db;
}
