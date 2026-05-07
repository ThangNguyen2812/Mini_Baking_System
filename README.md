# 🏦 Mini Banking System - C++ Project

## 📋 Mô tả Project

Hệ thống ngân hàng mini được xây dựng bằng C++ với đầy đủ tính năng:
- ✅ Quản lý tài khoản (Account Management)
- ✅ Giao dịch (Deposit, Withdraw, Transfer)
- ✅ Lịch sử giao dịch (Transaction History)
- ✅ Thống kê (Statistics)
- ✅ Lưu/Tải dữ liệu từ file (File Handling)
- ✅ Xác thực PIN (PIN Verification)

---

## 🏗️ Cấu trúc Project

```
banking_system/
├── Account.h              # Header file cho class Account
├── Account.cpp            # Implementation của Account
├── Transaction.h          # Header file cho class Transaction
├── Transaction.cpp        # Implementation của Transaction
├── Bank.h                 # Header file cho class Bank
├── Bank.cpp               # Implementation của Bank (phần 1)
├── Bank2.cpp              # Implementation của Bank (phần 2)
├── Bank3.cpp              # Implementation của Bank (phần 3)
├── main.cpp               # Main program
├── Makefile               # Build file
├── accounts.txt           # Database: Tài khoản
├── transactions.txt       # Database: Giao dịch
└── README.md              # File này
```

---

## 🛠️ Cài đặt & Chạy

### Compile thủ công 
```bash
g++ -std=c++17 -o banking_system *.cpp
./banking_system
```

---

## 📊 Các Class Chính

### 1. Account Class
```cpp
class Account {
private:
    int id;
    string name;
    double balance;
    string pin;

public:
    // Methods
    void deposit(double amount);
    void withdraw(double amount);
    bool verifyPin(string pin);
    // ... Getters
};
```

**Tính năng:**
- Lưu trữ thông tin tài khoản
- Xác thực PIN
- Gửi/rút tiền (validate amount)

---

### 2. Transaction Class
```cpp
class Transaction {
private:
    int fromAccountId;
    int toAccountId;
    double amount;
    TransactionType type;  // DEPOSIT / WITHDRAW / TRANSFER
    time_t timestamp;

public:
    // Methods
    string getTypeString();
    string getFormattedTime();
    // ... Getters
};
```

**Tính năng:**
- Ghi lại mọi giao dịch
- Lưu thời gian tự động
- Theo dõi account nguồn & đích

---

### 3. Bank Class
```cpp
class Bank {
private:
    vector<Account> accounts;
    vector<Transaction> transactions;

public:
    // Account Management
    void createAccount(int id, string name, double balance, string pin);
    Account* findAccount(int id);
    Account* findAccount(string name);  // Overloading

    // Transactions
    void deposit(int accountId, double amount, string pin);
    void withdraw(int accountId, double amount, string pin);
    void transfer(int fromId, int toId, double amount, string pin);

    // Statistics (STL)
    void displayStatistics();
    Account getRichestAccount();
    void displayTop3RichestAccounts();
    Account getMostActiveAccount();  // Dùng map<int,int>

    // File I/O
    void loadFromFile();
    void saveToFile();
};
```

---

## 🎯 Các Tính Năng

### 1️⃣ Tạo Tài Khoản
```
Input:
- Account ID (int)
- Name (string)
- PIN (4+ digits)
- Initial Balance
```

**Validation:**
- ✓ ID không được trùng
- ✓ Balance không âm
- ✓ PIN phải ≥ 4 ký tự

---

### 2️⃣ Gửi Tiền (Deposit)
```
Input:
- Account ID
- Amount
- PIN (xác thực)

Output:
- Cộng vào balance
- Lưu transaction
- Hiển thị balance mới
```

---

### 3️⃣ Rút Tiền (Withdraw)
```
Input:
- Account ID
- Amount
- PIN

Validation:
- ✓ Balance ≥ Amount
- ✓ PIN đúng
- ✓ Amount > 0
```

---

### 4️⃣ Chuyển Tiền (Transfer) ⭐
```
Input:
- From Account ID
- To Account ID
- Amount
- PIN của tài khoản gửi

Validation:
- ✓ Cả 2 account tồn tại
- ✓ Không chuyển cho chính mình
- ✓ Đủ tiền
- ✓ PIN đúng

Operation:
1. Trừ tiền từ account A
2. Cộng tiền vào account B
3. Lưu transaction TRANSFER
```

---

### 5️⃣ Thống Kê (Statistics) 📊

#### Dùng STL Algorithms:
```cpp
// find_if: Tìm account theo điều kiện
auto it = find_if(accounts.begin(), accounts.end(),
                  [id](const Account& acc) { 
                      return acc.getId() == id; 
                  });

// max_element: Tìm account giàu nhất
auto richest = max_element(accounts.begin(), accounts.end(),
                           [](const Account& a, const Account& b) {
                               return a.getBalance() < b.getBalance();
                           });

// sort: Sắp xếp theo balance giảm dần
sort(accounts.begin(), accounts.end(),
     [](const Account& a, const Account& b) {
         return a.getBalance() > b.getBalance();
     });

// count_if: Đếm account có điều kiện
int count = count_if(accounts.begin(), accounts.end(),
                     [](const Account& acc) {
                         return acc.getBalance() > 1000;
                     });
```

