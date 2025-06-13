#ifndef CUSTOMERMANAGER_H
#define CUSTOMERMANAGER_H

#include "Customer.h"
#include "myvector.h"

class CustomerManager {
protected:
    MyVector<Customer> customers;

    void loadCustomersFromFile();
    void loadOrderHistoryFromFile();

public:
    CustomerManager();
    void registerCustomer();
    Customer* findCustomerByName(const AYstr& name);
    void saveCustomerToFile(const Customer& customer);
    void saveAllCustomerData();
    void updateCustomerInfo();
    void displayOrderHistory();
    MyVector<Customer>& getCustomers();
};

#endif