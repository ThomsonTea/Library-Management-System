#include "User.h"
#include "Menu.h"
#include "Library.h"
#include "dbConnection.h"
#include <iostream>
#include "Menu.h"

int main()
{
    dbConnection* db = new dbConnection();
    User loggingUser(db);
    Library library(db);

    // Loop for the entire system to allow re-login after logout.
    while (true)
    {
        library.updateOverdueStatus();
        // Login process
        if (!loggingUser.userVerify())
        {
            std::cout << "Login failed. Exiting system." << std::endl;
            break;
        }

        // Retrieve user information from the database after successful login.
        loggingUser.retrieveUserFromDB(loggingUser.getUserID());

        // Determine user role and redirect to the appropriate menu.
        std::string userRole = loggingUser.getRole(); // Assumes `getRole()` returns the role as a string.

        if (userRole == "Admin")
        {
            adminMenu(loggingUser); // Call the admin menu.
        }
        else if (userRole == "Staff")
        {
            staffMenu(loggingUser); // Call the staff menu.
        }
        else if (userRole == "Patron")
        {
            userMenu(loggingUser); // Call the user menu.
        }
        else
        {
            std::cerr << "Error: Invalid user role detected. Exiting system." << std::endl;
            break;
        }
    }

    delete db; // Clean up the database connection before exiting.
    return 0;
}