#### Dùng Map để tìm Most Active:
```cpp
map<int, int> transactionCount;

for (const auto& trans : transactions) {
    transactionCount[trans.getFromAccountId()]++;
}

// Tìm key với value lớn nhất
int maxAccount = -1;
int maxCount = 0;

for (const auto& pair : transactionCount) {
    if (pair.second > maxCount) {
        maxCount = pair.second;
        maxAccount = pair.first;
    }
}
```

---

## 💾 File Database

### accounts.txt
```
ID Name PIN Balance
101 Alice 1234 5000.00
102 Bob 5678 3500.50
103 Charlie 9999 10000.00
```

### transactions.txt
```
FromID ToID Amount Type Success
101 101 500.00 0 1
101 102 1000.00 2 1
102 102 2000.00 1 1
```

---

## 🔒 Exception Handling

Chương trình xử lý các lỗi sau:

```cpp
try {
    // Negative amount
    if (amount <= 0) {
        throw runtime_error("Invalid amount");
    }

    // Insufficient balance
    if (balance < amount) {
        throw runtime_error("Insufficient balance!");
    }

    // Invalid PIN
    if (!verifyPin(inputPin)) {
        throw runtime_error("Invalid PIN!");
    }

    // Account not found
    if (acc == nullptr) {
        throw runtime_error("Account not found!");
    }

    // Same account transfer
    if (fromId == toId) {
        throw runtime_error("Cannot transfer to same account!");
    }

} catch (const exception& e) {
    cerr << "Error: " << e.what() << "\n";
}
```

---

## 📈 Yêu Cầu Kỹ Thuật ✅

| Yêu Cầu | Trạng Thái |
|---------|-----------|
| OOP Design (3 Classes) | ✅ |
| vector (STL) | ✅ |
| find_if, count_if, sort | ✅ |
| map<int, int> | ✅ |
| Function Overloading | ✅ (findAccount) |
| Template Function | ✅ (printVector) |
| File Handling (I/O) | ✅ |
| Exception Handling | ✅ |
| Pointer/Reference | ✅ |

---

## 💡 Cách Sử Dụng Ví Dụ

### Ví dụ 1: Tạo 2 tài khoản và chuyển tiền

```
Menu > 1 (Create Account)
ID: 101
Name: Alice
PIN: 1234
Balance: 5000

Menu > 1 (Create Account)
ID: 102
Name: Bob
PIN: 5678
Balance: 2000

Menu > 4 (Transfer)
From: 101
To: 102
Amount: 1000
PIN: 1234

Result:
✓ Transfer successful!
  From: Alice -> New Balance: $4000.00
  To: Bob -> New Balance: $3000.00
```

### Ví dụ 2: Xem thống kê

```
Menu > 8 (Statistics)

╔══════════════════════════════╗
║     BANK STATISTICS          ║
╠══════════════════════════════╣
║ Total Accounts: 2            ║
║ Total Transactions: 3        ║
║ Total Money: $7000.00        ║
╠══════════════════════════════╣
║ Richest Account:             ║
║   Alice - $4000.00           ║
╠══════════════════════════════╣
║ Most Active Account:         ║
║   Alice                      ║
╚══════════════════════════════╝
```

---

## 🚀 Tính Năng Nâng Cao (Optional)

Các tính năng có thể thêm:
- [ ] Lãi suất (Interest rate)
- [ ] Cảnh báo số dư thấp
- [ ] Multi-user login system
- [ ] Báo cáo theo ngày/tháng
- [ ] Mã hóa PIN
- [ ] Undo/Rollback giao dịch

---

## 🐛 Lỗi Thường Gặp & Cách Sửa

### 1. "No such file: accounts.txt"
➜ Bình thường! Lần đầu chạy sẽ tạo mới.

### 2. Chuyển tiền không thành công
➜ Kiểm tra:
- [ ] PIN có đúng không?
- [ ] Có đủ tiền không?
- [ ] Account ID có tồn tại không?

### 3. Compilation error: "undefined reference"
➜ Ensure tất cả .cpp files được compile:
```bash
g++ -std=c++17 -o banking_system *.cpp
```

---

## 📝 Notes

- Data được **tự động lưu** khi exit (Menu > 0)
- Data được **tự động load** khi start
- PIN được lưu **plaintext** (trong production nên mã hóa)
- All amounts are **unsigned** (no negative balance)
- Timestamps được set **tự động** cho mỗi transaction

---

## 📞 Support

Nếu có câu hỏi, kiểm tra:
1. Syntax đúng không?
2. Compiler version: `g++ --version`
3. C++17 enabled: `-std=c++17`
4. Tất cả files có không?

---

**Happy Banking! 🏦💰**
