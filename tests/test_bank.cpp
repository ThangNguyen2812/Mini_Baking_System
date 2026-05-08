#include <filesystem>
#include <fstream>

#include <gtest/gtest.h>

#include "Bank.h"

namespace fs = std::filesystem;

class BankTest : public ::testing::Test {
protected:
    void SetUp() override {
        fs::remove_all("data");
        fs::create_directories("data");

        std::ofstream accounts("data/accounts.txt");
        accounts << "1 Alice 1234 500.00\n";
        accounts << "2 Bob 4321 250.00\n";
        accounts.close();

        std::ofstream transactions("data/transactions.txt");
        transactions.close();
    }

    void TearDown() override {
        fs::remove_all("data");
    }
};

TEST_F(BankTest, LoadFromFileAddsAccountsAndAdmin) {
    Bank bank;

    bank.loadFromFile();

    ASSERT_NE(bank.findAccount(1), nullptr);
    ASSERT_NE(bank.findAccount(2), nullptr);
    ASSERT_NE(bank.findAccount(10), nullptr);
    EXPECT_EQ(bank.findAccount(1)->getName(), "Alice");
}

TEST_F(BankTest, DepositWithdrawAndDuplicateAccountAreHandled) {
    Bank bank;

    bank.createAccount(3, "Carol", 100.0, "9999");
    bank.createAccount(3, "Carol2", 200.0, "8888");

    ASSERT_NE(bank.findAccount(3), nullptr);
    EXPECT_DOUBLE_EQ(bank.findAccount(3)->getBalance(), 100.0);

    bank.deposit(3, 50.0, "9999");
    bank.withdraw(3, 25.0, "9999");

    EXPECT_DOUBLE_EQ(bank.findAccount(3)->getBalance(), 125.0);
    EXPECT_EQ(bank.getTotalTransactions(), 2);
}

TEST_F(BankTest, TransferMovesMoneyBetweenAccounts) {
    Bank bank;

    bank.createAccount(3, "Carol", 300.0, "9999");
    bank.createAccount(4, "Dave", 100.0, "7777");

    bank.transfer(3, 4, 75.0, "9999");

    ASSERT_NE(bank.findAccount(3), nullptr);
    ASSERT_NE(bank.findAccount(4), nullptr);
    EXPECT_DOUBLE_EQ(bank.findAccount(3)->getBalance(), 225.0);
    EXPECT_DOUBLE_EQ(bank.findAccount(4)->getBalance(), 175.0);
    EXPECT_EQ(bank.getTotalTransactions(), 1);
}

TEST_F(BankTest, UpdateAccountInfoPersistsToDisk) {
    {
        Bank bank;
        bank.loadFromFile();

        ASSERT_TRUE(bank.updateAccountInfo(1, "Alicia", "1111", true, true));
        ASSERT_NE(bank.findAccount(1), nullptr);
        EXPECT_EQ(bank.findAccount(1)->getName(), "Alicia");
        EXPECT_TRUE(bank.findAccount(1)->verifyPin("1111"));
        bank.saveToFile();
    }

    Bank reloaded;
    reloaded.loadFromFile();

    ASSERT_NE(reloaded.findAccount(1), nullptr);
    EXPECT_EQ(reloaded.findAccount(1)->getName(), "Alicia");
    EXPECT_TRUE(reloaded.findAccount(1)->verifyPin("1111"));
}