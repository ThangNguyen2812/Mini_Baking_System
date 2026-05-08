#include <gtest/gtest.h>

#include "Account.h"

TEST(AccountTest, DepositAndWithdrawUpdateBalance) {
    Account account(1, "Alice", 100.0, "1234");

    account.deposit(50.0);
    account.withdraw(25.0);

    EXPECT_DOUBLE_EQ(account.getBalance(), 125.0);
}

TEST(AccountTest, RejectsInvalidName) {
    EXPECT_THROW(Account(1, "Alice1", 100.0, "1234"), std::runtime_error);
    EXPECT_THROW(Account(2, "", 100.0, "1234"), std::runtime_error);
}

TEST(AccountTest, RejectsShortPin) {
    Account account(1, "Alice", 100.0, "1234");

    EXPECT_THROW(account.setPin("123"), std::runtime_error);
}

TEST(AccountTest, VerifyPinMatchesStoredPin) {
    Account account(1, "Alice", 100.0, "1234");

    EXPECT_TRUE(account.verifyPin("1234"));
    EXPECT_FALSE(account.verifyPin("0000"));
}

TEST(AccountTest, UpdateNameAndPin) {
    Account account(1, "Alice", 100.0, "1234");

    account.setName("Alicia");
    account.setPin("4321");

    EXPECT_EQ(account.getName(), "Alicia");
    EXPECT_TRUE(account.verifyPin("4321"));
}