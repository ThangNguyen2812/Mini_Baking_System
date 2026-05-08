#include "../include/Bank.h"
#include <iostream>
#include <limits>
#include <cctype>
using namespace std;


constexpr int ADMIN_ACCOUNT_ID = 10; // ID for Admin account.


void clearScreen() {
#ifdef _WIN32
    system("cls");
#else
    system("clear");
#endif
}


// Login and account creation menus
void displayAuthMenu() {
    cout << "  ========== MINI BANKING SYSTEM ==========\n";
    cout << "         Welcome to Our Bank             \n";
    cout << " 1. Login to Existing Account             \n";
    cout << " 2. Create New Account                    \n";
    cout << " 0. Exit                                  \n";
    cout << "Enter your choice: ";
}

// User interface functions
void displayMainMenu(Account* currentUser) {
    cout << "      ===== USER MENU =====                \n";
    if (currentUser != nullptr) {
        cout << "Hello, " << currentUser->getName() << "!\n";
    }
    cout << " 1. Deposit Money                         \n";
    cout << " 2. Withdraw Money                        \n";
    cout << " 3. Transfer Money                        \n";
    cout << " 4. Show My Transactions                  \n";
    cout << " 5. View My Account Info                  \n";
    cout << " 6. Change Account Info                  \n";
    cout << " 0. Logout (Save Data)                    \n";
    cout << "Enter your choice: ";
}

// Admin interface functions
void displayAdminMenu() {
    cout << "      ===== ADMIN MENU =====               \n";
    cout << " 1. Show All Accounts                     \n";
    cout << " 2. Show All Transactions                 \n";
    cout << " 3. Show Statistics                       \n";
    cout << " 4. Show Top 3 Richest Accounts           \n";
    cout << " 5. View My Account Info                  \n";
    cout << " 0. Logout (Save Data)                    \n";
    cout << "Enter your choice: ";
}

// Input validation functions
bool isValidAccountName(const string& name) {
    if (name.empty()) {
        return false;
    }

    for (char ch : name) {
        if (isdigit(static_cast<unsigned char>(ch))) {
            return false;
        }
    }

    return true;
}


void clearInputLine() { // Clear input buffer
    cin.clear();
    cin.ignore(numeric_limits<streamsize>::max(), '\n');
}

void pauseForEnter() { // Pause execution until user presses Enter
    cout << "\nPress Enter to continue...";
    string dummy;
    getline(cin, dummy);
}

// Prompt user for integer input with validation
int promptInt(const string& prompt) {
    int value;

    while (true) {
        cout << prompt;
        if (cin >> value) {
            clearInputLine();
            return value;
        }

        cout << "Invalid input! Please enter a number.\n";
        clearInputLine();
    }
}

// Prompt user for double input with validation
double promptDouble(const string& prompt) {
    double value;

    while (true) {
        cout << prompt;
        if (cin >> value) {
            clearInputLine();
            return value;
        }

        cout << "Invalid input! Please enter a valid amount.\n";
        clearInputLine();
    }
}


void createNewAccount(Bank& bank) { // Handle account creation with input validation
    while (true) {
        clearScreen();
        cout << "\n========== CREATE NEW ACCOUNT ==========\n";

        int id = promptInt("\nEnter Account ID: \n");
        if (bank.accountExists(id)) {
            cout << "Error: Account with this ID already exists.\n";
            pauseForEnter();
            continue;
        }

        string name;
        cout << "\nEnter Account Name: \n";
        getline(cin, name);

        if (!isValidAccountName(name)) {
            cout << "Error: Account name must not be empty and cannot contain numbers.\n";
            pauseForEnter();
            continue;
        }

        string pin;
        while (true) {
            cout << "\nEnter PIN (at least 4 digits): \n";
            getline(cin, pin);

            if (pin.length() >= 4) {
                break;
            }

            cout << "Error: PIN must be at least 4 digits.\n";
        }

        double balance = promptDouble("\nEnter Initial Balance: $\n");
        if (balance < 0) {
            cout << "Error: Initial balance cannot be negative.\n";
            pauseForEnter();
            continue;
        }

        bank.createAccount(id, name, balance, pin);
        pauseForEnter();
        clearScreen();
        return;
    }
}

