#include "Menu.h"
#include "dbConnection.h"
#include "Report.h"
#include "Library.h"

void adminMenu(User loggingUser)
{
    dbConnection* db = new dbConnection();
    Book book(db);
    Library library(db);
    Report report;
    std::string userID, password;
    bool loggedIn = false;
    int selected = 0;  // Keeps track of which option is selected.
    bool selecting = true;
    std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
        // Main menu loop
    do {

        system("cls");
        std::cout << CYAN << "Welcome to Library Management System, " << BOLD << loggingUser.getName() << RESET << "!" << std::endl;
        std::cout << "\nModule: " << std::endl;
        std::cout << (selected == 0 ? "-> " : "   ") << (selected == 0 ? BG_YELLOW : "") << "My Profile" << RESET << std::endl;
        std::cout << (selected == 1 ? "-> " : "   ") << (selected == 1 ? BG_YELLOW : "") << "Borrowing / Returning" << RESET << std::endl;
        std::cout << (selected == 2 ? "-> " : "   ") << (selected == 2 ? BG_YELLOW : "") << "Reporting" << RESET << std::endl;
        std::cout << (selected == 3 ? "-> " : "   ") << (selected == 3 ? BG_YELLOW : "") << "Book Library Management" << RESET << std::endl;
        std::cout << (selected == 4 ? "-> " : "   ") << (selected == 4 ? BG_YELLOW : "") << "User Management" << RESET << std::endl;
        std::cout << (selected == 5 ? "-> " : "   ") << (selected == 5 ? BG_YELLOW : "") << "Settings" << RESET << std::endl;

        std::cout << "\n\n\nUse arrow keys to navigate, press Enter to select, or press Esc to quit.\n";

        // Capture user input for navigation

        char c = _getch(); // Use _getch() to get key press without waiting for enter.
        std::string exitpass;
        switch (c) {
        case KEY_UP:
            selected = (selected - 1 + 6) % 6; // Wrap around to the last option if at the top.
            break;
        case KEY_DOWN:
            selected = (selected + 1) % 6; // Wrap around to the first option if at the bottom.
            break;
        case KEY_ENTER:
            switch (selected) {
            case 0:
                loggingUser.userProfile(loggingUser);
                break;
            case 1:
                library.inputUserData();
                break;
            case 2:
                report.reportingMenu();
                break;
            case 3:
                book.libraryManagementMenu();
                break;
            case 4:
                loggingUser.userManagementMenu();
                break;
            case 5:
                library.settings();
                break;
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
                selecting = false;
            }
            break;
        }

        std::this_thread::sleep_for(std::chrono::milliseconds(1));
    } while (selecting);

    return;
}

void staffMenu(User loggingUser)
{
    dbConnection* db = new dbConnection();
    Book book(db);
    Library library(db);
    Report report;
    std::string userID, password;
    bool loggedIn = false;
    int selected = 0;  // Keeps track of which option is selected.
    bool selecting = true;
    std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
        // Main menu loop
    do {

        system("cls");
        std::cout << CYAN << "Welcome to Library Management System, " << BOLD << loggingUser.getName()  << RESET << "!" << std::endl;
        std::cout << "\nModule: " << std::endl;
        std::cout << (selected == 0 ? "-> " : "   ") << (selected == 0 ? BG_YELLOW : "") << "My Profile" << RESET << std::endl;
        std::cout << (selected == 1 ? "-> " : "   ") << (selected == 1 ? BG_YELLOW : "") << "Borrowing / Returning" << RESET << std::endl;
        std::cout << (selected == 2 ? "-> " : "   ") << (selected == 2 ? BG_YELLOW : "") << "Reporting" << RESET << std::endl;
        std::cout << (selected == 3 ? "-> " : "   ") << (selected == 3 ? BG_YELLOW : "") << "Book Library Management" << RESET << std::endl;

        std::cout << "\n\n\nUse arrow keys to navigate, press Enter to select, or press Esc to quit.\n";

        // Capture user input for navigation
        char c = _getch(); // Use _getch() to get key press without waiting for enter.
        std::string exitpass;
        switch (c) {
        case KEY_UP:
            selected = (selected - 1 + 4) % 4; // Wrap around to the last option if at the top.
            break;
        case KEY_DOWN:
            selected = (selected + 1) % 4; // Wrap around to the first option if at the bottom.
            break;
        case KEY_ENTER:
            switch (selected) {
            case 0:
                loggingUser.userProfile(loggingUser);
                break;
            case 1:
                library.inputUserData();
                break;
            case 2:
                report.reportingMenu();
                break;
            case 3:
                book.libraryManagementMenu();
                break;
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
                selecting = false;
            }
            break;
        }

        std::this_thread::sleep_for(std::chrono::milliseconds(1));
    } while (selecting);
return;
}


void userMenu(User loggingUser)
{
    dbConnection* db = new dbConnection();
    Book book(db);
    Library library(db);
    Report report;
    std::string userID, password;
    bool loggedIn = false;
    int selected = 0;  // Keeps track of which option is selected.
    bool selecting = true;
    std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
    // Main menu loop
    do {

        system("cls");
        std::cout << CYAN << "Welcome to Library Management System, " << BOLD << loggingUser.getName() << RESET << "!" << std::endl;
        std::cout << "\nModule: " << std::endl;
        std::cout << (selected == 0 ? "-> " : "   ") << (selected == 0 ? BG_YELLOW : "") << "My Profile" << RESET << std::endl;
        std::cout << (selected == 1 ? "-> " : "   ") << (selected == 1 ? BG_YELLOW : "") << "Find Books" << RESET << std::endl;

        std::cout << "\n\n\nUse arrow keys to navigate, press Enter to select, or press Esc to quit.\n";

        // Capture user input for navigation
        char c = _getch(); // Use _getch() to get key press without waiting for enter.
        std::string exitpass;
        switch (c) {
        case KEY_UP:
            selected = (selected - 1 + 2) % 2; // Wrap around to the last option if at the top.
            break;
        case KEY_DOWN:
            selected = (selected + 1) % 2; // Wrap around to the first option if at the bottom.
            break;
        case KEY_ENTER:
            switch (selected) {
            case 0:
                loggingUser.userProfile(loggingUser);
                break;
            case 1:
                book.searchBook();
                break;
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
                selecting = false;
            }
            break;
        }

        std::this_thread::sleep_for(std::chrono::milliseconds(1));
    } while (selecting);

    return;
}