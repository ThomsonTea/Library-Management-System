#include <iostream>

// Declare the function defined in sql_connection.cpp
extern void setupDatabase();

int main()
{
    std::cout << "Starting the Library Management System..." << std::endl;

    // Call the function to set up the database
    setupDatabase();

    std::cout << "Database setup complete." << std::endl;
    return 0;
}
