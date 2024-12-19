#include "Bank.h"
#include "SavingsAccount.h"
#include "CheckingAccount.h"
#include "BusinessAccount.h"
#include <iostream>
#include <fstream>
#include <sstream>
#include <algorithm>
#include <iomanip>
#include <limits>


Bank::Bank() {
    if (customers.empty()) {
        AddCustomer("admin", "123456789", "admin");
    }
    LoadData();
}
Bank::~Bank() {
    SaveData();
}
std::shared_ptr<Customer> Bank::GetCustomer(int index) const {
    if (index < customers.size() && index >= 0)
        return customers[index];
    else return nullptr;
}

void Bank::AddCustomer(const std::string& name, const std::string& id, const std::string& password) {
    customers.push_back(std::make_shared<Customer>(name, id, encryptCaesar(password)));
}

void Bank::AddAccount(std::shared_ptr<Account> account) {
    accounts.push_back(account);
}

std::shared_ptr<Account> Bank::GetAccount(int index) const {
    if (index < accounts.size() && index >= 0)
        return accounts[index];
    else return nullptr;
}

void Bank::ShowAccounts() const {
    std::cout << "-------------------- Счета --------------------" << std::endl;
    for (const auto& account : accounts) {
        std::cout << *account;
        std::cout << "------------------------------------------------" << std::endl;
    }
}

void Bank::ShowCustomers() const {
    std::cout << "------------------ Клиенты -------------------" << std::endl;
    for (const auto& customer : customers) {
        customer->DisplayInfo();
    }
    std::cout << "------------------------------------------------" << std::endl;
}
bool Bank::ValidateLogin(const std::string& id, const std::string& password) const {
    for (const auto& customer : customers) {
        if (customer->getId() == id && decryptCaesar(customer->getPasswordHash()) == password)
            return true;
    }
    return false;
}

void Bank::LoadData() {
    std::ifstream cFile(CUSTOMERS_FILE);
    if (!cFile.is_open()) {
        throw std::runtime_error("Не удалось открыть файл клиентов для чтения: " + CUSTOMERS_FILE);
    }
    std::string line;
    while (std::getline(cFile, line)) {
        std::stringstream ss(line);
        std::string name, id, hash;
        if (!getline(ss, name, ',') || !getline(ss, id, ',') || !getline(ss, hash, ',')) {
            cFile.close();
            throw std::runtime_error("Ошибка формата данных в файле клиентов: " + CUSTOMERS_FILE);
        }
        customers.push_back(std::make_shared<Customer>(name, id, hash));
    }
    cFile.close();


    std::ifstream aFile(ACCOUNTS_FILE);
    if (!aFile.is_open()) {
        throw std::runtime_error("Не удалось открыть файл счетов для чтения: " + ACCOUNTS_FILE);
    }
    while (std::getline(aFile, line)) {
        std::stringstream ss(line);
        std::string typeStr;
        double bal;
        std::string ex;

        if (!getline(ss, typeStr, ',') || !(ss >> bal) || (ss.peek() == ',' && !getline(ss, ex, ','))) {
            aFile.close();
            throw std::runtime_error("Ошибка формата данных в файле счетов: " + ACCOUNTS_FILE);
        }
        try {
            if (typeStr == "1") AddAccount(std::make_shared<SavingsAccount>(bal, std::stod(ex)));
            else if (typeStr == "2") AddAccount(std::make_shared<CheckingAccount>(bal, std::stod(ex)));
            else if (typeStr == "3") AddAccount(std::make_shared<BusinessAccount>(bal));
        }
        catch (const std::invalid_argument& e) {
            aFile.close();
            throw std::runtime_error("Неверный формат данных в файле счетов: " + ACCOUNTS_FILE);
        }
    }
    aFile.close();
}

