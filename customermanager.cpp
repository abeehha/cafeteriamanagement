#include "CustomerManager.h"
#include <fstream>
#include <iostream>

CustomerManager::CustomerManager() {
    loadCustomersFromFile();
    loadOrderHistoryFromFile();
}

void CustomerManager::loadCustomersFromFile() {
    std::ifstream customerFile("customers.txt");
    if (!customerFile.is_open()) {
        std::cout << "Warning: Could not open customers.txt - starting with empty customer list\n";
        return;
    }

    AYstr line;
    char buffer[256];
    while (customerFile.getline(buffer, 256)) {
        line = buffer;

        int idStart = line.find_first("ID: ") + 4;
        int idEnd = line.find_first(',', idStart);
        AYstr idStr = line.substring(idStart, idEnd - idStart);
        int id = idStr.strtoint(idStr.c_str());

        int nameStart = line.find_first("Name: ", idEnd) + 6;
        int nameEnd = line.find_first(',', nameStart);
        AYstr name = line.substring(nameStart, nameEnd - nameStart).trim();

        int emailStart = line.find_first("Email: ", nameEnd) + 7;
        int emailEnd = line.find_first(',', emailStart);
        AYstr email = line.substring(emailStart, emailEnd - emailStart).trim();

        int prefStart = line.find_first("Dietary Preference: ", emailEnd) + 19;
        int prefEnd = line.find_first(',', prefStart);
        AYstr preference = line.substring(prefStart, prefEnd - prefStart).trim();

        int pointsStart = line.find_first("Loyalty Points: ", prefEnd) + 16;
        AYstr pointsStr = line.substring(pointsStart, line.strlength(line.c_str()) - pointsStart).trim();
        double points = pointsStr.strtodouble(pointsStr.c_str());

        customers.push_back(Customer(name, email, preference, points, id));
    }
    customerFile.close();
}

void CustomerManager::loadOrderHistoryFromFile() {
    std::ifstream orderFile("order_history.txt");
    if (!orderFile.is_open()) {
        return;
    }

    AYstr line;
    char buffer[256];
    while (orderFile.getline(buffer, 256)) {
        line = buffer;

        int commaPos = line.find_first(',');
        if (commaPos == -1) continue;

        AYstr idStr = line.substring(0, commaPos);
        int customerId = idStr.strtoint(idStr.c_str());

        AYstr orderItem = line.substring(commaPos + 1, line.strlength(line.c_str()) - commaPos - 1).trim();

        for (int i = 0; i < customers.size(); ++i) {
            if (customers[i].getId() == customerId) {
                customers[i].addOrderToHistory(orderItem);
                break;
            }
        }
    }
    orderFile.close();
}

void CustomerManager::registerCustomer() {
    char name[100], email[100];
    int preferenceChoice;

    bool validName = false;
    do {
        std::cout << "Enter customer name (minimum 2 characters): ";
        std::cin.ignore();
        std::cin.getline(name, 100);

        AYstr nameStr(name);
        if (nameStr.strlength(nameStr.c_str()) < 2) {
            std::cout << "Error: Name must be at least 2 characters long\n";
        }
        else if (findCustomerByName(nameStr)) {
            std::cout << "Error: Customer already exists with this name\n";
            return;
        }
        else {
            validName = true;
        }
    } while (!validName);

    bool validEmail = false;
    do {
        std::cout << "Enter email address: ";
        std::cin.getline(email, 100);

        AYstr emailStr(email);
        int atPos = emailStr.find_first('@');
        int dotPos = emailStr.find_first('.', atPos + 1);

        if (atPos == -1 || dotPos == -1 || atPos > dotPos) {
            std::cout << "Error: Invalid email format. Must contain '@' and '.' in correct order\n";
        }
        else {
            validEmail = true;
        }
    } while (!validEmail);

    std::cout << "\nSelect dietary preference:\n"
        << "1. Vegetarian\n"
        << "2. Vegan\n"
        << "3. Gluten-Free\n"
        << "4. None\n"
        << "Choice: ";
    std::cin >> preferenceChoice;

    AYstr preference;
    switch (preferenceChoice) {
    case 1: preference = "Vegetarian"; break;
    case 2: preference = "Vegan"; break;
    case 3: preference = "Gluten-Free"; break;
    default: preference = "None"; break;
    }

    customers.push_back(Customer(AYstr(name), AYstr(email), preference));
    saveCustomerToFile(customers.back());

    std::cout << "\nCustomer Registration Successful\n"
        << "-------------------------\n"
        << "ID: " << customers.back().getId() << "\n"
        << "Name: " << name << "\n"
        << "Email: " << email << "\n"
        << "Preference: ";
    preference.print();
    std::cout << "\n";
}

