#ifndef CAFETERIA_H
#define CAFETERIA_H

#include "MenuManager.h"
#include "CustomerManager.h"
#include "SalesRecord.h"

class Cafeteria {
private:
    double totalSales;
    Date currentDate;
    MyVector<SalesRecord> salesrecords;
    MenuManager menuManager;
    CustomerManager customerManager;

public:
    Cafeteria();
    void menuManagement();
    void displayMenu();
    void customerManagement();
    void suggestMenuItems(const Customer& customer);
    void placeOrder();
    void displayReportingOptions();
    void generateDailySalesReport();
    void generateMonthlySalesReport();
    void generatePopularItemsReport();
    void saveData();
    void displayOrderHistory();
    void saveSalesRecord(const AYstr& itemName, int quantity, double price, const AYstr& customerName);
    void loadSalesRecords();
    void saveSalesRecordsToFile();
    void displayAllCustomers();
    void viewCustomerDetails();
    void removeCustomer();
};

#endif