#include "SavingsAccount.h"
#include <iostream>
#include <iomanip>

using namespace std;

SavingsAccount::SavingsAccount(double initialBalance, double interestRate)
    : Account(initialBalance), interestRate(interestRate) {
}

void SavingsAccount::Withdraw(double amount) {
    if (amount <= balance) {
        balance -= amount;
        std::cout << "Снятие: " << std::fixed << std::setprecision(2) << amount << " выполнено." << std::endl;
    }
    else {
        std::cout << "Недостаточно средств для снятия!" << std::endl;
    }
}

void SavingsAccount::ApplyInterest() {
    double interest = balance * interestRate;
    balance += interest;
    std::cout << "Проценты начислены: " << std::fixed << std::setprecision(2) << interest << std::endl;
}

void SavingsAccount::DisplayInfo() const {
    std::cout << "Сберегательный счет, баланс: " << std::fixed << std::setprecision(2) << balance << ", Процентная ставка: " << std::fixed << std::setprecision(2) << interestRate << std::endl;
}

double SavingsAccount::GetInterestRate() const {
    return interestRate;
}
