#ifndef TRANSACTION_H
#define TRANSACTION_H

#include <string>
#include <ctime>
#include <iostream>
using namespace std;

class Transaction {
public:
    enum TransactionType {
        DEPOSIT,
        WITHDRAW,
        TRANSFER
    };

private:
    int fromAccountId;
    int toAccountId;
    double amount;
    TransactionType type;
    time_t timestamp;
    bool success;

public:
    // Constructor
    Transaction();
    Transaction(int fromId, int toId, double amt, TransactionType t, bool success = true);

    // Getter methods
    int getFromAccountId() const;
    int getToAccountId() const;
    double getAmount() const;
    TransactionType getType() const;
    time_t getTimestamp() const;
    bool isSuccess() const;

    // Helper methods
    string getTypeString() const;
    string getFormattedTime() const;

    // Display
    void display() const;
};

#endif // TRANSACTION_H
