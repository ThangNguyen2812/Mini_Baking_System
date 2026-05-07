#ifndef ACCOUNT_H
#define ACCOUNT_H

#include <string>
#include <iostream>
using namespace std;

class Account {
private:
    int id;
    string name;
    double balance;
    string pin; // PIN code for security

public:
    // Constructor
    Account();
    Account(int id, string name, double balance, string pin);

    // Getter methods
    int getId() const;
    string getName() const;
    double getBalance() const;
    string getPin() const;

    // Transaction methods
    void deposit(double amount);
    void withdraw(double amount);
    void setBalance(double amount);
    void setName(string newName);
    void setPin(string newPin);

    // Display
    void display() const;

    // PIN verification
    bool verifyPin(string pin) const;
};

#endif // ACCOUNT_H
