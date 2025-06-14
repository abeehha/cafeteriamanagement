#include "cafeteria.h"
#include "payment.h"
#include "date.h"
#include "salesrecord.h"

#include <iostream>
#include <fstream>
#include <iomanip>

using namespace std;

Cafeteria::Cafeteria() : totalSales(0.0) {
    currentDate = Date::getCurrentDate();
    loadSalesRecords();
}
void Cafeteria::suggestMenuItems(const Customer& customer) {
    AYstr preference = customer.getPreference();
    MyVector<AYstr> history = customer.getOrderHistory();
    
    std::cout << "\n--- Personalized Suggestions ---\n";

    if (history.empty()) {
        std::cout << "Welcome new customer!\n";
        if (!preference.isequal("None")) {
            std::cout << "Based on your " << preference.c_str() << " preference:\n";
            bool found = false;
            for (int i = 0; i < menuManager.getMenu().size(); ++i) {
                if (menuManager.getMenu()[i]->getCategory().isequal(preference)) {
                    std::cout << "- ";
                    menuManager.getMenu()[i]->getName().print();
                    std::cout << "\n";
                    found = true;
                }
            }
            if (!found) {
                std::cout << "No items match your preference.\n";
            }
        }
        return;
    }

    if (!preference.isequal("None")) {
        std::cout << "Recommended based on your " << preference.c_str() << " preference:\n";
        for (int i = 0; i < menuManager.getMenu().size(); ++i) {
            if (menuManager.getMenu()[i]->getCategory().isequal(preference)) {
                std::cout << "- ";
                menuManager.getMenu()[i]->getName().print();
                std::cout << "\n";
            }
        }
    }
}

void Cafeteria::saveSalesRecord(const AYstr& itemName, int quantity, double price, const AYstr& customerName) {
    Customer* customer = customerManager.findCustomerByName(customerName);
    if (customer) {
        salesrecords.push_back(SalesRecord(itemName, customerName, quantity, price,
            currentDate.getDay(), currentDate.getMonth(), currentDate.getYear()));

        
        std::ofstream file("sales_records.txt", std::ios::app);
        if (file.is_open()) {
            file << currentDate.getFormattedDate().c_str() << ","
                << itemName.c_str() << ","
                << quantity << ","
                << price << ","
                << customerName.c_str() << "\n";
            file.close();
        }
    }
}
void Cafeteria::loadSalesRecords() {
    std::ifstream file("sales_records.txt");
    if (!file.is_open()) return;

    char buffer[256];
    while (file.getline(buffer, 256)) {
        AYstr line(buffer);
        int commaPos[4];
        int pos = 0;

        for (int i = 0; i < 4; i++) {
            pos = line.find_first(',', pos);
            if (pos == -1) break;
            commaPos[i] = pos;
            pos++;
        }

        if (commaPos[0] == -1 || commaPos[1] == -1 || commaPos[2] == -1 || commaPos[3] == -1) continue;

        AYstr dateStr = line.substring(0, commaPos[0]);
        AYstr itemName = line.substring(commaPos[0] + 1, commaPos[1] - commaPos[0] - 1);
        AYstr quantityStr = line.substring(commaPos[1] + 1, commaPos[2] - commaPos[1] - 1);
        AYstr priceStr = line.substring(commaPos[2] + 1, commaPos[3] - commaPos[2] - 1);
        AYstr customerName = line.substring(commaPos[3] + 1, line.strlength(line.c_str()) - commaPos[3] - 1);

        int dashPos1 = dateStr.find_first('-');
        int dashPos2 = dateStr.find_first('-', dashPos1 + 1);
        if (dashPos1 == -1 || dashPos2 == -1) continue;

        AYstr dayStr = dateStr.substring(0, dashPos1);
        AYstr monthStr = dateStr.substring(dashPos1 + 1, dashPos2 - dashPos1 - 1);
        AYstr yearStr = dateStr.substring(dashPos2 + 1, dateStr.strlength(dateStr.c_str()) - dashPos2 - 1);

        int day = dayStr.strtoint(dayStr.c_str());
        int month = monthStr.strtoint(monthStr.c_str());
        int year = yearStr.strtoint(yearStr.c_str());
        int quantity = quantityStr.strtoint(quantityStr.c_str());
        double price = priceStr.strtodouble(priceStr.c_str());

        salesrecords.push_back(SalesRecord(itemName, customerName, quantity, price, day, month, year));
    }
    file.close();
}

