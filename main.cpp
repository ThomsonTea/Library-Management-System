#include <iostream>
#include <conio.h>
#include <thread>
#include "tc.h"
#include "dbConnection.h"
#include "user.h"
using namespace std;

int main() {
    int choice;
    dbConnection db;
    User user;
    std::string username, password;
    bool loggedIn = false;

    while (true) 
    {
        while (!loggedIn) 
        {
            if (user.userLogin(db, username, password)) 
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
            cin >> choice;

            switch (choice) {
            case 1:
                // My profile;
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
                cin >> exitpass;

                if (exitpass == "YES") {
                    std::cout << "\nLogging out..." << std::endl;
                    std::this_thread::sleep_for(chrono::milliseconds(500));
                    system("cls");
                    std::cout << "Thank you for using the Library System!" << std::endl;
                    std::this_thread::sleep_for(chrono::seconds(1));

                    loggedIn = false;
                    break;
                }
                break;
            }
            default:
                std::cout << "\nInvalid Input, please try again..." << std::endl;
                std::this_thread::sleep_for(chrono::milliseconds(500));
                system("cls");
                break;
            }

        } while (loggedIn);
    }

    return 0;
}
