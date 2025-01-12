#include <iostream>
#include <string>
#include <thread>
#include <iomanip>
#include <sstream>
#include <format>
#include "tabulate/table.hpp"
#include "table.h"
#include "book.h"
#include "validation.h"

Book::Book(dbConnection* connection)
{
    this->db = connection;
}

Book::Book(std::string bookID, std::string title, std::string isbn,
    std::string author, std::string publisher, std::string category, int publicationYear,
    int quantity, std::string status, double price)
{
    this->bookID = bookID;
    this->title = title;
    this->isbn = isbn;
    this->author = author;
    this->publisher = publisher;
    this->category = category;
    this->publicationYear = publicationYear;
    this->quantity = quantity;
    this->status = status;
    this->price = price;
}

Book::~Book()
{}

std::string Book::getBookID()
{
    return bookID;
}

std::string Book::getTitle()
{
    return title;
}

std::string Book::getISBN()
{
    return isbn;
}

std::string Book::getAuthor()
{
    return author;
}

std::string Book::getPublisher()
{
    return publisher;
}

std::string Book::getCategory()
{
    return category;
}

int Book::getPublicationYear()
{
    return publicationYear;
}

int Book::getQuantity()
{
    return quantity;
}

std::string Book::getStatus()
{
    return status;
}