void Cafeteria::saveSalesRecordsToFile() {
    std::ofstream file("sales_records.txt");
    if (!file.is_open()) return;

    for (int i = 0; i < salesrecords.size(); ++i) {
        file << salesrecords[i].getDate().getFormattedDate().c_str() << ","
            << salesrecords[i].getItemName().c_str() << ","
            << salesrecords[i].getQuantity() << ","
            << salesrecords[i].getUnitPrice() << ","
            << salesrecords[i].getCustomerName().c_str() << "\n";
    }
    file.close();
}

void Cafeteria::displayMenu() {
    menuManager.displayCompleteMenu();
}
void Cafeteria::placeOrder() {
    system("cls");
    Customer* customer = nullptr;
    char email[100], password[50];
    int authChoice;

  
    do {
        cout << "\n ________________________________________ ";
        cout << "\n|          LOGIN/SIGNUP OPTIONS           |";
        cout << "\n|----------------------------------------|";
        cout << "\n|  1. LOGIN                              |";
        cout << "\n|  2. SIGNUP                             |";
        cout << "\n|  3. BACK TO MAIN MENU                  |";
        cout << "\n|________________________________________|";
        cout << "\n\nEnter your choice (1-3): ";
        cin >> authChoice;

        switch (authChoice) {
        case 1: { 
            cin.ignore();
            cout << "Enter email: ";
            cin.getline(email, 100);
            cout << "Enter password: ";
            cin.getline(password, 50);

            customer = customerManager.authenticateCustomer(AYstr(email), AYstr(password));
            if (!customer) {
                cout << "Login failed. Invalid email or password.\n";
            }
            break;
        }
        case 2: { 
            customerManager.registerCustomer();
            
            cout << "\nRegistration successful! Please login to place an order.\n";
            break;
        }
        case 3: 
            return;
        default:
            cout << "Invalid choice. Please try again.\n";
        }
    } while (!customer && authChoice != 3);

    if (!customer) {
        return;
    }

   
    suggestMenuItems(*customer);
    menuManager.displayCompleteMenu();

    struct OrderedItem {
        AYstr name;
        int quantity;
        double unitPrice;
        double totalPrice;
    };

    MyVector<OrderedItem> orderedItems;
    double orderTotal = 0.0;

   
    while (true) {
        int choice;
        cout << "\nSelect menu item by number (0 to finish ordering): ";
        while (!(cin >> choice) || choice < 0 || choice > menuManager.getMenu().size()) {
            cin.clear();
            cin.ignore(10000, '\n');
            cout << "Invalid choice. Please enter a number between 0 and "
                << menuManager.getMenu().size() << ": ";
        }

        if (choice == 0) {
            break;
        }

        MenuItem* selectedItem = menuManager.getMenu()[choice - 1];
        cout << "\nSelected: ";
        selectedItem->getName().print();
        cout << " - $" << selectedItem->getPrice();
        cout << " (" << selectedItem->getStock() << " available)\n";

        int quantity;
        cout << "Enter quantity: ";
        while (!(cin >> quantity) || quantity <= 0) {
            cin.clear();
            cin.ignore(10000, '\n');
            cout << "Invalid quantity. Please enter a positive number: ";
        }

        if (!selectedItem->isAvailable(quantity)) {
            cout << "Error: Only " << selectedItem->getStock()
                << " available. Please choose a smaller quantity.\n";
            continue;
        }

        double customizationCost = menuManager.handleCustomizations();
        double itemPrice = selectedItem->getPrice() + customizationCost;
        double itemTotal = itemPrice * quantity;

        OrderedItem newItem;
        newItem.name = selectedItem->getName();
        newItem.quantity = quantity;
        newItem.unitPrice = itemPrice;
        newItem.totalPrice = itemTotal;
        orderedItems.push_back(newItem);

        selectedItem->setStock(selectedItem->getStock() - quantity);

        cout << "Added to order: ";
        newItem.name.print();
        cout << " x" << quantity << "  $" << itemPrice
            << " = $" << itemTotal << "\n";
    }

    if (orderedItems.empty()) {
        cout << "No items selected. Order cancelled.\n";
        return;
    }

  
    cout << "\n--- Order Summary ---\n";
    cout << "Customer: ";
    customer->getName().print();
    cout << "\nItems:\n";

    for (int i = 0; i < orderedItems.size(); ++i) {
        cout << i + 1 << ". ";
        orderedItems[i].name.print();
        cout << " x" << orderedItems[i].quantity
            << " @ $" << orderedItems[i].unitPrice
            << " = $" << orderedItems[i].totalPrice << "\n";
        orderTotal += orderedItems[i].totalPrice;
    }

   
    double discount = 0.0;
    if (customer->getLoyaltyPoints() >= 5) {
        discount = orderTotal * 0.1;
        cout << "Loyalty discount applied: -$" << discount << "\n";
        customer->resetLoyaltyPoints();
    }

    double finalTotal = orderTotal - discount;
    cout << "Subtotal: $" << orderTotal << "\n";
    if (discount > 0) {
        cout << "Discount: -$" << discount << "\n";
    }
    cout << "Total: $" << finalTotal << "\n";

   
    char confirm;
    cout << "\nConfirm order (y/n)? ";
    cin >> confirm;
    cin.ignore();

    if (tolower(confirm) != 'y') {
        
        for (int i = 0; i < orderedItems.size(); ++i) {
            for (int j = 0; j < menuManager.getMenu().size(); ++j) {
                if (menuManager.getMenu()[j]->getName().isequal(orderedItems[i].name)) {
                    menuManager.getMenu()[j]->setStock(
                        menuManager.getMenu()[j]->getStock() + orderedItems[i].quantity);
                    break;
                }
            }
        }
        cout << "Order cancelled. Stock has been restored.\n";
        return;
    }

   
    bool paymentSuccessful = false;
    do {
        Payment::displayPaymentOptions();
        cout << "Enter payment method (1-6): ";
        int paymentChoice;
        cin >> paymentChoice;
        cin.ignore();

        AYstr paymentMethod;
        switch (paymentChoice) {
        case 1: paymentMethod = "JazzCash"; break;
        case 2: paymentMethod = "EasyPaisa"; break;
        case 3: paymentMethod = "SadaPay"; break;
        case 4: paymentMethod = "NayaPay"; break;
        case 5: paymentMethod = "Bank Transfer"; break;
        case 6: paymentMethod = "Cash"; break;
        default:
            cout << "Invalid payment method!\n";
            continue;
        }

        AYstr accountNumber;
        if (!paymentMethod.isequal("Cash")) {
            char accNum[20];
            cout << "Enter account number: ";
            cin.getline(accNum, 20);
            accountNumber = AYstr(accNum);
        }

        Payment payment(paymentMethod, accountNumber, finalTotal);
        paymentSuccessful = payment.processPayment();

        if (!paymentSuccessful) {
            cout << "Payment failed. Please try another method.\n";
        }
    } while (!paymentSuccessful);

    for (int i = 0; i < orderedItems.size(); ++i) {
        saveSalesRecord(orderedItems[i].name, orderedItems[i].quantity,
            orderedItems[i].unitPrice, customer->getName());

       
        AYstr historyEntry = orderedItems[i].name + " x" + AYstr(orderedItems[i].quantity) +
            " @ $" + AYstr(orderedItems[i].unitPrice);
        customer->addOrderToHistory(historyEntry);

        
        std::ofstream historyFile("order_history.txt", std::ios::app);
        if (historyFile.is_open()) {
            historyFile << customer->getId() << "," << historyEntry.c_str() << "\n";
            historyFile.close();
        }
    }
    int pointsEarned = static_cast<int>(finalTotal / 10);
    customer->addLoyaltyPoints(pointsEarned);

    totalSales += finalTotal;
    menuManager.saveMenuToFile();
    saveSalesRecordsToFile();

    cout << "\nOrder placed successfully!\n";
    cout << "Earned " << pointsEarned << " loyalty points.\n";
    cout << "Total loyalty points: " << customer->getLoyaltyPoints() << "\n";
}
void Cafeteria::generateDailySalesReport() {

    Date cdate =Date::getCurrentDate();
   
    cout << "Daily Sales Report for ";
    cdate.getFormattedDate().print();

    double dailyTotal = 0.0;
    int orderCount = 0;

    MyVector<AYstr> itemNames;
    MyVector<int> itemQuantities;
    MyVector<double> itemRevenues;


    for (int i = 0; i < salesrecords.size(); ++i) {
        if (salesrecords[i].isSameDate(cdate.getDay(), cdate.getMonth(), cdate.getYear())) {
            dailyTotal += salesrecords[i].getTotal();
            orderCount++;

            bool itemFound = false;
            for (int j = 0; j < itemNames.size(); ++j) {
                if (itemNames[j].isequal(salesrecords[i].getItemName())) {
                    itemQuantities[j] += salesrecords[i].getQuantity();
                    itemRevenues[j] += salesrecords[i].getTotal();
                    itemFound = true;
                    break;
                }
            }

            if (!itemFound) {
                itemNames.push_back(salesrecords[i].getItemName());
                itemQuantities.push_back(salesrecords[i].getQuantity());
                itemRevenues.push_back(salesrecords[i].getTotal());
            }
        }
    }
    cout << "\nSUMMARY:\n";
    cout << "--------\n";
    cout << left << setw(25) << "Total Orders:" << right << setw(15) << orderCount << endl;
    cout << left << setw(25) << "Total Revenue:" << "$" << right << setw(14) << fixed << setprecision(2) << dailyTotal << endl;

    if (orderCount == 0) {
        cout << "\nNo sales recorded for today.\n";
        return;
    }

    cout << left << setw(25) << "Average Order Value:" << "$" << right << setw(14) << dailyTotal / orderCount << endl;

   
    cout << "\nITEM-WISE SALES:\n";
    cout << "-----------------------------------------------------------\n";
    cout << left << setw(30) << "Item Name"
        << right << setw(10) << "Qty"
        << right << setw(15) << "Revenue" << endl;
    cout << "-----------------------------------------------------------\n";

    for (int i = 0; i < itemNames.size(); ++i) {
        cout << left << setw(30) << itemNames[i].c_str()
            << right << setw(10) << itemQuantities[i]
            << right << setw(15) << fixed << setprecision(2) << itemRevenues[i] << endl;
    }
}