Account* handleLogin(Bank& bank) { // Handle user login with input validation
    while (true) {
        clearScreen();
        cout << "\n========== LOGIN ==========\n";

        int accountId = promptInt("\nEnter Account ID: \n");
        string pin;

        cout << "\nEnter PIN: \n";
        getline(cin, pin);

        Account* account = bank.login(accountId, pin);
        if (account != nullptr) {
            return account;
        }

        pauseForEnter();
    }
}

void changeAccountInfo(Bank& bank, Account* currentUser) { // Handle account info change with validation
    while (true) {
        clearScreen();
        cout << "\n========== CHANGE ACCOUNT INFO ==========" << "\n";
        cout << "Account ID: " << currentUser->getId() << "\n";
        cout << "Current Name: " << currentUser->getName() << "\n";
        cout << "\nVerify current PIN to continue.\n";

        string currentPin;
        cout << "Enter PIN: ";
        getline(cin, currentPin);

        if (!currentUser->verifyPin(currentPin)) {
            cout << "Error: Invalid PIN.\n";
            pauseForEnter();
            continue;
        }

        cout << "\nWhat do you want to change?\n";
        cout << " 1. Change Name\n";
        cout << " 2. Change PIN\n";
        cout << " 3. Change Both\n";
        cout << " 0. Cancel\n";

        int changeChoice = promptInt("Enter your choice: ");
        if (changeChoice == 0) {
            return;
        }

        if (changeChoice != 1 && changeChoice != 2 && changeChoice != 3) {
            cout << "Invalid choice!\n";
            pauseForEnter();
            continue;
        }

        string newName = currentUser->getName(); 
        string newPin = currentUser->getPin();
        bool changeName = false;
        bool changePin = false;

        if (changeChoice == 1 || changeChoice == 3) {
            cout << "\nEnter New Name: ";
            getline(cin, newName);

            if (!isValidAccountName(newName)) {
                cout << "Error: Account name must not be empty and cannot contain numbers.\n";
                pauseForEnter();
                continue;
            }

            changeName = true;
        }

        if (changeChoice == 2 || changeChoice == 3) {
            while (true) {
                cout << "Enter New PIN (at least 4 digits): ";
                getline(cin, newPin);

                if (newPin.length() >= 4) {
                    break;
                }

                cout << "Error: PIN must be at least 4 digits.\n";
            }

            changePin = true;
        }

        cout << "\nYou are about to update your account with these values:\n";
        if (changeName) {
            cout << " Name: " << newName << "\n";
        }
        if (changePin) {
            cout << " PIN: " << newPin << "\n";
        }
        cout << "\nAgree to change? (y/n): ";

        string agree;
        getline(cin, agree);

        if (agree != "y" && agree != "Y") {
            cout << "Change cancelled.\n";
            pauseForEnter();
            return;
        }

        try {
            if (bank.updateAccountInfo(currentUser->getId(), newName, newPin, changeName, changePin)) {
                currentUser = bank.findAccount(currentUser->getId());
                cout << "Account information updated successfully!\n";
                pauseForEnter();
                clearScreen();
                return;
            }
        } catch (const exception& e) {
            cout << "Error: " << e.what() << "\n";
            pauseForEnter();
        }
    }
}

