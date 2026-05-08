cmake_minimum_required(VERSION 3.15)

project(MiniBankingSystem LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

list(PREPEND CMAKE_PREFIX_PATH "C:/msys64/ucrt64")

add_library(bank_core
    src/source/Account.cpp
    src/source/Bank.cpp
    src/source/Transaction.cpp
)

target_include_directories(bank_core
    PUBLIC
        ${CMAKE_CURRENT_SOURCE_DIR}/src/include
)

add_executable(banking_system
    src/source/main.cpp
)

target_link_libraries(banking_system PRIVATE bank_core)

enable_testing()

find_package(GTest CONFIG REQUIRED)

add_executable(bank_tests
    tests/test_account.cpp
    tests/test_bank.cpp
    tests/test_transaction.cpp
)

target_link_libraries(bank_tests PRIVATE bank_core GTest::gtest GTest::gtest_main)

include(GoogleTest)
gtest_discover_tests(bank_tests)