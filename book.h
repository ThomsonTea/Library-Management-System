#ifndef BOOK
#define BOOK
#include <iostream>
#include <thread>

#include "dbConnection.h"
#include "tc.h"
class Book
{
public:
	Book(dbConnection* connection);
	Book(std::string bookID, std::string title, std::string isbn,
		std::string author, std::string publisher, std::string category, int publicationYear,
		int quantity, std::string status, double price);
	~Book();

	std::string getBookID();
	std::string getTitle();
	std::string getIsbn();
	std::string getAuthor();
	std::string getPublisher();
	std::string getCategory();
	int getPublicationYear();
	int getQuantity();
	std::string getStatus();
	double getPrice();
	dbConnection* getDB();

	void setBookID(std::string bookID);
	void setTitle(std::string title);
	void setIsbn(std::string isbn);
	void setAuthor(std::string author);
	void setPublisher(std::string publisher);
	void setCategory(std::string category);
	void setPublicationYear(int publicationYear);
	void setQuantity(int quantity);
	void setStatus(std::string status);
	void setPrice(double price);

	void searchBook();
	void retrieveBookFromDB(std::string bookId);
	void addBook();
	void editBook();
	void removeBook();
	void libraryManagementMenu();
	std::string generateBookID(sql::Connection* conn);

private:
	std::string bookID;
	std::string title;
	std::string isbn;
	std::string author;
	std::string publisher;
	std::string category;
	int publicationYear;
	int quantity;
	std::string status;
	double price;

	dbConnection* db;
};
#endif
