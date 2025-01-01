#include <iostream>
#include <conio.h>
#include <thread>
#include <stdlib.h>
#include "dbConnection.h"
#include "user.h"
#include "tc.h"

void displayMainMenu(int selected) {
    std::cout << CYAN << "Library Management System" << RESET << std::endl;

    std::cout << "\nModule: " << std::endl;
    std::cout << (selected == 0 ? "-> " : "   ") << (selected == 0 ? YELLOW : "") << "My Profile" << RESET << std::endl;
    std::cout << (selected == 1 ? "-> " : "   ") << (selected == 1 ? YELLOW : "") << "Borrowing" << RESET << std::endl;
    std::cout << (selected == 2 ? "-> " : "   ") << (selected == 2 ? YELLOW : "") << "Returning" << RESET << std::endl;
    std::cout << (selected == 3 ? "-> " : "   ") << (selected == 3 ? YELLOW : "") << "Reporting" << RESET << std::endl;
    std::cout << (selected == 4 ? "-> " : "   ") << (selected == 4 ? YELLOW : "") << "Library" << RESET << std::endl;
    std::cout << (selected == 5 ? "-> " : "   ") << (selected == 5 ? YELLOW : "") << "User Management" << RESET << std::endl;

    std::cout << "\n\n\nUse arrow keys to navigate, press Enter to select, or press Esc/q to quit.\n";
}

int main() {
    int choice;
    dbConnection db;
    User user;
    std::string userID, password;
    bool loggedIn = false;
    int selected = 0;  // Keeps track of which option is selected.
    bool selecting = true;

    while (true) {
        // User login verification (you can define userVerify logic in the User class)
        while (!loggedIn) {
            if (user.userVerify(db)) {
                loggedIn = true;
            }
        }

        // Main menu loop
        do {
            system("cls");
            // Display the menu with arrow navigation
            displayMainMenu(selected);

            // Capture user input for navigation
            char c = _getch(); // Use _getch() to get key press without waiting for enter.
            std::string exitpass;
            switch (c) {
            case KEY_UP:
                selected = (selected - 1 + 7) % 7; // Wrap around to the last option if at the top.
                break;
            case KEY_DOWN:
                selected = (selected + 1) % 7; // Wrap around to the first option if at the bottom.
                break;
            case KEY_ENTER:
                switch (selected) {
                case 0:
                    user.userProfile(); // User Profile
                    break;
                case 1:
                    // Borrowing module
                    break;
                case 2:
                    // Returning Book module
                    break;
                case 3:
                    // Report
                    break;
                case 4:
                    // Library Inventory
                    break;
                case 5:
                    // User Management
                    break;
                case 6: {

                }
                default:
                    std::cout << "\nInvalid Input, please try again..." << std::endl;
                    break;
                }
                break;
                case KEY_ESC:
                std::cout << "\n\nARE YOU SURE?\n" << "PLEASE TYPE \"YES\" TO LOG OUT:\n";
                std::cin >> exitpass;

                if (exitpass == "YES") 
                {
                    std::cout << "\nLogging out..." << std::endl;
                    std::this_thread::sleep_for(std::chrono::milliseconds(500));
                    system("cls");
                    std::cout << "Thank you for using the Library System!" << std::endl;
                    std::this_thread::sleep_for(std::chrono::seconds(1));

                    loggedIn = false;
                }
                break;
            }

            std::this_thread::sleep_for(std::chrono::milliseconds(1));
        } while (selecting);
    }

    return 0;
}
