#include "../include/Transaction.h"
#include <iomanip>
#include <sstream>

// Constructor default
Transaction::Transaction()
    : fromAccountId(0), toAccountId(0), amount(0), type(DEPOSIT), success(true) {
    timestamp = time(nullptr);
}

// Constructor with parameter
Transaction::Transaction(int fromId, int toId, double amt, TransactionType t, bool success)
    : fromAccountId(fromId), toAccountId(toId), amount(amt), type(t), success(success) {
    timestamp = time(nullptr);
}

// Getter methods

int Transaction::getFromAccountId() const { // Getter for fromAccountId
    return fromAccountId;
}



int Transaction::getToAccountId() const {  // Getter for toAccountId
    return toAccountId;
}

double Transaction::getAmount() const { // Getter for amount
    return amount;
}

Transaction::TransactionType Transaction::getType() const { // Getter for type
    return type;
}

time_t Transaction::getTimestamp() const {  // Getter for timestamp
    return timestamp;
}

bool Transaction::isSuccess() const {   // Getter for success
    return success;
}

// Get type as string
string Transaction::getTypeString() const {
    switch (type) {
        case DEPOSIT:
            return "DEPOSIT";
        case WITHDRAW:
            return "WITHDRAW";
        case TRANSFER:
            return "TRANSFER";
        default:
            return "UNKNOWN";
    }
}

// Get formatted time
string Transaction::getFormattedTime() const {
    struct tm* timeinfo = localtime(&timestamp);
    char buffer[80];
    strftime(buffer, sizeof(buffer), "%Y-%m-%d %H:%M:%S", timeinfo);
    return string(buffer);
}

// Display transaction
void Transaction::display() const {
    cout << " Type: " << getTypeString();
    if (!success) {
        cout << " [FAILED]";
    }
    cout << "\n";
    cout << " From Account: " << fromAccountId << "\n";
    cout << " To Account: " << toAccountId << "\n";
    cout << " Amount: $" << fixed << setprecision(2) << amount << "\n";
    cout << " Time: " << getFormattedTime() << "\n\n";
}
