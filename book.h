#ifndef BOOK
#define BOOK
#include <iostream>
#include <thread>

#include "dbConnection.h"
#include "tc.h"
class Book
{
public:
	Book();
	Book(const std::string& bookID, const std::string& title, const std::string& isbn,
		const std::string& author, const std::string& publisher, const std::string& category,
		int publicationYear, int quantity, const std::string& status, double price);
	~Book();

	std::string getBookID();
	std::string getTitle();
	std::string getISBN();
	std::string getAuthor();
	std::string getPublisher();
	std::string getCategory();
	int getPublicationYear();
	int getQuantity();
	std::string getStatus();
	double getPrice();

	void setBookID(std::string bookID);
	void setTitle(std::string title);
	void setISBN(std::string isbn);
	void setAuthor(std::string author);
	void setPublisher(std::string publisher);
	void setCategory(std::string category);
	void setPublicationYear(int publicationYear);
	void setQuantity(int quantity);
	void setStatus(std::string status);
	void setPrice(double price);

	void libraryPrompt();
	void retrieveBookFromDB(std::string bookId);
	void addBook();
	std::string generateBookID(dbConnection& db);
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

	dbConnection* con;
	dbConnection db;
};
#endif
