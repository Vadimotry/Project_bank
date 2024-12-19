#ifndef BANK_H
#define BANK_H

#include <vector>
#include <memory>
#include <string>
#include <iostream>

#include "Customer.h"
#include "Account.h"

class Bank {
private:
    std::vector<std::shared_ptr<Customer>> customers;
    std::vector<std::shared_ptr<Account>> accounts;
    const std::string CUSTOMERS_FILE = "customers.txt";
    const std::string ACCOUNTS_FILE = "accounts.txt";
    int shift = 3;
public:
    Bank();
    ~Bank();
    std::shared_ptr<Customer> GetCustomer(int index) const;
    void AddCustomer(const std::string& name, const std::string& id, const std::string& password);
    void AddAccount(std::shared_ptr<Account> account);
    std::shared_ptr<Account> GetAccount(int index) const;
    void ShowAccounts() const;
    void ShowCustomers() const;
    bool ValidateLogin(const std::string& id, const std::string& password) const;
    void LoadData();
    void SaveData() const;
    void Transfer(int fromIndex, int toIndex, double amount);
    void ApplyInterestToAllSavings();
    void ShowAccountInfo(int index) const;
    double CalculateTotalAssets() const;
    void SortAccountsByBalance();
    void WithdrawFromAccount(int index, double amount);
    void DeleteCustomer(int index);
    void DeleteAccount(int index);
    std::string encryptCaesar(const std::string& text) const;
    std::string decryptCaesar(const std::string& text) const;

};

bool isValidId(const std::string& id);

#endif
