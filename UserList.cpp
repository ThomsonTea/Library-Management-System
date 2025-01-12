#include <iostream>
#include <list>
#include "table.h"
#include "UserList.h"
#include "user.h"

using namespace tabulate;

UserList::UserList(dbConnection* connection) : db(connection)
{
    fetchUsersFromDB();
}

UserList::~UserList() {}

void UserList::setUserList(std::list<User> userList)
{
    this->userList = userList;
}

std::list<User> UserList::getUserList()
{
    return userList;
}

void UserList::fetchUsersFromDB()
{
    try {
        std::list<User> users;
        sql::PreparedStatement* pstmt = db->getConnection()->prepareStatement(
            "SELECT userID, name, ic, phoneNum, email, address, password, role FROM user");
        sql::ResultSet* res = pstmt->executeQuery();

        // Add users to the vector
        while (res->next()) {
            User user(
                res->getString("userID"),
                res->getString("name"),
                res->getString("ic"),
                res->getString("phoneNum"),
                res->getString("email"),
                res->getString("address"),
                res->getString("password"),
                res->getString("role")
            );
            users.push_back(user);
        }
        setUserList(users);
        delete pstmt;
        delete res;

    }
    catch (sql::SQLException& e) {
        std::cerr << "Error fetching users: " << e.what() << std::endl;
    }
}

void UserList::displayUsers()
{
    Table tableUserList;
    tableUserList.add_row({ "ID", "Name", "IC", "Phone Number", "Email", "Address", "Password", "Role" });

    for (User user : getUserList())
    {
        tableUserList.add_row({ user.getUserID(), user.getName(), user.getIc(), user.getPhoneNum(), user.getEmail(), user.getAddress(), user.getPassword(), user.getRole()});
    }
    tableFormat(tableUserList);

    std::cout << tableUserList << std::endl;
}