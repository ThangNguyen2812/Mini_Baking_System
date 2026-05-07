#include "../include/Bank.h"
#include <iostream>
#include <iomanip>
#include <sstream>
#include <map>
#include <cctype>

namespace {
constexpr int ADMIN_ACCOUNT_ID = 10;
constexpr const char* ADMIN_ACCOUNT_NAME = "Admin";
constexpr const char* ADMIN_ACCOUNT_PIN = "1234";
}

Bank::Bank() {
    accountsFile = "data/accounts.txt";
    transactionsFile = "data/transactions.txt";
}

Bank::~Bank() {
    saveToFile();
}

// ============= FILE HANDLING =============

void Bank::loadFromFile() {
    loadAccountsFromFile();
    loadTransactionsFromFile();
    ensureAdminAccount();
    cout << "✓ Data loaded from files!\n";
}

void Bank::saveToFile() {
    saveAccountsToFile();
    saveTransactionsToFile();
    cout << "Data saved to files!\n";
}

void Bank::ensureAdminAccount() {
    if (accountExists(ADMIN_ACCOUNT_ID)) {
        return;
    }

    try {
        accounts.push_back(Account(ADMIN_ACCOUNT_ID, ADMIN_ACCOUNT_NAME, 0.0, ADMIN_ACCOUNT_PIN));
        cout << "! Admin account created with ID 10 and default PIN 1234.\n";
    } catch (const exception& e) {
        cerr << "Error creating admin account: " << e.what() << "\n";
    }
}

void Bank::loadAccountsFromFile() {
    ifstream file(accountsFile);
    if (!file.is_open()) {
        cout << "No previous accounts file found. Starting fresh.\n";
        return;
    }

    int id;
    string name, pin;
    double balance;

    while (file >> id >> name >> pin >> balance) {
        try {
            accounts.push_back(Account(id, name, balance, pin));
        } catch (const exception& e) {
            cerr << "Error loading account: " << e.what() << "\n";
        }
    }
    file.close();
}

void Bank::loadTransactionsFromFile() {
    ifstream file(transactionsFile);
    if (!file.is_open()) {
        return;
    }

    int fromId, toId, typeInt, success;
    double amount;
    time_t timestamp;

    while (file >> fromId >> toId >> amount >> typeInt >> success) {
        Transaction::TransactionType type = static_cast<Transaction::TransactionType>(typeInt);
        transactions.push_back(Transaction(fromId, toId, amount, type, success));
    }
    file.close();
}

void Bank::saveAccountsToFile() {
    ofstream file(accountsFile);
    if (!file.is_open()) {
        cerr << "Error opening accounts file for writing!\n";
        return;
    }

    for (const auto& acc : accounts) {
        file << acc.getId() << " "
             << acc.getName() << " "
             << acc.getPin() << " "
             << fixed << setprecision(2) << acc.getBalance() << "\n";
    }
    file.close();
}

void Bank::saveTransactionsToFile() {
    ofstream file(transactionsFile);
    if (!file.is_open()) {
        cerr << "Error opening transactions file for writing!\n";
        return;
    }

    for (const auto& trans : transactions) {
        file << trans.getFromAccountId() << " "
             << trans.getToAccountId() << " "
             << fixed << setprecision(2) << trans.getAmount() << " "
             << static_cast<int>(trans.getType()) << " "
             << (trans.isSuccess() ? 1 : 0) << "\n";
    }
    file.close();
}

// ============= ACCOUNT MANAGEMENT =============

void Bank::createAccount(int id, string name, double initialBalance, string pin) {
    try {
        // Check if account already exists
        if (accountExists(id)) {
            throw runtime_error("Account with this ID already exists!");
        }

        if (id == ADMIN_ACCOUNT_ID) {
            throw runtime_error("Account ID 10 is reserved for the Admin account!");
        }

        if (name.empty()) {
            throw runtime_error("Account name cannot be empty!");
        }

        for (char ch : name) {
            if (isdigit(static_cast<unsigned char>(ch))) {
                throw runtime_error("Account name cannot contain numbers!");
            }
        }

        if (initialBalance < 0) {
            throw runtime_error("Initial balance cannot be negative!");
        }

        if (pin.length() < 4) {
            throw runtime_error("PIN must be at least 4 digits!");
        }

        accounts.push_back(Account(id, name, initialBalance, pin));
        cout << "✓ Account created successfully!\n";

    } catch (const exception& e) {
        cerr << "✗ Error: " << e.what() << "\n";
    }
}

int Bank::findAccountIndex(int id) {
    auto it = find_if(accounts.begin(), accounts.end(),
                      [id](const Account& acc) { return acc.getId() == id; });

    if (it != accounts.end()) {
        return distance(accounts.begin(), it);
    }
    return -1;
}

int Bank::findAccountIndex(string name) {
    auto it = find_if(accounts.begin(), accounts.end(),
                      [name](const Account& acc) { return acc.getName() == name; });

    if (it != accounts.end()) {
        return distance(accounts.begin(), it);
    }
    return -1;
}