void Bank::SaveData() const {
    std::ofstream cFile(CUSTOMERS_FILE);
    if (!cFile.is_open()) {
        throw std::runtime_error("Не удалось открыть файл клиентов для записи: " + CUSTOMERS_FILE);
    }
    for (const auto& c : customers)
        cFile << c->name << "," << c->id << "," << c->getPasswordHash() << std::endl;
    cFile.close();

    std::ofstream aFile(ACCOUNTS_FILE);
    if (!aFile.is_open()) {
        throw std::runtime_error("Не удалось открыть файл счетов для записи: " + ACCOUNTS_FILE);
    }
    for (const auto& account : accounts) {
        if (auto savings = std::dynamic_pointer_cast<SavingsAccount>(account))
            aFile << "1," << std::fixed << std::setprecision(2) << savings->GetBalance() << "," << std::fixed << std::setprecision(2) << savings->GetInterestRate() << std::endl;
        else if (auto checking = std::dynamic_pointer_cast<CheckingAccount>(account))
            aFile << "2," << std::fixed << std::setprecision(2) << checking->GetBalance() << "," << std::fixed << std::setprecision(2) << checking->GetOverdraftLimit() << std::endl;
        else if (auto business = std::dynamic_pointer_cast<BusinessAccount>(account))
            aFile << "3," << std::fixed << std::setprecision(2) << business->GetBalance() << "," << std::endl;
    }
    aFile.close();
}
void Bank::Transfer(int fromIndex, int toIndex, double amount) {
    std::shared_ptr<Account> fromAccount = GetAccount(fromIndex);
    std::shared_ptr<Account> toAccount = GetAccount(toIndex);
    if (fromAccount && toAccount) {
        if (fromAccount->GetBalance() >= amount) {
            fromAccount->Withdraw(amount);
            toAccount->Deposit(amount);
            std::cout << "Перевод: " << std::fixed << std::setprecision(2) << amount << " выполнен со счета #"
                << fromIndex << " на счет #" << toIndex << std::endl;
        }
        else {
            std::cout << "Недостаточно средств для перевода!" << std::endl;
        }
    }
    else {
        std::cout << "Некорректные счета для перевода!" << std::endl;
    }
}
void Bank::ApplyInterestToAllSavings() {
    std::cout << "--- Начисление процентов на сберегательные счета ---" << std::endl;
    for (const auto& account : accounts) {
        if (std::dynamic_pointer_cast<SavingsAccount>(account)) {
            std::dynamic_pointer_cast<SavingsAccount>(account)->ApplyInterest();
        }
    }
}

void Bank::ShowAccountInfo(int index) const {
    std::shared_ptr<Account> account = GetAccount(index);
    if (account) {
        std::cout << "------- Информация об аккаунте #" << index << " -------" << std::endl;
        account->DisplayInfo();
        std::cout << "------------------------------------------------" << std::endl;
    }
    else {
        std::cout << "Аккаунт не найден!" << std::endl;
    }
}

double Bank::CalculateTotalAssets() const {
    double total = 0;
    for (const auto& account : accounts) {
        total += account->GetBalance();
    }
    return total;
}

void Bank::SortAccountsByBalance() {
    std::sort(accounts.begin(), accounts.end(), [](const auto& a, const auto& b) {
        return a->GetBalance() < b->GetBalance();
        });
    std::cout << "Счета отсортированы по балансу (от меньшего к большему)." << std::endl;
}

void Bank::WithdrawFromAccount(int index, double amount) {
    std::shared_ptr<Account> account = GetAccount(index);
    if (account) {
        account->Withdraw(amount);
    }
    else {
        std::cout << "Аккаунт не найден!" << std::endl;
    }
}

void Bank::DeleteCustomer(int index) {
    if (index < 0 || index >= customers.size()) {
        std::cout << "Некорректный индекс клиента!" << std::endl;
        return;
    }

    bool hasAccounts = false;
    for (const auto& account : accounts) {
        // Add logic to check if account belongs to the customer
        if (true) {
            hasAccounts = true;
            break;
        }
    }
    if (hasAccounts) {
        std::cout << "Нельзя удалить клиента, т.к. у него есть открытые счета." << std::endl;
    }
    else {
        customers.erase(customers.begin() + index);
        std::cout << "Клиент удален." << std::endl;
    }
}

void Bank::DeleteAccount(int index) {
    std::shared_ptr<Account> account = GetAccount(index);
    if (account) {
        if (account->GetBalance() != 0) {
            std::cout << "Нельзя удалить аккаунт с балансом не равным 0!" << std::endl;
        }
        else {
            accounts.erase(accounts.begin() + index);
            std::cout << "Аккаунт удален." << std::endl;
        }
    }
    else {
        std::cout << "Некорректный индекс аккаунта." << std::endl;
    }
}


std::string Bank::encryptCaesar(const std::string& text) const {
    std::string result = "";
    for (char c : text) {
        if (isalpha(c)) {
            char base = islower(c) ? 'a' : 'A';
            result += (char)((c - base + shift) % 26 + base);
        }
        else if (isdigit(c)) {
            result += (char)((c - '0' + shift) % 10 + '0');
        }
        else {
            result += c;
        }
    }
    return result;
}

std::string Bank::decryptCaesar(const std::string& text) const {
    std::string result = "";
    for (char c : text) {
        if (isalpha(c)) {
            char base = islower(c) ? 'a' : 'A';
            result += (char)((c - base - shift + 26) % 26 + base);
        }
        else if (isdigit(c)) {
            result += (char)((c - '0' - shift + 10) % 10 + '0');
        }
        else {
            result += c;
        }
    }
    return result;
}

bool isValidId(const std::string& id) {
    return id.length() == 9 && std::all_of(id.begin(), id.end(), ::isdigit);
}
