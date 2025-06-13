#ifndef CAFETERIA_H
#define CAFETERIA_H

#include "MenuManager.h"
#include "CustomerManager.h"
#include "Date.h"
#include "SalesRecord.h"
#include "myvector.h"

class Cafeteria {
private:
    MenuManager menuManager;
    CustomerManager customerManager;
    MyVector<SalesRecord> salesRecords;
    Date currentDate;
    double totalSales;

    void suggestMenuItems(const Customer& customer);
    void saveSalesRecord(const AYstr& itemName, int quantity, double price, const AYstr& customerName);
    void loadSalesRecords();
    void saveSalesRecordsToFile();

public:
    Cafeteria();
    void displayMenu();
    void placeOrder();
    void generateDailySalesReport();
    void generateMonthlySalesReport();
    void generatePopularItemsReport();
    void displayReportingOptions();
    void menuManagement();
    void customerManagement();
    void saveData();
    void displayOrderHistory();
};

#endif
