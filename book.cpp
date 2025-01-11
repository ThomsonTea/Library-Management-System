#include <iostream>
#include <string>
#include <thread>
#include <iomanip>
#include <sstream>

#include "book.h"

Book::Book(dbConnection* connection) : db(connection) 
{
}

Book::~Book()
{}

std::string Book::getBookID()
{
    return this->bookID;
}

std::string Book::getTitle()
{
    return this->title;
}

std::string Book::getISBN()
{
    return this->isbn;
}

std::string Book::getAuthor()
{
    return this->author;
}

std::string Book::getPublisher()
{
    return this->publisher;
}

std::string Book::getCategory()
{
    return this->category;
}

int Book::getPublicationYear()
{
    return this->publicationYear;
}

int Book::getQuantity()
{
    return this->quantity;
}

std::string Book::getStatus()
{
    return this->status;
}

double Book::getPrice()
{
    return this->price;
}

void Book::setBookID(std::string bookID)
{
    this->bookID = bookID;
}

void Book::setTitle(std::string title)
{
    this->title = title;
}

void Book::setISBN(std::string isbn)
{
    this->isbn = isbn;
}

void Book::setAuthor(std::string author)
{
    this->author = author;
}

void Book::setPublisher(std::string publisher)
{
    this->publisher = publisher;
}

void Book::setCategory(std::string category)
{
    this->category = category;
}

void Book::setPublicationYear(int publicationYear)
{
    this->publicationYear = publicationYear;
}

void Book::setQuantity(int quantity)
{
    this->quantity = quantity;
}

void Book::setStatus(std::string status)
{
    this->status = status;
}

void Book::setPrice(double price)
{
    this->price = price;
}

void Book::retrieveBookFromDB(std::string bookId)
{
    if (!db->getConnection()) {  // Make sure db connection is available
        std::cerr << "No database connection available!" << std::endl;
        std::this_thread::sleep_for(std::chrono::milliseconds(500));
        return;
    }
    try
    {
        std::string query = "SELECT * FROM Book WHERE userID = ?";

        // Get the connection from dbConnection
        sql::PreparedStatement* pstmt = db->getConnection()->prepareStatement(query);
        pstmt->setString(1, bookID);          // Bind name

        sql::ResultSet* res = pstmt->executeQuery();

        if (res->next())
        {
            // Fetch data from the result set
            setBookID(res->getString("bookID"));
            setTitle(res->getString("title"));
            setISBN(res->getString("isbn"));
            setAuthor(res->getString("author"));
            setPublisher(res->getString("publisher"));
            setCategory(res->getString("category"));
            setPublicationYear(res->getInt("publicationYear"));
            setQuantity(res->getInt("quantity"));
            setStatus(res->getString("status"));
            setPrice(res->getDouble("price"));
        }
        else {
            std::cerr << "No user found with the provided userID." << std::endl;
        }

        // Clean up
        delete res;
        delete pstmt;
    }
    catch (sql::SQLException& e)
    {
        std::cerr << "Error executing query: " << e.what() << std::endl;
    }
}

void Book::libraryPrompt()
{
    int selected = 0;  // Keeps track of which option is selected.
    bool selecting = true;
    std::string query = "SELECT bookID, isbn, title, category, author, publisher FROM Book";
    do
    {
        system("cls");
        std::string data;

        std::cout << CYAN << "Welcome to Library !\n" << RESET << std::endl;
        std::cout << "Search:" << std::endl;
        std::cout << (selected == 0 ? "-> " : "   ") << (selected == 0 ? BG_YELLOW : "") << "ID: " << RESET << std::endl;
        std::cout << (selected == 1 ? "-> " : "   ") << (selected == 1 ? BG_YELLOW : "") << "Title: " << RESET << std::endl;
        std::cout << (selected == 2 ? "-> " : "   ") << (selected == 2 ? BG_YELLOW : "") << "ISBN: " << RESET << std::endl;
        std::cout << (selected == 3 ? "-> " : "   ") << (selected == 3 ? BG_YELLOW : "") << "Author: " << RESET << std::endl;
        std::cout << (selected == 4 ? "-> " : "   ") << (selected == 4 ? BG_YELLOW : "") << "Publisher: " << RESET << std::endl;
        std::cout << (selected == 5 ? "-> " : "   ") << (selected == 5 ? BG_YELLOW : "") << "Category: " << RESET << std::endl;

        std::cout << "\n\n\nUse arrow keys to navigate, press Enter to select, press R to reset searching, or press Esc to quit.\n";

        db->fetchAndDisplayData(query);


        char c = _getch(); // Use _getch() to get key press without waiting for enter.
        switch (c) {
        case KEY_UP:
            selected = (selected - 1 + 6) % 6; // Wrap around to the last option if at the top.
            break;
        case KEY_DOWN:
            selected = (selected + 1) % 6; // Wrap around to the first option if at the bottom.
            break;
        case KEY_UPPER_R:
        case KEY_LOWER_R:
            query = "SELECT bookID, isbn, title, category, author, publisher FROM Book"; // Reset the searching
            break;
        case KEY_ENTER:
            switch (selected) {
            case 0:
                std::cout << "\x1b[4;8H";
                std::cin >> data;
                query = "SELECT bookID,isbn, title, category, author, publisher FROM Book WHERE bookID LIKE '" + data + "'";
                break;
            case 1:
                std::cout << "\x1b[5;11H";
                std::cin >> data;
                query = "SELECT bookID,isbn, title, category, author, publisher FROM Book WHERE LIKE '" + data + "'";
                break;
            case 2:
                std::cout << "\x1b[6;10H";
                std::cin >> data;
                query = "SELECT bookID,isbn, title, category, author, publisher FROM Book WHERE LIKE '" + data + "'";
                break;
            case 3:
                std::cout << "\x1b[7;11H";
                std::cin >> data;
                query = "SELECT bookID,isbn, title, category, author, publisher FROM Book WHERE LIKE '" + data + "'";
                break;
            case 4:
                std::cout << "\x1b[8;14H";
                std::cin >> data;
                query = "SELECT bookID,isbn, title, category, author, publisher FROM Book WHERE LIKE '" + data + "'";
                break;
            case 5:
                std::cout << "\x1b[9;13H";
                std::cin >> data;
                query = "SELECT bookID,isbn, title, category, author, publisher FROM Book WHERE LIKE '" + data + "'";
                break;
            default:
                std::cout << "\nInvalid Input, please try again..." << std::endl;
                break;
            }
            break;
        case KEY_ESC:
            selecting = false;
            break;
        }

        std::this_thread::sleep_for(std::chrono::milliseconds(1));
    } while (selecting);
}

