#ifndef CHECKINGACCOUNT_H
#define CHECKINGACCOUNT_H

#include "Account.h"

class CheckingAccount : public Account {
private:
    double overdraftLimit;
public:
    CheckingAccount(double initialBalance, double overdraftLimit = 0);
    void Withdraw(double amount) override;
    void DisplayInfo() const override;
    void SetOverdraftLimit(double newLimit);
    double GetOverdraftLimit() const;
};

#endif
