#ifndef LIBRARY_H
#define LIBRARY_H

#include "book.h"
#include "user.h"
#include "dbConnection.h"  // Assuming dbConnection is defined elsewhere
#include <string>

class Library
{
public:
    // Constructor that takes a dbConnection pointer
    Library(dbConnection* connection);

    // Method to borrow a book
    void borrowBookMenu();
    void returnBookMenu();
    std::string generateCurrentDateTime();
    std::string generateDueDate(int days);
    std::string generateLoanID(sql::Connection* conn);
    void changeMaxBookBorrow();
    void changeBorrowingDuration();
    void settings();
    void borrowBook(User user);
    void returnBook(User user);
    int getBorrowDuration(const std::string& userRole);
    void updateOverdueStatus();

private:

    dbConnection* db;  // Pointer to the database connection
    
};

#endif  // LIBRARY_H