Customer* CustomerManager::findCustomerByName(const AYstr& name) {
    for (int i = 0; i < customers.size(); ++i) {
        if (customers[i].getName().isequal(name)) {
            return &customers[i];
        }
    }
    return nullptr;
}

void CustomerManager::saveCustomerToFile(const Customer& customer) {
    std::ofstream customerFile("customers.txt", std::ios::app);
    if (customerFile.is_open()) {
        customerFile << "ID: " << customer.getId()
            << ", Name: " << customer.getName().c_str()
            << ", Email: " << customer.getEmail().c_str()
            << ", Dietary Preference: " << customer.getPreference().c_str()
            << ", Loyalty Points: " << customer.getLoyaltyPoints() << "\n";
        customerFile.close();
    }
}

void CustomerManager::saveAllCustomerData() {
    std::ofstream customerFile("customers.txt");
    if (customerFile.is_open()) {
        for (int i = 0; i < customers.size(); ++i) {
            customerFile << "ID: " << customers[i].getId()
                << ", Name: " << customers[i].getName().c_str()
                << ", Email: " << customers[i].getEmail().c_str()
                << ", Dietary Preference: " << customers[i].getPreference().c_str()
                << ", Loyalty Points: " << customers[i].getLoyaltyPoints() << "\n";
        }
        customerFile.close();
    }
    Customer::saveIdCounter();
}

void CustomerManager::updateCustomerInfo() {
    char name[100];
    std::cout << "Enter customer name to update: ";
    std::cin.ignore();
    std::cin.getline(name, 100);

    Customer* customer = findCustomerByName(AYstr(name));
    if (!customer) {
        std::cout << "Customer not found!\n";
        return;
    }

    int choice;
    do {
        std::cout << "\n--- Update Customer Information ---\n";
        std::cout << "1. Update Email\n";
        std::cout << "2. Update Dietary Preference\n";
        std::cout << "3. Back to Main Menu\n";
        std::cout << "Enter your choice: ";
        std::cin >> choice;
        std::cin.ignore();

        switch (choice) {
        case 1: {
            char newEmail[100];
            bool validEmail = false;
            do {
                std::cout << "Enter new email: ";
                std::cin.getline(newEmail, 100);

                AYstr emailStr(newEmail);
                int atPos = emailStr.find_first('@');
                int dotPos = emailStr.find_first('.', atPos + 1);

                if (atPos == -1 || dotPos == -1 || atPos > dotPos) {
                    std::cout << "Error: Invalid email format. Must contain '@' and '.' in correct order\n";
                }
                else {
                    validEmail = true;
                    customer->updateEmail(AYstr(newEmail));
                }
            } while (!validEmail);
            break;
        }
        case 2: {
            int prefChoice;
            std::cout << "\nSelect new dietary preference:\n"
                << "1. Vegetarian\n"
                << "2. Vegan\n"
                << "3. Gluten-Free\n"
                << "4. None\n"
                << "Choice: ";
            std::cin >> prefChoice;

            AYstr newPreference;
            switch (prefChoice) {
            case 1: newPreference = "Vegetarian"; break;
            case 2: newPreference = "Vegan"; break;
            case 3: newPreference = "Gluten-Free"; break;
            default: newPreference = "None"; break;
            }
            customer->updatePreference(newPreference);
            break;
        }
        case 3:
            break;
        default:
            std::cout << "Invalid choice!\n";
        }
    } while (choice != 3);

    saveAllCustomerData();
}

void CustomerManager::displayOrderHistory() {
    char name[100];
    std::cout << "Enter customer name: ";
    std::cin.ignore();
    std::cin.getline(name, 100);

    Customer* customer = findCustomerByName(AYstr(name));
    if (!customer) {
        std::cout << "\nError: Customer not found!\n";
        return;
    }

    MyVector<AYstr> orderHistory = customer->getOrderHistory();
    std::cout << "\nOrder History for ";
    customer->getName().print();
    std::cout << ":\n";

    if (orderHistory.empty()) {
        std::cout << "No orders found.\n";
        return;
    }

    for (int i = 0; i < orderHistory.size(); ++i) {
        std::cout << i + 1 << ". ";
        orderHistory[i].print();
        std::cout << "\n";
    }
}