double Book::getPrice()
{
    return price;
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

void Book::searchBook()
{
    int selected = 0;  // Keeps track of which option is selected.
    bool selecting = true;
    std::string query = "SELECT bookID, isbn, title, author, publisher, category, publicationYear, quantity, price, status FROM Book LIMIT 10";

    do
    {
        system("cls");
        std::string data;

        std::cout << CYAN << "Welcome to Library !\n" << RESET << std::endl;
        std::cout << "Search By:" << std::endl;
        std::cout << (selected == 0 ? "-> " : "   ") << (selected == 0 ? BG_YELLOW : "") << "ID: " << RESET << std::endl;
        std::cout << (selected == 1 ? "-> " : "   ") << (selected == 1 ? BG_YELLOW : "") << "Title: " << RESET << std::endl;
        std::cout << (selected == 2 ? "-> " : "   ") << (selected == 2 ? BG_YELLOW : "") << "ISBN: " << RESET << std::endl;
        std::cout << (selected == 3 ? "-> " : "   ") << (selected == 3 ? BG_YELLOW : "") << "Author: " << RESET << std::endl;
        std::cout << (selected == 4 ? "-> " : "   ") << (selected == 4 ? BG_YELLOW : "") << "Publisher: " << RESET << std::endl;
        std::cout << (selected == 5 ? "-> " : "   ") << (selected == 5 ? BG_YELLOW : "") << "Category: " << RESET << std::endl;

        std::cout << "\n\n\nUse arrow keys to navigate, press Enter to select, press R to reset searching, or press Esc to quit.\n";

        db->fetchAndDisplayData(query);  // This should display data in a table format.

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
            query = "SELECT bookID, isbn, title, author, publisher, category, publicationYear, quantity, price, status FROM Book LIMIT 10"; // Reset the search
            break;
        case KEY_ENTER:
            switch (selected) {
            case 0:
                std::cout << "\x1b[4;8H";
                std::cin >> data;
                query = "SELECT bookID, isbn, title, author, publisher, category, publicationYear, quantity, price, status FROM Book WHERE bookID LIKE '%" + data + "%'";
                break;
            case 1:
                std::cout << "\x1b[5;11H";
                std::cin >> data;
                query = "SELECT bookID, isbn, title, author, publisher, category, publicationYear, quantity, price, status FROM Book WHERE title LIKE '%" + data + "%'";
                break;
            case 2:
                std::cout << "\x1b[6;10H";
                std::cin >> data;
                query = "SELECT bookID, isbn, title, author, publisher, category, publicationYear, quantity, price, status FROM Book WHERE isbn LIKE '%" + data + "%'";
                break;
            case 3:
                std::cout << "\x1b[7;12H";
                std::cin >> data;
                query = "SELECT bookID, isbn, title, author, publisher, category, publicationYear, quantity, price, status FROM Book WHERE author LIKE '%" + data + "%'";
                break;
            case 4:
                std::cout << "\x1b[8;14H";
                std::cin >> data;
                query = "SELECT bookID, isbn, title, author, publisher, category, publicationYear, quantity, price, status FROM Book WHERE publisher LIKE '%" + data + "%'";
                break;
            case 5:
                std::cout << "\x1b[9;13H";
                std::cin >> data;
                query = "SELECT bookID, isbn, title, author, publisher, category, publicationYear, quantity, price, status FROM Book WHERE category LIKE '%" + data + "%'";
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


std::string Book::generateBookID(sql::Connection* conn)
{
    try {
        if (!conn) {
            throw std::runtime_error("Database connection not available");
        }

        std::string query = "SELECT bookID FROM Book ORDER BY bookID DESC LIMIT 1";

        std::unique_ptr<sql::PreparedStatement> pstmt(conn->prepareStatement(query));

        if (!pstmt) {
            throw std::runtime_error("Failed to create prepared statement");
        }

        std::unique_ptr<sql::ResultSet> res(pstmt->executeQuery());

        if (!res) {
            throw std::runtime_error("Failed to execute query");
        }

        if (res->next()) {
            std::string lastBookID = res->getString("bookID");

            // Check if the bookID follows the format B########
            if (lastBookID.length() != 9 || lastBookID[0] != 'B') {
                throw std::runtime_error("Invalid book ID format in database");
            }

            try {
                // Extract the number part (8 digits)
                int lastIDNumber = std::stoi(lastBookID.substr(1));
                int newIDNumber = lastIDNumber + 1;

                // Format with 8 digits (changed from 3 to 8)
                std::ostringstream oss;
                oss << "B" << std::setw(8) << std::setfill('0') << newIDNumber;
                return oss.str();
            }
            catch (const std::exception& e) {
                throw std::runtime_error("Failed to parse book ID number: " + std::string(e.what()));
            }
        }

        return "B00000001";  // Default first ID if no books are present
    }
    catch (const sql::SQLException& e) {
        throw std::runtime_error("Database error in generateBookID: " + std::string(e.what()));
    }
    catch (const std::exception& e) {
        throw std::runtime_error("Error in generateBookID: " + std::string(e.what()));
    }
}

void Book::libraryManagementMenu()
{
    int selected = 0;  // Keeps track of which option is selected.
    bool selecting = true;

    do {
        system("cls");
        std::cout << CYAN << "Library Management System" << RESET << std::endl;

        std::cout << "\nModule: " << std::endl;
        std::cout << (selected == 0 ? "-> " : "   ") << (selected == 0 ? BG_YELLOW : "") << "Search Book" << RESET << std::endl;
        std::cout << (selected == 1 ? "-> " : "   ") << (selected == 1 ? BG_YELLOW : "") << "Register New Book" << RESET << std::endl;
        std::cout << (selected == 2 ? "-> " : "   ") << (selected == 2 ? BG_YELLOW : "") << "Edit Book" << RESET << std::endl;
        std::cout << (selected == 3 ? "-> " : "   ") << (selected == 3 ? BG_YELLOW : "") << "Remove Book" << RESET << std::endl;

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
            switch (selected)
            {
            case 0:
                searchBook();
                break;
            case 1:
                addBook();
                break;
            case 2:
                //editBook();
                break;
            case 3:
                //removeBook();
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
        std::unique_ptr<dbConnection> db = std::make_unique<dbConnection>();
        sql::Connection* conn = db->getConnection();

        if (!conn) {
            std::cerr << "Failed to establish database connection!" << std::endl;
            system("pause");
            return;
        }

        Book newBook("", "", "", "", "", "", 0, 0, "", 0.00);
        int selected = 0;
        bool selecting = true;

        do
        {
            system("cls");
            std::string data;
            int intData = 0;
            std::cout << CYAN << "Welcome to Library Management System" << RESET << std::endl;
            std::cout << CYAN << "\nInfo of New Book" << RESET << std::endl;

            tabulate::Table table;
            table.add_row({ "Title", newBook.getTitle() });
            table.add_row({ "ISBN", newBook.getISBN() });
            table.add_row({ "Author", newBook.getAuthor() });
            table.add_row({ "Publisher", newBook.getPublisher() });
            table.add_row({ "Category", newBook.getCategory() });
            table.add_row({ "Publication Year", std::to_string(newBook.getPublicationYear()) });
            table.add_row({ "Quantity", std::to_string(newBook.getQuantity()) });
            table.add_row({ "Price", std::format("{:.2f}", newBook.getPrice()) });
            table.add_row({ "Status", newBook.getStatus() });

            paraTableFormat(table);

            std::cout << "Insert Data:" << std::endl;
            std::cout << (selected == 0 ? "-> " : "   ") << (selected == 0 ? BG_YELLOW : "") << "Title: " << RESET << std::endl;
            std::cout << (selected == 1 ? "-> " : "   ") << (selected == 1 ? BG_YELLOW : "") << "ISBN: " << RESET << std::endl;
            std::cout << (selected == 2 ? "-> " : "   ") << (selected == 2 ? BG_YELLOW : "") << "Author: " << RESET << std::endl;
            std::cout << (selected == 3 ? "-> " : "   ") << (selected == 3 ? BG_YELLOW : "") << "Publisher: " << RESET << std::endl;
            std::cout << (selected == 4 ? "-> " : "   ") << (selected == 4 ? BG_YELLOW : "") << "Category: " << RESET << std::endl;
            std::cout << (selected == 5 ? "-> " : "   ") << (selected == 5 ? BG_YELLOW : "") << "Publication Year: " << RESET << std::endl;
            std::cout << (selected == 6 ? "-> " : "   ") << (selected == 6 ? BG_YELLOW : "") << "Quantity: " << RESET << std::endl;
            std::cout << (selected == 7 ? "-> " : "   ") << (selected == 7 ? BG_YELLOW : "") << "Price(RM): " << RESET << std::endl;
            std::cout << (selected == 8 ? "-> " : "   ") << (selected == 8 ? BG_YELLOW : "") << "Status[1 for Available, 0 for Unavailable]: " << RESET << std::endl;
            std::cout << (selected == 9 ? "-> " : "   ") << (selected == 9 ? BG_GREEN : "") << "Save and Exit " << RESET << std::endl;

            std::cout << "\n\n\nUse arrow keys to navigate, press Enter to select, or press Esc to quit.\n";

            char c = _getch();

            switch (c) {
            case KEY_UP:
                selected = (selected - 1 + 10) % 10;
                break;
            case KEY_DOWN:
                selected = (selected + 1) % 10;
                break;
            case KEY_ENTER:
                switch (selected) {
                case 0:
                    std::cout << "\x1b[24;11H";
                    std::getline(std::cin, data);
                    newBook.setTitle(data);
                    break;
                case 1:
                    std::cout << "\x1b[25;10H";
                    std::getline(std::cin, data);
                    newBook.setISBN(data);
                    break;
                case 2:
                    std::cout << "\x1b[26;12H";
                    std::getline(std::cin, data);
                    newBook.setAuthor(data);
                    break;
                case 3:
                    std::cout << "\x1b[27;15H";
                    std::getline(std::cin, data);
                    newBook.setPublisher(data);
                    break;
                case 4:
                    std::cout << "\x1b[28;14H";
                    std::getline(std::cin, data);
                    newBook.setCategory(data);
                    break;
                case 5:
                    std::cout << "\x1b[29;22H";
                    std::getline(std::cin, data);
                    if (isNumber(data)) {
                        newBook.setPublicationYear(std::stoi(data));
                    }
                    else {
                        std::cout << "\x1b[29;22H" << RED << "Invalid input. Please enter a valid number for Publication Year." << RESET << std::endl;
                        std::this_thread::sleep_for(std::chrono::seconds(2));
                    }
                    break;
                case 6:
                    std::cout << "\x1b[30;14H";
                    std::getline(std::cin, data);
                    if (isNumber(data)) {
                        newBook.setQuantity(std::stoi(data));
                    }
                    else {
                        std::cout << "\x1b[30;14H" << RED << "Invalid input. Please enter a valid number for Quantity." << RESET << std::endl;
                        std::this_thread::sleep_for(std::chrono::seconds(2));
                    }
                    break;
                case 7:
                    std::cout << "\x1b[31;15H";
                    std::getline(std::cin, data);
                    if (isNumber(data)) {
                        std::ostringstream oss;
                        oss << std::fixed << std::setprecision(2) << std::stod(data);
                        newBook.setPrice(std::stod(oss.str()));
                    }
                    else {
                        std::cout << "\x1b[31;15H" << RED << "Invalid input. Please enter a valid number for Price." << RESET << std::endl;
                        std::this_thread::sleep_for(std::chrono::seconds(2));
                    }
                    break;
                case 8:
                    std::cout << "\x1b[32;48H";
                    std::cin >> intData;
                    std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
                    if (intData == 1) {
                        newBook.setStatus("Available");
                    }
                    else if (intData == 0) {
                        newBook.setStatus("Unavailable");
                    }
                    else {
                        std::cout << RED << "Invalid input. Please enter 1 for Available or 0 for Unavailable." << RESET << std::endl;
                        std::this_thread::sleep_for(std::chrono::seconds(2));
                    }
                    break;
                case 9:
                    if (isDataEmpty(newBook.getTitle()) || isDataEmpty(newBook.getISBN()) ||
                        isDataEmpty(newBook.getAuthor()) || isDataEmpty(newBook.getPublisher()) ||
                        isDataEmpty(newBook.getCategory()) || newBook.getPublicationYear() == 0 ||
                        newBook.getQuantity() == 0 || newBook.getPrice() == 0.0 ||
                        isDataEmpty(newBook.getStatus())) {
                        std::cout << RED << "Please fill in all the fields before saving." << RESET << std::endl;
                        std::this_thread::sleep_for(std::chrono::seconds(2));
                    }
                    else {
                        try {
                            std::string newBookId = newBook.generateBookID(conn);
                            newBook.setBookID(newBookId);

                            std::string query = "INSERT INTO Book (bookID, title, isbn, author, publisher, "
                                "category, publicationYear, quantity, price, status) "
                                "VALUES (?, ?, ?, ?, ?, ?, ?, ?, ?, ?)";

                            std::unique_ptr<sql::PreparedStatement> pstmt(conn->prepareStatement(query));

                            pstmt->setString(1, newBook.getBookID());
                            pstmt->setString(2, newBook.getTitle());
                            pstmt->setString(3, newBook.getISBN());
                            pstmt->setString(4, newBook.getAuthor());
                            pstmt->setString(5, newBook.getPublisher());
                            pstmt->setString(6, newBook.getCategory());
                            pstmt->setInt(7, newBook.getPublicationYear());
                            pstmt->setInt(8, newBook.getQuantity());
                            pstmt->setDouble(9, newBook.getPrice());
                            pstmt->setString(10, newBook.getStatus());

                            pstmt->executeUpdate();

                            std::cout << GREEN << "Book added successfully with Book ID: "
                                << RESET << newBook.getBookID() << std::endl;
                            std::this_thread::sleep_for(std::chrono::seconds(3));
                            selecting = false;
                        }
                        catch (const std::exception& e) {
                            std::cerr << RED << "Error during save: " << e.what() << RESET << std::endl;
                            system("pause");
                        }
                    }
                    break;
                }
                break;
            case KEY_ESC:
                selecting = false;
                break;
            }
        } while (selecting);
    }
    catch (const sql::SQLException& e) {
        std::cerr << RED << "SQL Error: " << e.what() << std::endl;
        std::cerr << "Error code: " << e.getErrorCode() << std::endl;
        std::cerr << "SQLState: " << e.getSQLState() << RESET << std::endl;
        system("pause");
    }
    catch (const std::exception& e)
    {
        std::cerr << RED << "Error: " << e.what() << RESET << std::endl;
        system("pause");
    }
}