#ifndef STAFF_H
#define STAFF_H

#include <string>
#include <iostream>
#include <mysql_driver.h>
#include <mysql_connection.h>
#include <cppconn/statement.h>
#include <cppconn/prepared_statement.h>

class Staff {
private:
    std::string staffId;
    std::string name;
    std::string phoneNum;
    std::string email;
    std::string address;
    std::string role; // Roles: "Admin", "Manager", "User"
    std::string password;

    sql::mysql::MySQL_Driver* driver;
    sql::Connection* conn;

public:
    Staff();

    // Parameterized Constructor
    Staff(const std::string& staffId, const std::string& name, const std::string& phoneNum,
        const std::string& email, const std::string& address, const std::string& role,
        const std::string& password);

    ~Staff();

    bool addUser(const std::string& adminId, const std::string& newStaffId, const std::string& newName,
        const std::string& newPhoneNum, const std::string& newEmail, const std::string& newAddress,
        const std::string& newRole, const std::string& newPassword);

    // Update user details (Admin/Manager only)
    bool updateUserDetails(const std::string& adminId, const std::string& updateStaffId, const std::string& newName,
        const std::string& newPhoneNum, const std::string& newEmail, const std::string& newAddress,
        const std::string& newRole);

    // Renew password (Any user)
    bool renewPassword(const std::string& staffId, const std::string& newPassword);

};

#endif
