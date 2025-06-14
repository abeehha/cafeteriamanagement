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
    AYstr phone;  
    AYstr address; 
    AYstr dietaryPreference;
    double loyaltyPoints;
    MyVector<AYstr> orderHistory;
    AYstr password;

public:
    Customer() = default;
    Customer(const AYstr& name, const AYstr& email, const AYstr& phone,
        const AYstr& address, const AYstr& password, const AYstr& dietaryPreference, double loyaltypoints=0.0, int id=0);
    AYstr getName() const;
    AYstr getEmail() const;
    AYstr getPreference() const;
    double getLoyaltyPoints() const;
    int getId()const;
    AYstr getPhone() const;
    AYstr getAddress() const;
    AYstr getPassword() const;
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
