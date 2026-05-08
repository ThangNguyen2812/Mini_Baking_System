#include <gtest/gtest.h>

#include "Transaction.h"

TEST(TransactionTest, TypeStringMatchesEnum) {
    Transaction deposit(1, 1, 25.0, Transaction::DEPOSIT, true);
    Transaction withdraw(1, 1, 10.0, Transaction::WITHDRAW, true);
    Transaction transfer(1, 2, 15.0, Transaction::TRANSFER, true);

    EXPECT_EQ(deposit.getTypeString(), "DEPOSIT");
    EXPECT_EQ(withdraw.getTypeString(), "WITHDRAW");
    EXPECT_EQ(transfer.getTypeString(), "TRANSFER");
}

TEST(TransactionTest, StoresSuccessFlagAndAmount) {
    Transaction failedTransfer(1, 2, 99.5, Transaction::TRANSFER, false);

    EXPECT_FALSE(failedTransfer.isSuccess());
    EXPECT_DOUBLE_EQ(failedTransfer.getAmount(), 99.5);
}

TEST(TransactionTest, FormattedTimeIsNotEmpty) {
    Transaction transaction(1, 2, 50.0, Transaction::TRANSFER, true);

    EXPECT_FALSE(transaction.getFormattedTime().empty());
}