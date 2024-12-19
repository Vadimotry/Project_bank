#include "Customer.h"

Customer::Customer(const std::string& n, const std::string& i, const std::string& p)
    : name(n), id(i), passwordHash(p) {
}

std::string Customer::getName() const { return name; }
std::string Customer::getId() const { return id; }
std::string Customer::getPasswordHash() const { return passwordHash; }
void Customer::DisplayInfo() const {
    std::cout << "Клиент: " << name << ", ID: " << id << std::endl;
}
