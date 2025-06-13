#include "cafeteria.h"
#include "payment.h"
#include <iostream>
#include <fstream>

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
    salesRecords.push_back(SalesRecord(itemName, customerName, quantity, price,
        currentDate.getDay(), currentDate.getMonth(), currentDate.getYear()));
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

        salesRecords.push_back(SalesRecord(itemName, customerName, quantity, price, day, month, year));
    }
    file.close();
}

void Cafeteria::saveSalesRecordsToFile() {
    std::ofstream file("sales_records.txt");
    if (!file.is_open()) return;

    for (int i = 0; i < salesRecords.size(); ++i) {
        file << salesRecords[i].getDate().getFormattedDate().c_str() << ","
            << salesRecords[i].getItemName().c_str() << ","
            << salesRecords[i].getQuantity() << ","
            << salesRecords[i].getUnitPrice() << ","
            << salesRecords[i].getCustomerName().c_str() << "\n";
    }
    file.close();
}

void Cafeteria::displayMenu() {
    menuManager.displayCompleteMenu();
}

void Cafeteria::placeOrder() {
    char name[100];
    std::cout << "Enter customer name: ";
    std::cin.ignore();
    std::cin.getline(name, 100);

    Customer* customer = customerManager.findCustomerByName(AYstr(name));
    if (!customer) {
        std::cout << "Customer not found! Please register first.\n";
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
        std::cout << "\nSelect menu item by number (0 to finish ordering): ";
        while (!(std::cin >> choice) || choice < 0 || choice > menuManager.getMenu().size()) {
            std::cin.clear();
            std::cin.ignore(10000, '\n');
            std::cout << "Invalid choice. Please enter a number between 0 and "
                << menuManager.getMenu().size() << ": ";
        }

        if (choice == 0) {
            break;
        }

        MenuItem* selectedItem = menuManager.getMenu()[choice - 1];

       
        std::cout << "\nSelected: ";
        selectedItem->getName().print();
        std::cout << " - $" << selectedItem->getPrice();
        std::cout << " (" << selectedItem->getStock() << " available)\n";

        
        int quantity;
        std::cout << "Enter quantity: ";
        while (!(std::cin >> quantity) || quantity <= 0) {
            std::cin.clear();
            std::cin.ignore(10000, '\n');
            std::cout << "Invalid quantity. Please enter a positive number: ";
        }

      
        if (!selectedItem->isAvailable(quantity)) {
            std::cout << "Error: Only " << selectedItem->getStock()
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

        std::cout << "Added to order: ";
        newItem.name.print();
        std::cout << " x" << quantity << "  $" << itemPrice
            << " = $" << itemTotal << "\n";
    }
    if (orderedItems.empty()) {
        std::cout << "No items selected. Order cancelled.\n";
        return;
    }

   
    std::cout << "\n--- Order Summary ---\n";
    std::cout << "Customer: " << name << "\n";
    std::cout << "Items:\n";

    for (int i = 0; i < orderedItems.size(); ++i) {
        std::cout << i + 1 << ". ";
        orderedItems[i].name.print();
        std::cout << " x" << orderedItems[i].quantity
            << " @ $" << orderedItems[i].unitPrice
            << " = $" << orderedItems[i].totalPrice << "\n";
        orderTotal += orderedItems[i].totalPrice;
    }

    
    double discount = 0.0;
    if (customer->getLoyaltyPoints() >= 100) {
        discount = orderTotal * 0.1; 
        std::cout << "Loyalty discount applied: -$" << discount << "\n";
        customer->resetLoyaltyPoints();
    }

    double finalTotal = orderTotal - discount;
    std::cout << "Subtotal: $" << orderTotal << "\n";
    if (discount > 0) {
        std::cout << "Discount: -$" << discount << "\n";
    }
    std::cout << "Total: $" << finalTotal << "\n";

    char confirm;
    std::cout << "\nConfirm order (y/n)? ";
    std::cin >> confirm;

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
        std::cout << "Order cancelled. Stock has been restored.\n";
        return;
    }
    bool paymentSuccessful = false;
    do {
        Payment::displayPaymentOptions();
        std::cout << "Enter payment method (1-6): ";
        int paymentChoice;
        std::cin >> paymentChoice;

        AYstr paymentMethod;
        switch (paymentChoice) {
        case 1: paymentMethod = "JazzCash"; break;
        case 2: paymentMethod = "EasyPaisa"; break;
        case 3: paymentMethod = "SadaPay"; break;
        case 4: paymentMethod = "NayaPay"; break;
        case 5: paymentMethod = "Bank Transfer"; break;
        case 6: paymentMethod = "Cash"; break;
        default:
            std::cout << "Invalid payment method!\n";
            continue;
        }

        AYstr accountNumber;
        if (!paymentMethod.isequal("Cash")) {
            char accNum[20];
            std::cout << "Enter account number: ";
            std::cin >> accNum;
            accountNumber = AYstr(accNum);
        }

        Payment payment(paymentMethod, accountNumber, finalTotal);
        paymentSuccessful = payment.processPayment();

        if (!paymentSuccessful) {
            std::cout << "Payment failed. Please try another method.\n";
        }
    } while (!paymentSuccessful);

 
    for (int i = 0; i < orderedItems.size(); ++i) {
       
        saveSalesRecord(orderedItems[i].name, orderedItems[i].quantity,
            orderedItems[i].unitPrice, AYstr(name));

        
        AYstr historyEntry = orderedItems[i].name + " x" + AYstr(orderedItems[i].quantity) +
            " @ $" + AYstr(orderedItems[i].unitPrice);
        customer->addOrderToHistory(historyEntry);
    }

   
    int pointsEarned = static_cast<int>(finalTotal / 10);
    customer->addLoyaltyPoints(pointsEarned);

    totalSales += finalTotal;
    menuManager.saveMenuToFile();
    saveSalesRecordsToFile();

    std::cout << "\nOrder placed successfully!\n";
    std::cout << "Earned " << pointsEarned << " loyalty points.\n";
    std::cout << "Total loyalty points: " << customer->getLoyaltyPoints() << "\n";
}
void Cafeteria::generateDailySalesReport() {
    double dailyTotal = 0.0;
    std::cout << "\nDaily Sales Report for ";
    currentDate.getFormattedDate().print();
    std::cout << "\n";

    for (int i = 0; i < salesRecords.size(); ++i) {
        if (salesRecords[i].isSameDate(currentDate.getDay(), currentDate.getMonth(), currentDate.getYear())) {
            salesRecords[i].print();
            dailyTotal += salesRecords[i].getTotal();
        }
    }

    std::cout << "Total Daily Sales: $" << dailyTotal << "\n";
}

