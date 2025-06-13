#pragma once
#include "MenuManager.h"
#include "CustomerManager.h"
#include "SalesRecord.h"
#include "raylib.h"

class Cafeteria {
private:
    double totalSales;
    Date currentDate;
    MyVector<SalesRecord> salesRecords;
    MenuManager menuManager;
    CustomerManager customerManager;

public:
    Cafeteria();

    // Menu management
    void menuManagement();
    void displayMenu();

    // Customer management
    void customerManagement();
    void suggestMenuItems(const Customer& customer);

    // Order processing
    void placeOrder();

    // Reporting
    void displayReportingOptions();
    void generateDailySalesReport();
    void generateMonthlySalesReport();
    void generatePopularItemsReport();

    // Data handling
    void saveData();
    void displayOrderHistory();

    // Sales records
    void saveSalesRecord(const AYstr& itemName, int quantity, double price, const AYstr& customerName);
    void loadSalesRecords();
    void saveSalesRecordsToFile();
};