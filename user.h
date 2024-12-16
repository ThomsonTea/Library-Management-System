#ifndef USER_H
#define USER_H

#include <iostream>
#include <string>
#include "dbConnection.h"  // Include the dbConnection header

class User
{
public:
    User();
    ~User();

    bool userVerify(dbConnection& db, std::string& username, std::string& password);
    bool isUser(const std::string& userId, const std::string& password);

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
