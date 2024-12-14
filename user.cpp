#include <iostream>
#include <conio.h>
#include <thread>
#include <chrono>
#include "user.h"

User::User() {}

User::~User() {}

bool User::userLogin(dbConnection& db, std::string& username, std::string& password)
{
    while (true)
    {
        system("cls");
        std::cout << "/*User Login*/" << std::endl;
        std::cout << "Enter id: ";
        std::cin >> username;
        std::cout << "Enter password: ";
        std::cin >> password;

        if (db.login(username, password))
        {
            system("cls");
            std::cout << "Welcome, " << username << "!" << std::endl;
            std::this_thread::sleep_for(std::chrono::seconds(2));
            return true;
        }
        else
        {
            system("cls");
            std::cout << "Login failed. Please try again." << std::endl;
            std::this_thread::sleep_for(std::chrono::seconds(1));
        }
    }
}