void Cafeteria::generateMonthlySalesReport() {
    double monthlyTotal = 0.0;
    std::cout << "\nMonthly Sales Report for ";
    currentDate.getMonthName().print();
    std::cout << "\n";

    for (int i = 0; i < salesRecords.size(); ++i) {
        if (salesRecords[i].isSameDate(0, currentDate.getMonth(), currentDate.getYear())) {
            salesRecords[i].print();
            monthlyTotal += salesRecords[i].getTotal();
        }
    }

    std::cout << "Total Monthly Sales: $" << monthlyTotal << "\n";
}

void Cafeteria::generatePopularItemsReport() {
    MyVector<AYstr> itemNames;
    MyVector<int> itemCounts;

    for (int i = 0; i < salesRecords.size(); ++i) {
        bool found = false;
        for (int j = 0; j < itemNames.size(); ++j) {
            if (itemNames[j].isequal(salesRecords[i].getItemName())) {
                itemCounts[j] += salesRecords[i].getQuantity();
                found = true;
                break;
            }
        }
        if (!found) {
            itemNames.push_back(salesRecords[i].getItemName());
            itemCounts.push_back(salesRecords[i].getQuantity());
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
    do {
        std::cout << "\n--- Reporting Options ---\n";
        std::cout << "1. Daily Sales Report\n";
        std::cout << "2. Monthly Sales Report\n";
        std::cout << "3. Popular Items Report\n";
        std::cout << "4. Back to Main Menu\n";
        std::cout << "Enter choice: ";
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
    menuManager.displayMenuOptions();
}

void Cafeteria::customerManagement() {
    int choice;
    do {
        std::cout << "\n--- Customer Management ---\n";
        std::cout << "1. Register Customer\n";
        std::cout << "2. Update Customer Info\n";
        std::cout << "3. View Order History\n";
        std::cout << "4. Back to Main Menu\n";
        std::cout << "Enter choice: ";
        std::cin >> choice;

        switch (choice) {
        case 1: customerManager.registerCustomer(); break;
        case 2: customerManager.updateCustomerInfo(); break;
        case 3: customerManager.displayOrderHistory(); break;
        case 4: break;
        default: std::cout << "Invalid choice!\n";
        }
    } while (choice != 4);
}

void Cafeteria::saveData() {
    menuManager.saveMenuToFile();
    customerManager.saveAllCustomerData();
    saveSalesRecordsToFile();
}
void Cafeteria::displayOrderHistory() {
    customerManager.displayOrderHistory();
}