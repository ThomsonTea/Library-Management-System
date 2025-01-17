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
        "FROM rolePrivilege "
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
                updateQuery = "UPDATE rolePrivilege SET overdueFineRate='" + data + "' WHERE role = 'User'";
                break;
            case 1:
                updateQuery = "UPDATE rolePrivilege SET overdueFineRate='" + data + "' WHERE role = 'Staff'";
                break;
            case 2:
                updateQuery = "UPDATE rolePrivilege SET overdueFineRate='" + data + "' WHERE role = 'Admin'";
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
        "FROM rolePrivilege "
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
                updateQuery = "UPDATE rolePrivilege SET maxOverdueFine='" + data + "' WHERE role = 'User'";
                break;
            case 1:
                updateQuery = "UPDATE rolePrivilege SET maxOverdueFine='" + data + "' WHERE role = 'Staff'";
                break;
            case 2:
                updateQuery = "UPDATE rolePrivilege SET maxOverdueFine='" + data + "' WHERE role = 'Admin'";
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

void Fine::changedamagedFinePercentage()
{
    dbConnection* db = new dbConnection();
    int selected = 0;
    bool selecting = true;
    std::string damageFineQuery = "SELECT role, damagedFinePercentage "
        "FROM rolePrivilege "
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
                updateQuery = "UPDATE rolePrivilege SET damagedFinePercentage='" + data + "' WHERE role = 'User'";
                break;
            case 1:
                updateQuery = "UPDATE rolePrivilege SET damagedFinePercentage='" + data + "' WHERE role = 'Staff'";
                break;
            case 2:
                updateQuery = "UPDATE rolePrivilege SET damagedFinePercentage='" + data + "' WHERE role = 'Admin'";
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
    std::string loanID, bookStatus, bookID, bookPriceStr, bookTitleStr;
    std::string query;
    char confirmation;
    int dueDays;
    Book book(db);
    std::vector<std::map<std::string, std::string>> result;
    FineRecord newFineRecord;
    std::vector<FineRecord> totalFines;
    std::string dueDateStr, bookIdStr;
    std::string statusQuery =
        "SELECT l.loanID, l.bookID, b.title AS bookName, l.borrow_date, l.due_date, l.bookStatus "
        "FROM Loan l "
        "JOIN Book b ON l.bookID = b.bookID "
        "WHERE l.bookStatus IN('Overdue', 'Lost', 'Damaged', 'Borrowing') "
        "AND l.return_date IS NULL "
        "AND l.userID = '" + user.getUserID() + "'";

    do {
        system("cls");
        std::cout << "=== Fine Management Menu ===" << std::endl;
        std::cout << (selected == 0 ? "-> " : "   ") << (selected == 0 ? BG_YELLOW : "") << "Insert Loan ID: " << RESET << std::endl;
        std::cout << (selected == 1 ? "-> " : "   ") << (selected == 1 ? BG_GREEN : "") << "Pay Selected Fines" << RESET << std::endl;
        std::cout << (selected == 2 ? "-> " : "   ") << (selected == 2 ? BG_RED : "") << "Return to Main Menu" << RESET << std::endl;

        std::cout << "\n\nUse arrow keys to navigate, press Enter to select, or press Esc to quit.\n";

        // Display fine books
        std::cout << "=== Fine Books ===" << std::endl;
        db->fetchAndDisplayData(statusQuery);

        // Display total fines table if the vector is not empty
        if (!totalFines.empty()) {
            std::cout << "=== Total Fine List ===" << std::endl;
            tabulate::Table fineTable;
            fineTable.add_row({ "Loan ID", "Book ID", "Book Title", "Book Price", "Book Status", "Due Days", "Fine Reason" });

            for (auto& fine : totalFines) {
                fineTable.add_row({ fine.loanID, fine.bookID, fine.bookTitle, fine.bookPrice, fine.bookStatus, std::to_string(fine.dueDays), fine.fineReason });
            }

            tableFormat(fineTable);
        }

        char c = _getch();
        switch (c) {
        case KEY_UP:
            selected = (selected - 1 + 3) % 3;
            break;
        case KEY_DOWN:
            selected = (selected + 1) % 3;
            break;
        case KEY_ENTER:
            switch (selected) {
            case 0: {  // Insert Loan ID
                std::cout << "\x1b[2;20H";
                std::getline(std::cin, loanID);

                query =
                    "SELECT l.loanID, l.userID, l.bookID, l.bookStatus, l.due_date, b.title, b.price "
                    "FROM Loan l "
                    "JOIN Book b ON l.bookID = b.bookID "
                    "WHERE l.loanID = '" + loanID + "' "
                    "AND l.userID = '" + user.getUserID() + "' "
                    "AND l.bookStatus IN ('Borrowing', 'Overdue', 'Lost', 'Damaged') "
                    "AND l.return_date IS NUll "
                    "ORDER BY l.due_date ASC LIMIT 1";

                result = db->fetchResults(query);

                if (result.empty()) {
                    std::cout << RED << "Error: No matching book found in the required status." << RESET << std::endl;
                    std::this_thread::sleep_for(std::chrono::seconds(2));
                    break;
                }

                loanID = result[0]["loanID"];
                bookStatus = result[0]["bookStatus"];
                dueDateStr = result[0]["due_date"];
                bookIdStr = result[0]["bookID"];
                bookTitleStr = result[0]["title"];
                bookPriceStr = result[0]["price"];
                dueDays = 0;  // Default for non-overdue books

                // Handle status updates for 'Damaged' or 'Lost'
                if (bookStatus == "Borrowing" || bookStatus == "Overdue") {
                    std::cout << YELLOW << "\x1b[6;1H" << "Do you want to update the status of this book to 'Damaged' or 'Lost'? (Y/N): " << RESET;
                    confirmation = _getch();
                    if (confirmation == 'Y' || confirmation == 'y') {
                        bool statusSelecting = true;
                        int statusSelected = 0;

                        // Let user choose the new status
                        while (statusSelecting) {
                            system("cls");
                            std::cout << "Select the new status for the book:\n";
                            std::cout << (statusSelected == 0 ? "-> " : "   ") << BG_YELLOW << "Damaged" << RESET << std::endl;
                            std::cout << (statusSelected == 1 ? "-> " : "   ") << BG_YELLOW << "Lost" << RESET << std::endl;

                            std::cout << "\nUse arrow keys to navigate and press Enter to confirm.\n";

                            char statusKey = _getch();
                            switch (statusKey) {
                            case KEY_UP:
                                statusSelected = (statusSelected - 1 + 2) % 2;  // Wrap around
                                break;
                            case KEY_DOWN:
                                statusSelected = (statusSelected + 1) % 2;  // Wrap around
                                break;
                            case KEY_ENTER:
                                if (statusSelected == 0) {
                                    db->executeQuery("UPDATE Loan SET bookStatus = 'Damaged' WHERE loanID = '" + loanID + "'");
                                    bookStatus = "Damaged";
                                }
                                else if (statusSelected == 1) {
                                    db->executeQuery("UPDATE Loan SET bookStatus = 'Lost' WHERE loanID = '" + loanID + "'");
                                    bookStatus = "Lost";
                                }
                                std::cout << GREEN << "Book status updated to '" << bookStatus << "' successfully!" << RESET << std::endl;
                                std::this_thread::sleep_for(std::chrono::seconds(2));
                                statusSelecting = false;
                                break;
                            case KEY_ESC:
                                statusSelecting = false;  // Cancel status update
                                break;
                            }
                        }
                    }
                }

                // Process fine based on book status
                if (bookStatus == "Overdue") {
                    dueDays = getDueTime(dueDateStr);
                    if (dueDays == -1) {
                        std::cout << RED << "Invalid due date for calculation. Fine not added." << RESET << std::endl;
                        std::this_thread::sleep_for(std::chrono::seconds(2));
                        break;
                    }
                    newFineRecord.fineReason = "Book is Overdue";
                }
                else if (bookStatus == "Damaged") {
                    newFineRecord.fineReason = "Book is Damaged";
                }
                else if (bookStatus == "Lost") {
                    newFineRecord.fineReason = "Book is Lost";
                }
                else {
                    break;  // Skip invalid statuses
                }

                // Add fine to the list
                newFineRecord.loanID = loanID;
                newFineRecord.bookID = bookIdStr;
                newFineRecord.bookTitle = bookTitleStr;
                newFineRecord.bookPrice = bookPriceStr;
                newFineRecord.bookStatus = bookStatus;
                newFineRecord.dueDays = dueDays;
                totalFines.push_back(newFineRecord);

                std::cout << GREEN << "\x1b[2;19H" << "Fine added successfully!" << RESET << std::endl;
                std::this_thread::sleep_for(std::chrono::seconds(2));
                break;
            case 1:  // Pay Selected Fines
                doPayment(user, totalFines);
                selecting = false;
                break;
            case 2:  // Return to Main Menu
                selecting = false;
                break;
            default:
                std::cout << RED << "Invalid option selected." << RESET << std::endl;
                break;
            }
                  break;
            case KEY_ESC:
                selecting = false;
                break;
            }
        }
    }while (selecting);
}


