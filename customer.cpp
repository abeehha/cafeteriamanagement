#include "Customer.h"
#include <fstream>

int Customer::idCounter = 0;



Customer::Customer(const AYstr& name, const AYstr& email, const AYstr& dietaryPreference, double loyaltyPoints, int id)
    : name(name), email(email), dietaryPreference(dietaryPreference), loyaltyPoints(loyaltyPoints) {
    if (id == 0) {
        this->id = ++idCounter;
    }
    else {
        this->id = id;
        idCounter = (idCounter > id) ? idCounter : id;
    }
}

int Customer::getId() const { return id; }
AYstr Customer::getName() const { return name; }
AYstr Customer::getEmail() const { return email; }
AYstr Customer::getPreference() const { return dietaryPreference; }
double Customer::getLoyaltyPoints() const { return loyaltyPoints; }
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