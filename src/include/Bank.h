#ifndef BANK_H
#define BANK_H

#include "Account.h"
#include "Transaction.h"
#include <vector>
#include <map>
#include <algorithm>
#include <fstream>

class Bank {
private:
    vector<Account> accounts;
    vector<Transaction> transactions;
    string accountsFile;
    string transactionsFile;
    void ensureAdminAccount();

    // Helper methods
    int findAccountIndex(int id);
    int findAccountIndex(string name);

public:
    // Constructor & Destructor
    Bank();
    ~Bank();

    // File handling
    void loadFromFile();
    void saveToFile();
    void loadAccountsFromFile();
    void loadTransactionsFromFile();
    void saveAccountsToFile();
    void saveTransactionsToFile();

    // Account management
    void createAccount(int id, string name, double initialBalance, string pin);
    Account* findAccount(int id);
    Account* findAccount(string name);
    void displayAllAccounts() const;
    bool accountExists(int id);
    Account* login(int id, string pin);  // Login with ID and PIN verification
    void saveAccountChanges();
    bool updateAccountInfo(int id, const string& newName, const string& newPin, bool changeName, bool changePin);

    // Transaction operations
    void deposit(int accountId, double amount, string pin);
    void withdraw(int accountId, double amount, string pin);
    void transfer(int fromId, int toId, double amount, string fromPin);

    // Transaction history
    void displayAllTransactions() const;
    void displayAccountTransactions(int accountId) const;

    // Statistics (using STL)
    void displayStatistics();
    Account getRichestAccount();
    void displayTop3RichestAccounts();
    double getTotalMoney() const;
    int getTotalTransactions() const;
    Account getMostActiveAccount();

    // Utility template
    template <typename T>
    void printVector(const vector<T>& list) const {
        if (list.empty()) {
            cout << "List is empty!\n";
            return;
        }
        for (const auto& item : list) {
            item.display();
        }
    }
};

#endif // BANK_H
