#include <iostream>
#include <conio.h>
#include <thread>
#include "tc.h"
#include "dbConnection.h"
#include "user.h"

int main() {
    int choice;
    dbConnection db;
    User user;
    std::string userID, password;
    bool loggedIn = false;

    while (true) 
    {
        while (!loggedIn) 
        {
            if (user.userVerify(db)) 
            {
                loggedIn = true;
            }
        }

        do 
        {
            system("cls");
            std::cout << CYAN << "Library Management System" << RESET
                << "\n1. My Profile"
                << "\n2. Borrowing"
                << "\n3. Returning"
                << "\n4. Reporting"
                << "\n5. User Management"
                << "\n0. Log Out"
                << "\n---------------------------"
                << std::endl;
            std::cout << "Enter your choice: ";
            std::cin >> choice;

            switch (choice) {
            case 1:
                user.userProfile();
                break;
            case 2:
                // Borrowing module;
                break;
            case 3:
                // Returning Book module;
                break;
            case 4:
                // Report;
                break;
            case 5:
                // User Management;
                break;
            case 0: {
                std::string exitpass;
                std::cout << "\n\nARE YOU SURE?\n" << "PLEASE TYPE \"YES\" TO LOG OUT:\n";
                std::cin >> exitpass;

                if (exitpass == "YES") {
                    std::cout << "\nLogging out..." << std::endl;
                    std::this_thread::sleep_for(std::chrono::milliseconds(500));
                    system("cls");
                    std::cout << "Thank you for using the Library System!" << std::endl;
                    std::this_thread::sleep_for(std::chrono::seconds(1));

                    loggedIn = false;
                }
                break;
            }
            default:
                std::cout << "\nInvalid Input, please try again..." << std::endl;
                std::this_thread::sleep_for(std::chrono::milliseconds(500));
                system("cls");
                break;
            }

        } while (loggedIn);
    
        main();
    }

    return 0;
}
