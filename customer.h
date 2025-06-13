#ifndef CUSTOMER_H
#define CUSTOMER_H

#include "aystr.h"
#include "myvector.h"
#include <fstream>

class Customer {
private:
    static int idCounter;
    int id;
    AYstr name;
    AYstr email;
    AYstr dietaryPreference;
    double loyaltyPoints;
    MyVector<AYstr> orderHistory;

public:
    Customer() = default;
    Customer(const AYstr& name, const AYstr& email, const AYstr& dietaryPreference, double loyaltyPoints = 0.0, int id = 0);
    int getId() const;
    AYstr getName() const;
    AYstr getEmail() const;
    AYstr getPreference() const;
    double getLoyaltyPoints() const;
    MyVector<AYstr> getOrderHistory() const;

    void addLoyaltyPoints(double points);
    void resetLoyaltyPoints();
    void addOrderToHistory(const AYstr& order);
    void updateEmail(const AYstr& newEmail);
    void updatePreference(const AYstr& newPreference);

    static void saveIdCounter();
    static void loadIdCounter();
    void saveOrderHistory(std::ofstream& outFile) const;
};

#endif