void Fine::doPayment(User user, std::vector<FineRecord>& totalFines)
{
    dbConnection* db = new dbConnection();
    system("cls");
    try
    {
        // Check if the user exists
        std::string userCheckQuery = "SELECT 1 FROM User WHERE userID = '" + user.getUserID() + "'";
        if (!db->recordExists(userCheckQuery))
        {
            throw std::runtime_error("Error: User information not found!");
        }

        // Fetch user's name and role
        std::string userQuery = "SELECT name, role FROM User WHERE userID = '" + user.getUserID() + "'";
        std::vector<std::map<std::string, std::string>> userResult = db->fetchResults(userQuery);
        std::string username = userResult[0]["name"];
        std::string userRole = userResult[0]["role"];

        // Fetch role-specific privileges
        std::string privilegesQuery = "SELECT overdueFineRate, maxOverdueFine, damagedFinePercentage "
            "FROM rolePrivilege WHERE role = '" + userRole + "'";
        if (!db->recordExists("SELECT 1 FROM rolePrivilege WHERE role = '" + userRole + "'"))
        {
            throw std::runtime_error("Error: Role privileges not found!");
        }
        std::vector<std::map<std::string, std::string>> privilegesResult = db->fetchResults(privilegesQuery);

        double overdueFineRate = std::stod(privilegesResult[0]["overdueFineRate"]);
        double maxOverdueFine = std::stod(privilegesResult[0]["maxOverdueFine"]);
        double damagedFinePercentage = std::stod(privilegesResult[0]["damagedFinePercentage"]);
        std::string currentDateTime = getCurrentTime();

        // Prepare receipt
        std::cout << "\n================= Payment Receipt =================\n";
        std::cout << "User ID: " << user.getUserID() << "\n";
        std::cout << "Username: " << user.getName() << "\n";
        std::cout << "Role: " << user.getRole() << "\n";
        std::cout << "Payment Date (YYYY-MM-DD HH/MM/SS): " << currentDateTime << "\n";
        std::cout << "==================================================\n";

        tabulate::Table receiptTable;
        receiptTable.add_row({ "Loan ID", "Book ID", "Book Title", "Book Price", "Status", "Calculation", "Fine Amount" });

        double totalAmount = 0.0;

        for (const auto& fine : totalFines)
        {
            double fineAmount = 0.0;
            std::string calculation;

            if (fine.bookStatus == "Overdue")
            {
                fineAmount = std::min(fine.dueDays * overdueFineRate, maxOverdueFine);
                calculation = std::to_string(fine.dueDays) + " Days * RM" + std::format("{:.2f}", overdueFineRate);
            }
            else if (fine.bookStatus == "Damaged")
            {
                fineAmount = std::stod(fine.bookPrice) * (damagedFinePercentage / 100);
                calculation = "RM" + fine.bookPrice + " * " + std::to_string(damagedFinePercentage) + "%";
            }
            else if (fine.bookStatus == "Lost")
            {
                fineAmount = std::stod(fine.bookPrice);
                calculation = "RM" + fine.bookPrice + " (Full Price)";
            }

            totalAmount += fineAmount;

            receiptTable.add_row({
                fine.loanID,
                fine.bookID,
                fine.bookTitle,
                fine.bookPrice,
                fine.bookStatus,
                calculation,
                "RM" + std::format("{:.2f}", fineAmount)
                });
        }

        tableFormat(receiptTable);
        std::cout << "\nTotal Amount to be Paid: RM" << std::format("{:.2f}", totalAmount) << "\n";
        std::cout << "==================================================\n";

        // Confirm payment
        char paymentConfirmation;
        std::cout << "Do you want to pay the total fine? (Y/N): ";
        std::cin >> paymentConfirmation;

        if (paymentConfirmation == 'Y' || paymentConfirmation == 'y')
        {
            // Generate a unique fine ID and get the current date/time
            std::string fineID = generateFineID(db->getConnection());
            std::string paymentDate = getCurrentTime();

            // Insert payment details into the Fine table
            std::string insertFineQuery = "INSERT INTO Fine (fineID, payment_date, amount, paymentStatus) VALUES ('" +
                fineID + "', '" + paymentDate + "', " + std::format("{:.2f}", totalAmount) + ", 'Done')";
            db->executeQuery(insertFineQuery);

            for (const auto& fine : totalFines)
            {
                // Update Loan table with fine ID and payment date
                std::string updateLoanQuery = "UPDATE Loan SET fineID = '" + fineID + "', return_date = '" + paymentDate + "' WHERE loanID = '" + fine.loanID + "'";
                db->executeQuery(updateLoanQuery);

                // If the book status is Overdue or Damaged, increment the quantity in the Book table
                if (fine.bookStatus == "Overdue" || fine.bookStatus == "Damaged")
                {
                    std::string updateBookQuery = "UPDATE Book SET quantity = quantity + 1 WHERE bookID = '" + fine.bookID + "'";
                    db->executeQuery(updateBookQuery);
                }
            }



            std::cout << "\nPayment successful! All fines have been processed and recorded.\n";
            system("pause");
        }
        else
        {
            std::cout << "\nPayment cancelled. No data was saved.\n";
        }
    }
    catch (const std::exception& e)
    {
        std::cerr << "Error: " << e.what() << "\n";
    }

    delete db; // Clean up database connection
}

