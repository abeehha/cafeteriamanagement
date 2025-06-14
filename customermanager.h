#ifndef CUSTOMERMANAGER_H
#define CUSTOMERMANAGER_H

#include "Customer.h"
#include "myvector.h"

class CustomerManager {
public:
    MyVector<Customer> customers;

    void loadCustomersFromFile();
    void loadOrderHistoryFromFile();
    CustomerManager();
    void registerCustomer();
    Customer* findCustomerByName(const AYstr& name);
    void saveCustomerToFile(const Customer& customer);
    void saveAllCustomerData();
    void updateCustomerInfo();
    void displayOrderHistory();
    Customer* authenticateCustomer(const AYstr& email, const AYstr& password);
};

#endif