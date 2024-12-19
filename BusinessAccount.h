#ifndef BUSINESSACCOUNT_H
#define BUSINESSACCOUNT_H

#include "Account.h"

class BusinessAccount : public Account {
public:
    BusinessAccount(double initialBalance);
    void Withdraw(double amount) override;
    void DisplayInfo() const override;
};

#endif