void Cafeteria::generateMonthlySalesReport() {
    cout << "Monthly Sales Report for ";
    currentDate.getMonthName().print();
    cout << " " << currentDate.getYear() << "\n";
    double monthlyTotal = 0.0;
    int orderCount = 0;

    MyVector<AYstr> uniqueCustomers;

    MyVector<AYstr> itemNames;
    MyVector<int> itemQuantities;
    MyVector<double> itemRevenues;
    MyVector<int> days;
    MyVector<double> dailyTotals;

    for (int i = 0; i < salesrecords.size(); ++i) {
        if (salesrecords[i].isSameMonth(currentDate.getMonth(), currentDate.getYear())) {
            monthlyTotal += salesrecords[i].getTotal();
            orderCount++;

            bool customerExists = false;
            for (int j = 0; j < uniqueCustomers.size(); ++j) {
                if (uniqueCustomers[j].isequal(salesrecords[i].getCustomerName())) {
                    customerExists = true;
                    break;
                }
            }
            if (!customerExists) {
                uniqueCustomers.push_back(salesrecords[i].getCustomerName());
            }
               bool itemFound = false;
            for (int j = 0; j < itemNames.size(); ++j) {
                if (itemNames[j].isequal(salesrecords[i].getItemName())) {
                    itemQuantities[j] += salesrecords[i].getQuantity();
                    itemRevenues[j] += salesrecords[i].getTotal();
                    itemFound = true;
                    break;
                }
            }
            if (!itemFound) {
                itemNames.push_back(salesrecords[i].getItemName());
                itemQuantities.push_back(salesrecords[i].getQuantity());
                itemRevenues.push_back(salesrecords[i].getTotal());
            }
            int day = salesrecords[i].getDate().getDay();
            bool dayFound = false;
            for (int j = 0; j < days.size(); ++j) {
                if (days[j] == day) {
                    dailyTotals[j] += salesrecords[i].getTotal();
                    dayFound = true;
                    break;
                }
            }
            if (!dayFound) {
                days.push_back(day);
                dailyTotals.push_back(salesrecords[i].getTotal());
            }
        }
    }

    cout << "\nSummary:\n";
    cout << "--------\n";
    cout << "Total Orders: " << orderCount << endl;
    cout << "Total Revenue: $" << fixed << setprecision(2) << monthlyTotal << endl;
    cout << "Unique Customers: " << uniqueCustomers.size() << endl;

    if (orderCount == 0) {
        cout << "\nNo sales recorded for this month.\n";
        return;
    }

    cout << "Average Order Value: $" << monthlyTotal / orderCount << endl;
    for (int i = 0; i < itemNames.size() - 1; ++i) {
        for (int j = 0; j < itemNames.size() - i - 1; ++j) {
            if (itemRevenues[j] < itemRevenues[j + 1]) {
                AYstr tempName = itemNames[j];
                itemNames[j] = itemNames[j + 1];
                itemNames[j + 1] = tempName;

                int tempQty = itemQuantities[j];
                itemQuantities[j] = itemQuantities[j + 1];
                itemQuantities[j + 1] = tempQty;

                double tempRev = itemRevenues[j];
                itemRevenues[j] = itemRevenues[j + 1];
                itemRevenues[j + 1] = tempRev;
            }
        }
    }

    cout << "\nTOP SELLING ITEMS:\n";
    cout << "----------------------------------------------------\n";
    cout << left << setw(30) << "Item Name"
        << right << setw(10) << "Qty"
        << right << setw(15) << "Revenue" << endl;
    cout << "-----------------------------------------------------\n";

    int displayCount = min(5, itemNames.size());
    for (int i = 0; i < displayCount; ++i) {
        cout << left << setw(30) << itemNames[i].c_str()
            << right << setw(10) << itemQuantities[i]
            << right << setw(15) << fixed << setprecision(2) << itemRevenues[i] << endl;
    }
    for (int i = 0; i < days.size() - 1; ++i) {
        for (int j = 0; j < days.size() - i - 1; ++j) {
            if (days[j] > days[j + 1]) {
               
                int tempDay = days[j];
                days[j] = days[j + 1];
                days[j + 1] = tempDay;

               
                double tempTotal = dailyTotals[j];
                dailyTotals[j] = dailyTotals[j + 1];
                dailyTotals[j + 1] = tempTotal;
            }
        }
    }

    
    cout << "\nDaily Breakdown:\n";
    cout << "---------------\n";
    cout << setw(10) << left << "Day"
        << setw(15) << right << "Revenue" << endl;
    cout << string(25, '-') << endl;

    for (int i = 0; i < days.size(); ++i) {
        cout << setw(10) << left << days[i]
            << setw(15) << right << fixed << setprecision(2) << dailyTotals[i] << endl;
    }
}
void Cafeteria::generatePopularItemsReport() {
    MyVector<AYstr> itemNames;
    MyVector<int> itemCounts;

    for (int i = 0; i < salesrecords.size(); ++i) {
        bool found = false;
        for (int j = 0; j < itemNames.size(); ++j) {
            if (itemNames[j].isequal(salesrecords[i].getItemName())) {
                itemCounts[j] += salesrecords[i].getQuantity();
                found = true;
                break;
            }
        }
        if (!found) {
            itemNames.push_back(salesrecords[i].getItemName());
            itemCounts.push_back(salesrecords[i].getQuantity());
        }
    }
    for (int i = 0; i < itemNames.size() - 1; ++i) {
        for (int j = 0; j < itemNames.size() - i - 1; ++j) {
            if (itemCounts[j] < itemCounts[j + 1]) {
                int tempCount = itemCounts[j];
                itemCounts[j] = itemCounts[j + 1];
                itemCounts[j + 1] = tempCount;

                AYstr tempName = itemNames[j];
                itemNames[j] = itemNames[j + 1];
                itemNames[j + 1] = tempName;
            }
        }
    }

    std::cout << "\nMost Popular Items:\n";
    for (int i = 0; i < itemNames.size() && i < 5; ++i) {
        itemNames[i].print();
        std::cout << " - Sold " << itemCounts[i] << " times\n";
    }
}

