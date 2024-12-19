#ifndef CUSTOMER_H
#define CUSTOMER_H

#include <string>
#include <iostream>

class Customer {
public:
    std::string name, id;
private:
    std::string passwordHash;
public:
    Customer(const std::string& n, const std::string& i, const std::string& p);
    std::string getName() const;
    std::string getId() const;
    std::string getPasswordHash() const;
    void DisplayInfo() const;
};

#endif