// Function overloading - find by ID
Account* Bank::findAccount(int id) {
    int index = findAccountIndex(id);
    if (index != -1) {
        return &accounts[index];
    }
    return nullptr;
}

// Function overloading - find by name
Account* Bank::findAccount(string name) {
    int index = findAccountIndex(name);
    if (index != -1) {
        return &accounts[index];
    }
    return nullptr;
}

bool Bank::accountExists(int id) {
    return findAccountIndex(id) != -1;
}

Account* Bank::login(int id, string pin) {
    try {
        Account* acc = findAccount(id);
        if (acc == nullptr) {
            throw runtime_error("Account not found! Please create a new account.");
        }

        if (!acc->verifyPin(pin)) {
            throw runtime_error("Invalid PIN!");
        }

        cout << "✓ Login successful! Welcome, " << acc->getName() << "!\n";
        return acc;

    } catch (const exception& e) {
        cerr << "✗ Login failed: " << e.what() << "\n";
        return nullptr;
    }
}

void Bank::saveAccountChanges() {
    saveAccountsToFile();
}

bool Bank::updateAccountInfo(int id, const string& newName, const string& newPin, bool changeName, bool changePin) {
    try {
        Account* account = findAccount(id);
        if (account == nullptr) {
            throw runtime_error("Account not found!");
        }

        if (changeName) {
            account->setName(newName);
        }

        if (changePin) {
            account->setPin(newPin);
        }

        saveAccountsToFile();
        return true;
    } catch (const exception& e) {
        cerr << "✗ Error: " << e.what() << "\n";
        return false;
    }
}

void Bank::displayAllAccounts() const {
    if (accounts.empty()) {
        cout << "No accounts found!\n";
        return;
    }

    cout << "\n========== ALL ACCOUNTS ==========\n";
    for (const auto& acc : accounts) {
        acc.display();
    }
    cout << "==================================\n\n";
}

// ============= TRANSACTION OPERATIONS =============

void Bank::deposit(int accountId, double amount, string pin) {
    try {
        Account* acc = findAccount(accountId);
        if (acc == nullptr) {
            throw runtime_error("Account not found!");
        }

        if (!acc->verifyPin(pin)) {
            throw runtime_error("Invalid PIN!");
        }

        if (amount <= 0) {
            throw runtime_error("Deposit amount must be positive!");
        }

        acc->deposit(amount);
        transactions.push_back(Transaction(accountId, accountId, amount, 
                                          Transaction::DEPOSIT, true));

        cout << " Deposit successful! New balance: $" << fixed << setprecision(2) 
             << acc->getBalance() << "\n";

    } catch (const exception& e) {
        cerr << "✗ Error: " << e.what() << "\n";
        transactions.push_back(Transaction(accountId, accountId, amount, 
                                          Transaction::DEPOSIT, false));
    }
}

void Bank::withdraw(int accountId, double amount, string pin) {
    try {
        Account* acc = findAccount(accountId);
        if (acc == nullptr) {
            throw runtime_error("Account not found!");
        }

        if (!acc->verifyPin(pin)) {
            throw runtime_error("Invalid PIN!");
        }

        if (amount <= 0) {
            throw runtime_error("Withdraw amount must be positive!");
        }

        if (acc->getBalance() < amount) {
            throw runtime_error("Insufficient balance!");
        }

        acc->withdraw(amount);
        transactions.push_back(Transaction(accountId, accountId, amount, 
                                          Transaction::WITHDRAW, true));

        cout << "✓ Withdrawal successful! New balance: $" << fixed << setprecision(2) 
             << acc->getBalance() << "\n";

    } catch (const exception& e) {
        cerr << "✗ Error: " << e.what() << "\n";
        transactions.push_back(Transaction(accountId, accountId, amount, 
                                          Transaction::WITHDRAW, false));
    }
}

void Bank::transfer(int fromId, int toId, double amount, string fromPin) {
    try {
        // Validation
        if (fromId == toId) {
            throw runtime_error("Cannot transfer to the same account!");
        }

        Account* fromAcc = findAccount(fromId);
        Account* toAcc = findAccount(toId);

        if (fromAcc == nullptr) {
            throw runtime_error("Source account not found!");
        }

        if (toAcc == nullptr) {
            throw runtime_error("Destination account not found!");
        }

        if (!fromAcc->verifyPin(fromPin)) {
            throw runtime_error("Invalid PIN!");
        }

        if (amount <= 0) {
            throw runtime_error("Transfer amount must be positive!");
        }

        if (fromAcc->getBalance() < amount) {
            throw runtime_error("Insufficient balance!");
        }

        // Perform transfer
        double fromBalance = fromAcc->getBalance();
        double toBalance = toAcc->getBalance();

        fromAcc->setBalance(fromBalance - amount);
        toAcc->setBalance(toBalance + amount);

        transactions.push_back(Transaction(fromId, toId, amount, 
                                          Transaction::TRANSFER, true));

        cout << " Transfer successful!\n";
        cout << "  From: " << fromAcc->getName() << " -> New Balance: $" 
             << fixed << setprecision(2) << fromAcc->getBalance() << "\n";
        cout << "  To: " << toAcc->getName() << " -> New Balance: $" 
             << fixed << setprecision(2) << toAcc->getBalance() << "\n";

    } catch (const exception& e) {
        cerr << " Error: " << e.what() << "\n";
        transactions.push_back(Transaction(fromId, toId, amount, 
                                          Transaction::TRANSFER, false));
    }
}

