#ifndef LIBRARY_H
#define LIBRARY_H

#include "dbConnection.h"  // Assuming dbConnection is defined elsewhere
#include <string>

class Library
{
public:
    // Constructor that takes a dbConnection pointer
    Library(dbConnection* connection);

    // Method to borrow a book
    void borrowBookMenu();
    std::string generateDateTime();
    std::string generateDueDate(int days);
    std::string generateLoanID(sql::Connection* conn);
    void changeMaxBookBorrow();
    void settings();
private:
    std::string loanID;
    

    dbConnection* db;  // Pointer to the database connection
    
};

#endif  // LIBRARY_H