void Cafeteria::displayReportingOptions() {
    int choice;
    system("cls");
    do {
        cout << "\n ________________________________________ ";
        cout << "\n|          REPORTING OPTIONS             |";
        cout << "\n|----------------------------------------|";
        cout << "\n|  1. DAILY SALES REPORT                 |";
        cout << "\n|  2. MONTHLY SALES REPORT               |";
        cout << "\n|  3. POPULAR ITEMS REPORT               |";
        cout << "\n|  4. BACK TO MAIN MENU                  |";
        cout << "\n|________________________________________|";
        cout << "\n\nEnter your choice (1-4): ";
        std::cin >> choice;

        switch (choice) {
        case 1: generateDailySalesReport(); break;
        case 2: generateMonthlySalesReport(); break;
        case 3: generatePopularItemsReport(); break;
        case 4: break;
        default: std::cout << "Invalid choice!\n";
        }
    } while (choice != 4);
}

void Cafeteria::menuManagement() {
    system("cls");
    menuManager.displayMenuOptions();
}

void Cafeteria::saveData() {
    menuManager.saveMenuToFile();
    customerManager.saveAllCustomerData();
    saveSalesRecordsToFile();
}
void Cafeteria::displayOrderHistory() {
    system("cls");
    customerManager.displayOrderHistory();
}
void Cafeteria::customerManagement() {
    int choice;
    system("cls");
    do {
        cout << "\n ________________________________________ ";
        cout << "\n|       CUSTOMER MANAGEMENT SYSTEM       |";
        cout << "\n|----------------------------------------|";
        cout << "\n|  1. VIEW ALL CUSTOMERS                 |";
        cout << "\n|  2. VIEW CUSTOMER DETAILS              |";
        cout << "\n|  3. REMOVE CUSTOMER                    |";
        cout << "\n|  4. BACK TO ADMIN MENU                 |";
        cout << "\n|________________________________________|";
        cout << "\n\nEnter your choice (1-4): ";
        cin >> choice;

        switch (choice) {
        case 1: displayAllCustomers(); break;
        case 2: viewCustomerDetails(); break;
        case 3: removeCustomer(); break;
        case 4: break;
        default: cout << "Invalid choice!\n";
        }
    } while (choice != 4);
}