// User menu handler
void handleUserMenu(Bank& bank, Account* currentUser) {
    int mainChoice;

    while (true) {
        displayMainMenu(currentUser);

        if (!(cin >> mainChoice)) {
            clearInputLine();
            cout << "Invalid input! Please enter a number.\n";
            continue;
        }
        clearInputLine();

        switch (mainChoice) {
            case 1: {
                changeAccountInfo(bank, currentUser);
                break;

                double amount = promptDouble("\nEnter Amount: $\n");

                while (amount <= 0) {
                    cout << "Error: Deposit amount must be positive.\n";
                    amount = promptDouble("\nEnter Amount: $\n");
                }

                bank.deposit(currentUser->getId(), amount, currentUser->getPin());
                pauseForEnter();
                clearScreen();
                break;
            }

            case 2: {
                clearScreen();
                cout << "\n========== WITHDRAW MONEY ==========\n";

                double amount = promptDouble("\nEnter Amount: $\n");

                while (amount <= 0) {
                    cout << "Error: Withdraw amount must be positive.\n";
                    amount = promptDouble("\nEnter Amount: $\n");
                }

                bank.withdraw(currentUser->getId(), amount, currentUser->getPin());
                pauseForEnter();
                clearScreen();
                break;
            }

            case 3: {
                clearScreen();
                cout << "\n========== TRANSFER MONEY ==========\n";

                int toId;
                while (true) {
                    toId = promptInt("\nEnter Recipient Account ID: \n");
                    if (toId == currentUser->getId()) {
                        cout << "Error: Cannot transfer to the same account.\n";
                        continue;
                    }
                    if (bank.findAccount(toId) == nullptr) {
                        cout << "Error: Destination account not found.\n";
                        continue;
                    }
                    break;
                }

                double amount = promptDouble("\nEnter Amount: $\n");

                while (amount <= 0) {
                    cout << "Error: Transfer amount must be positive.\n";
                    amount = promptDouble("\nEnter Amount: $\n");
                }

                bank.transfer(currentUser->getId(), toId, amount, currentUser->getPin());
                pauseForEnter();
                clearScreen();
                break;
            }

            case 4: {
                clearScreen();
                cout << "\n========== YOUR TRANSACTIONS ==========\n";
                bank.displayAccountTransactions(currentUser->getId());
                pauseForEnter();
                clearScreen();
                break;
            }

            case 5: {
                clearScreen();
                cout << "\n========== YOUR ACCOUNT INFO ==========\n";
                currentUser->display();
                cout << "\n";
                pauseForEnter();
                clearScreen();
                break;
            }

            case 6: {
                clearScreen();
                changeAccountInfo(bank, currentUser);
                clearScreen();
                break;
            }

            case 0: {
                cout << "\nLogging out...\n";
                bank.saveToFile();
                clearScreen();
                return;
            }

            default:
                cout << "Invalid choice! Please try again.\n";
        }
    }
}

// Admin menu handler
void handleAdminMenu(Bank& bank, Account* currentUser) {
    int adminChoice;

    while (true) {
        displayAdminMenu();

        if (!(cin >> adminChoice)) {
            clearInputLine();
            cout << "Invalid input! Please enter a number.\n";
            continue;
        }
        clearInputLine();

        switch (adminChoice) {
            case 1: {
                clearScreen();
                bank.displayAllAccounts();
                pauseForEnter();
                clearScreen();
                break;
            }

            case 2: {
                clearScreen();
                bank.displayAllTransactions();
                pauseForEnter();
                clearScreen();
                break;
            }

            case 3: {
                clearScreen();
                bank.displayStatistics();
                pauseForEnter();
                clearScreen();
                break;
            }

            case 4: {
                clearScreen();
                bank.displayTop3RichestAccounts();
                pauseForEnter();
                clearScreen();
                break;
            }

            case 5: {
                clearScreen();
                cout << "\n========== ADMIN ACCOUNT INFO ==========\n";
                currentUser->display();
                cout << "\n";
                pauseForEnter();
                clearScreen();
                break;
            }

            case 0: {
                cout << "\nLogging out...\n";
                bank.saveToFile();
                clearScreen();
                return;
            }

            default:
                cout << "Invalid choice! Please try again.\n";
        }
    }
}






int main() {
    Bank bank;
    bank.loadFromFile();

    int authChoice;
    bool appRunning = true;
    Account* currentUser = nullptr;

    // Authentication Loop
    while (appRunning) {
        if (currentUser == nullptr) {
            // Not logged in - show authentication menu
            displayAuthMenu();

            // Input validation
            if (!(cin >> authChoice)) {
                clearInputLine();
                cout << "Invalid input! Please enter a number.\n";
                continue;
            }
            clearInputLine();

            switch (authChoice) {
                case 1: {  // Login
                    currentUser = handleLogin(bank);
                    if (currentUser != nullptr) {
                        clearScreen();
                    }
                    break;
                }

                case 2: {  // Create Account
                    createNewAccount(bank);
                    cout << "\nNow please login with your new account.\n";
                    pauseForEnter();
                    clearScreen();
                    break;
                }

                case 0: {  // Exit
                    cout << "\nSaving data...\n";
                    bank.saveToFile();
                    cout << "Thank you for using Mini Banking System!\n";
                    appRunning = false;
                    break;
                }

                default:
                    cout << "Invalid choice! Please try again.\n";
            }
        } 
        else {
            if (currentUser->getId() == ADMIN_ACCOUNT_ID) {
                handleAdminMenu(bank, currentUser);
            } else {
                handleUserMenu(bank, currentUser);
            }

            currentUser = nullptr;
        }
    }

    return 0;
}




