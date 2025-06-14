#include "Customer.h"
#include <fstream>

int Customer::idCounter = 0;

Customer::Customer(const AYstr& name, const AYstr& email, const AYstr& phone,
    const AYstr& address, const AYstr& dietaryPreference,
    const AYstr& password, double loyaltyPoints, int id)
{
    if (id == 0) {
        this->id = ++idCounter;
    }
    else {
        this->id = id;
        idCounter = (id > idCounter) ? id : idCounter;
    }

    this->name = name;
    this->email = email;
    this->phone = phone;
    this->address = address;
    this->dietaryPreference = dietaryPreference;
    this->password = password;
    this->loyaltyPoints = loyaltyPoints;
}
int Customer::getId() const { return id; }
AYstr Customer::getName() const { return name; }
AYstr Customer::getEmail() const { return email; }
AYstr Customer::getPreference() const { return dietaryPreference; }
double Customer::getLoyaltyPoints() const { return loyaltyPoints; }
AYstr Customer::getPhone() const { return phone; }
AYstr Customer::getAddress() const { return address; }
AYstr Customer::getPassword() const { return password; }
MyVector<AYstr> Customer::getOrderHistory() const { return orderHistory; }

void Customer::addLoyaltyPoints(double points) { loyaltyPoints += points; }
void Customer::resetLoyaltyPoints() { loyaltyPoints = 0.0; }
void Customer::addOrderToHistory(const AYstr& order) { orderHistory.push_back(order); }

void Customer::updateEmail(const AYstr& newEmail) {
    email = newEmail;
}

void Customer::updatePreference(const AYstr& newPreference) {
    dietaryPreference = newPreference;
}

void Customer::saveIdCounter() {
    std::ofstream idFile("id_counter.txt");
    if (idFile.is_open()) {
        idFile << idCounter;
        idFile.close();
    }
}

void Customer::loadIdCounter() {
    std::ifstream idFile("id_counter.txt");
    if (idFile.is_open()) {
        idFile >> idCounter;
        idFile.close();
    }
    else {
        idCounter = 0;
    }
}

void Customer::saveOrderHistory(std::ofstream& outFile) const {
    for (int i = 0; i < orderHistory.size(); ++i) {
        outFile << id << ",";
        orderHistory[i].print();
        outFile << "\n";
    }
}