#include "../include/Account.h"
#include <stdexcept>
#include <iomanip>
#include <cctype>

// Constructor default
Account::Account() : id(0), name(""), balance(0), pin("") {}

// Constructor with parameter
Account::Account(int id, string name, double balance, string pin)
    : id(id), name(name), balance(balance), pin(pin) {
    if (balance < 0) {
        throw runtime_error("Initial balance cannot be negative!");
    }
    if (name.empty()) {
        throw runtime_error("Account name cannot be empty!");
    }
    for (char ch : name) {
        if (isdigit(static_cast<unsigned char>(ch))) {
            throw runtime_error("Account name cannot contain numbers!");
        }
    }
    if (pin.empty()) {
        throw runtime_error("PIN cannot be empty!");
    }
}

// Getter methods
int Account::getId() const {
    return id;
}

// Getter for name with validation
string Account::getName() const {
    if (name.empty()) {
        throw runtime_error("Account name is not set!");
    }
    for (char ch : name) {
        if (isdigit(static_cast<unsigned char>(ch))) {
            throw runtime_error("Account name cannot contain numbers!");
        }
    }
    return name;
}

// Getter for balance
double Account::getBalance() const {
    return balance;
}

// Getter for PIN
string Account::getPin() const {
    return pin;
}

// Deposit method
void Account::deposit(double amount) {
    if (amount <= 0) {
        throw runtime_error("Deposit amount must be positive!");
    }
    balance += amount;
}

// Withdraw method
void Account::withdraw(double amount) {
    if (amount <= 0) {
        throw runtime_error("Withdraw amount must be positive!");
    }
    if (balance < amount) {
        throw runtime_error("Insufficient balance!");
    }
    balance -= amount;
}

// Set balance
void Account::setBalance(double amount) {
    if (amount < 0) {
        throw runtime_error("Balance cannot be negative!");
    }
    balance = amount;
}

// Set name with validation
void Account::setName(string newName) {
    if (newName.empty()) {
        throw runtime_error("Account name cannot be empty!");
    }

    for (char ch : newName) {
        if (isdigit(static_cast<unsigned char>(ch))) {
            throw runtime_error("Account name cannot contain numbers!");
        }
    }

    name = newName;
}

// Set PIN with validation
void Account::setPin(string newPin) {
    if (newPin.length() < 4) {
        throw runtime_error("PIN must be at least 4 digits!");
    }

    pin = newPin;
}

// Verify PIN
bool Account::verifyPin(string inputPin) const {
    return pin == inputPin;
}

// Display account info
void Account::display() const {
    cout << " Account ID: " << id << "\n";
    cout << " Name: " << name << "\n";
    cout << " Balance: $" << fixed << setprecision(2) << balance << "\n";
}
