#ifndef USER_LIST
#define USER_LIST

#include <iostream>
#include <list>
#include "user.h"

class UserList
{
public:
	UserList(dbConnection* connection);
	~UserList();

	void setUserList(std::list<User> userList);
	std::list<User> getUserList();
	void fetchUsersFromDB();
	void displayUsers();

private:
	std::list<User> userList;

	dbConnection* db;
};

#endif
