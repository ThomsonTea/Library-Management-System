// Fine.h
#ifndef FINE_H
#define FINE_H

#include <string>
#include "dbConnection.h"
#include "user.h"
#include "book.h"
class Fine
{
private:
    dbConnection* db; // Database connection object

    std::string fineID;
    std::string loanID;
    std::string payment_date;
    int amount;
    std::string reason;
    int dueDay;
    std::string paymentStatus;

    std::vector<Fine> fineBooks;
public:
    Fine(dbConnection* dbConnection);
    Fine(const std::string& fineID, const std::string& loanID, const std::string& payment_date,
        int amount, const std::string& reason, int dueDay, const std::string& paymentStatus);

    std::string getLoanID() const;
    std::string getFineID() const;
    int getAmount() const;
    std::string getPaymentDate() const;
    const std::string& getReason() const;
    const int getDueDay() const;
    struct FineRecord {
        std::string loanID;
        std::string bookID;
        std::string bookTitle;
        std::string bookPrice;
        std::string bookStatus;
        int dueDays;
        std::string fineReason;
    };
    
    void setLoanID(const std::string& id);
    void setFineID(const std::string& id);
    void setAmount(int amount);
    void setPaymentDate(const std::string& date);
    void setReason(const std::string& newReason);
    void setDueDay(const int dueDay);

    void fineMenu(User user);
    void changeOverdueFineRate();
    void changeMaxOverdueFine();
    void changeDamageFinePercentage();
    void doPayment(User user, std::vector<FineRecord>& totalFines);
    std::string generateFineID(sql::Connection* conn);
    const std::vector<Fine>& getFines() const;
    void clearFines();
};

#endif // FINE_H