#ifndef USER_H
#define USER_H

#include <iostream>
#include <conio.h>
#include <thread>
#include <chrono>
#include <string>
#include "dbConnection.h"  // Include the dbConnection header

class User
{
public:
    User();
    ~User();

    std::string getUserID();
    std::string getName();
    std::string getIc();
    std::string getPhoneNum();
    std::string getEmail();
    std::string getAddress();
    std::string getPassword();
    std::string getRole();

    void setUserID(std::string userID);
    void setName(std::string name);
    void setIc(std::string ic);
    void setPhoneNum(std::string phoneNum);
    void setEmail(std::string email);
    void setAddress(std::string address);
    void setPassword(std::string password);
    void setRole(std::string role);

    bool userVerify(dbConnection db);
    bool isUser(const std::string& userId, const std::string& password);
    void registerUser();
    void editProfile();
    void userProfile();
    void retrieveUserFromDB(const std::string& userID);
    void changePassword();
    void userManagementMenu();
    std::string hiddenInput();

private:
    std::string userID;
    std::string name;
    std::string ic;
    std::string phoneNum;
    std::string email;
    std::string address;
    std::string password;
    std::string role;

    dbConnection* con;
    dbConnection db;
};

#endif