// ============= TRANSACTION HISTORY =============

void Bank::displayAllTransactions() const {
    if (transactions.empty()) {
        cout << "No transactions found!\n";
        return;
    }

    cout << "\n ALL TRANSACTIONS \n";
    for (const auto& trans : transactions) {
        trans.display();
    }
    cout << "\n\n";
}

void Bank::displayAccountTransactions(int accountId) const {
    vector<Transaction> filtered;

    for (const auto& trans : transactions) {
        // Show only transactions where user is involved
        if ((trans.getFromAccountId() == accountId || trans.getToAccountId() == accountId) 
            && trans.isSuccess()) {  // Only show successful transactions
            filtered.push_back(trans);
        }
    }

    if (filtered.empty()) {
        cout << "No transactions found for account " << accountId << "!\n";
        return;
    }

    cout << "\n========== TRANSACTIONS FOR ACCOUNT " << accountId << " ==========\n";
    for (const auto& trans : filtered) {
        trans.display();
    }
    cout << "=====================================================\n\n";
}

// ============= STATISTICS (STL + Map) =============

double Bank::getTotalMoney() const {
    double total = 0;
    for (const auto& acc : accounts) {
        total += acc.getBalance();
    }
    return total;
}

int Bank::getTotalTransactions() const {
    return transactions.size();
}

Account Bank::getRichestAccount() {
    if (accounts.empty()) {
        throw runtime_error("No accounts found!");
    }

    auto richest = max_element(accounts.begin(), accounts.end(),
                               [](const Account& a, const Account& b) {
                                   return a.getBalance() < b.getBalance();
                               });

    return *richest;
}

void Bank::displayTop3RichestAccounts() {
    if (accounts.empty()) {
        cout << "No accounts found!\n";
        return;
    }

    vector<Account> sorted_accounts = accounts;

    // Sort by balance in descending order
    sort(sorted_accounts.begin(), sorted_accounts.end(),
         [](const Account& a, const Account& b) {
             return a.getBalance() > b.getBalance();
         });

    cout << "\n========== TOP 3 RICHEST ACCOUNTS ==========\n";
    int count = min(3, (int)sorted_accounts.size());

    for (int i = 0; i < count; i++) {
        cout << i + 1 << ". ";
        cout << sorted_accounts[i].getName() << " - $" 
             << fixed << setprecision(2) << sorted_accounts[i].getBalance() << "\n";
    }
    cout << "==========================================\n\n";
}

Account Bank::getMostActiveAccount() {
    if (accounts.empty()) {
        throw runtime_error("No accounts found!");
    }

    // Use map to count transactions per account
    map<int, int> transactionCount;

    for (const auto& trans : transactions) {
        transactionCount[trans.getFromAccountId()]++;
        if (trans.getType() == Transaction::TRANSFER) {
            transactionCount[trans.getToAccountId()]++;
        }
    }

    // Find account with most transactions
    int maxAccount = -1;
    int maxCount = 0;

    for (const auto& pair : transactionCount) {
        if (pair.second > maxCount) {
            maxCount = pair.second;
            maxAccount = pair.first;
        }
    }

    if (maxAccount == -1) {
        throw runtime_error("No transactions found!");
    }

    Account* mostActive = findAccount(maxAccount);
    return *mostActive;
}

void Bank::displayStatistics() {
    try {
        cout << "      BANK STATISTICS      \n";

        cout << "  Total Accounts: " << setw(22) << accounts.size() << " \n";
        cout << "  Total Transactions: " << setw(18) << getTotalTransactions() << " \n";
        cout << "  Total Money in System: $" << setw(14) << fixed << setprecision(2) 
             << getTotalMoney() << " ║\n";

        if (!accounts.empty()) {
            try {
                Account richest = getRichestAccount();
                cout << "\n Richest Account:                       \n";
                cout << "   " << setw(34) << richest.getName() << " \n";
                cout << "   Balance: $" << setw(24) << fixed << setprecision(2) 
                     << richest.getBalance() << " \n";
            } catch (const exception& e) {
                cout << " Error getting richest account!         \n";
            }

            try {
                Account mostActive = getMostActiveAccount();
                cout << " Most Active Account:                   \n";
                cout << "   " << setw(34) << mostActive.getName() << " \n";
            } catch (const exception& e) {
                cout << " No active accounts!                    \n";
            }
        }

    } catch (const exception& e) {
        cerr << "Error displaying statistics: " << e.what() << "\n";
    }
}