void Cafeteria::displayAllCustomers() {
    for (int i = 0; i < customerManager.customers.size(); ++i) {
        Customer& c = customerManager.customers[i];
        cout << "ID: " << c.getId() << " | Name: " << c.getName().c_str()
            << " | Email: " << c.getEmail().c_str()
            << " | Orders: " << c.getOrderHistory().size() << endl;
    }
}

void Cafeteria::viewCustomerDetails() {
    char name[100];
    cout << "Enter customer name: ";
    cin.ignore();
    cin.getline(name, 100);

    Customer* c = customerManager.findCustomerByName(AYstr(name));
    if (!c) {
        cout << "Customer not found!\n";
        return;
    }

    cout << "\n--- CUSTOMER DETAILS ---\n";
    cout << "ID: " << c->getId() << endl;
    cout << "Name: " << c->getName().c_str() << endl;
    cout << "Email: " << c->getEmail().c_str() << endl;
    cout << "Phone: " << c->getPhone().c_str() << endl;
    cout << "Address: " << c->getAddress().c_str() << endl;
    cout << "Dietary Preference: " << c->getPreference().c_str() << endl;
    cout << "Loyalty Points: " << c->getLoyaltyPoints() << endl;
    cout << "Total Orders: " << c->getOrderHistory().size() << endl;

    cout << "\nOrder History:\n";
    MyVector<AYstr> history = c->getOrderHistory();
    for (int i = 0; i < history.size(); ++i) {
        cout << i + 1 << ". " << history[i].c_str() << endl;
    }
}

void Cafeteria::removeCustomer() {
    char name[100];
    cout << "Enter customer name to remove: ";
    cin.ignore();
    cin.getline(name, 100);

    for (int i = 0; i < customerManager.customers.size(); ++i) {
        if (customerManager.customers[i].getName().isequal(AYstr(name))) {
            customerManager.customers.erase_at(i);
            cout << "Customer removed successfully.\n";
            customerManager.saveAllCustomerData();
            return;
        }
    }
    cout << "Customer not found!\n";
}