void Book::addBook()
{
    try
    {
        system("cls");
        std::string title, isbn, author, publisher, category, publicationYear, status;
        int quantity;
        double price;

        std::cout << CYAN << "Add Book" << RESET << std::endl;
        std::cout << "Enter the Book Data:" << std::endl;

        std::cout << "Title: ";
        std::cin.ignore();
        std::getline(std::cin, title);

        std::cout << "ISBN: ";
        std::getline(std::cin, isbn);

        std::cout << "Author: ";
        std::getline(std::cin, author);

        std::cout << "Publisher: ";
        std::getline(std::cin, publisher);

        std::cout << "Category: ";
        std::getline(std::cin, category);

        std::cout << "Publication Year: ";
        std::getline(std::cin, publicationYear);

        std::cout << "Quantity: ";
        while (!(std::cin >> quantity)) {
            std::cout << "Invalid input. Enter a valid number for Quantity: ";
            std::cin.clear();
            std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
        }

        std::cout << "Status [0 for Unavailable, 1 for Available]: ";
        int statusInput;
        while (!(std::cin >> statusInput) || (statusInput != 0 && statusInput != 1)) {
            std::cout << "Invalid input. Enter 0 for Unavailable or 1 for Available: ";
            std::cin.clear();
            std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
        }
        status = (statusInput == 0) ? "Unavailable" : "Available";

        std::cout << "Price (RM): ";
        while (!(std::cin >> price)) {
            std::cout << "Invalid input. Enter a valid price: ";
            std::cin.clear();
            std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
        }

        std::string bookID = generateBookID();

        std::string query = "INSERT INTO Book (bookID, isbn, title, category, author, publisher, publicationYear, quantity, status, price) "
            "VALUES (?, ?, ?, ?, ?, ?, ?, ?, ?, ?)";
        std::unique_ptr<sql::PreparedStatement> pstmt(db->getConnection()->prepareStatement(query));

        pstmt->setString(1, bookID);
        pstmt->setString(2, isbn);
        pstmt->setString(3, title);
        pstmt->setString(4, category);
        pstmt->setString(5, author);
        pstmt->setString(6, publisher);
        pstmt->setString(7, publicationYear);
        pstmt->setInt(8, quantity);
        pstmt->setString(9, status);
        pstmt->setDouble(10, price);

        pstmt->executeUpdate();

        std::cout << "Book added successfully with bookID: " << bookID << std::endl;
    }
    catch (sql::SQLException& e)
    {
        std::cerr << "Error adding book: " << e.what() << std::endl;
        std::this_thread::sleep_for(std::chrono::milliseconds(1));
    }
    
}

std::string Book::generateBookID()
{
    std::string query = "SELECT bookID FROM Book ORDER BY bookID DESC LIMIT 1";
    std::unique_ptr<sql::PreparedStatement> pstmt(db->getConnection()->prepareStatement(query));
    std::unique_ptr<sql::ResultSet> res(pstmt->executeQuery());

    if (res->next()) {
        std::string lastBookID = res->getString("bookID");
        int lastIDNumber = std::stoi(lastBookID.substr(1));
        int newIDNumber = lastIDNumber + 1;

        std::ostringstream oss;
        oss << "B" << std::setw(3) << std::setfill('0') << newIDNumber;
        return oss.str();
    }
    return "B001";  // Default if no books are present
}