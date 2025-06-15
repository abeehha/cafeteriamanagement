#include "CustomerManager.h"
#include "CustomExceptions.h"

#include <fstream>
#include <iostream>
using namespace std;

CustomerManager::CustomerManager() {
    loadCustomersFromFile();
    loadOrderHistoryFromFile();
}

void CustomerManager::loadOrderHistoryFromFile() {
    std::ifstream orderFile("order_history.txt");
    if (!orderFile.is_open()) {
        return;
    }

    char buffer[256];
    while (orderFile.getline(buffer, 256)) {
        AYstr line(buffer);
        int commaPos = line.find_first(',');
        if (commaPos == -1) continue;

        AYstr idStr = line.substring(0, commaPos);
        int customerId = idStr.strtoint(idStr.c_str());

        AYstr orderItem = line.substring(commaPos + 1, line.strlength() - commaPos - 1).trim();

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
    char name[100], email[100], phone[20], address[100], password[50];
    int preferenceChoice;

    try {
        cout << "Enter customer name (minimum 2 characters): ";
        cin.getline(name, 100);
        AYstr nameStr(name);
        if (nameStr.strlength() < 2) {
            throw InvalidInputException("Name must be at least 2 characters");
        }
        if (findCustomerByName(nameStr)) {
            throw InvalidInputException("Customer already exists with this name");
        }

        cout << "Enter email address: ";
        cin.getline(email, 100);
        AYstr emailStr(email);
        int atPos = emailStr.find_first('@');
        int dotPos = emailStr.find_first('.', atPos + 1);
        if (atPos == -1 || dotPos == -1 || atPos > dotPos) {
            throw InvalidInputException("Invalid email format (must contain @ and .)");
        }

        cout << "Enter phone number (11 digits, starting with 03): ";
        cin.getline(phone, 20);
        AYstr phoneStr(phone);
        if (phoneStr.strlength() != 11 || !phoneStr.substring(0, 2).isequal("03")) {
            throw InvalidInputException("Phone must be 11 digits starting with 03");
        }

        cout << "Enter address (minimum 10 characters): ";
        cin.getline(address, 100);
        AYstr addressStr(address);
        if (addressStr.strlength() < 10) {
            throw InvalidInputException("Address must be at least 10 characters");
        }

      
        cout << "Create password (8-20 chars, 1 uppercase, 1 lowercase, 1 digit): ";
        cin.getline(password, 50);
        AYstr passwordStr(password);
        if (passwordStr.strlength() < 8 || passwordStr.strlength() > 20) {
            throw InvalidInputException("Password must be 8-20 characters");
        }

        bool hasUpper = false, hasLower = false, hasDigit = false;
        for (int i = 0; i < passwordStr.strlength(); i++) {
            if (passwordStr[i] >= 'A' && passwordStr[i] <= 'Z') hasUpper = true;
            else if (passwordStr[i] >= 'a' && passwordStr[i] <= 'z') hasLower = true;
            else if (passwordStr[i] >= '0' && passwordStr[i] <= '9') hasDigit = true;
        }
        if (!hasUpper || !hasLower || !hasDigit) {
            throw InvalidInputException("Password requires 1 uppercase, 1 lowercase, and 1 digit");
        }

        cout << "\nSelect dietary preference:\n"
            << "1. Vegetarian\n2. Vegan\n3. Gluten-Free\n4. None\nChoice: ";
        cin >> preferenceChoice;
        cin.ignore();

        AYstr preference;
        switch (preferenceChoice) {
        case 1: preference = "Vegetarian"; break;
        case 2: preference = "Vegan"; break;
        case 3: preference = "Gluten-Free"; break;
        default: preference = "None"; break;
        }

        customers.push_back(Customer(nameStr, emailStr, phoneStr, addressStr, preference, passwordStr));
        saveCustomerToFile(customers.back());

        cout << "\nRegistration Successful!\n";
    }
    catch (const InvalidInputException& e) {
        cerr << "\nERROR: " << e.what() << "\nPlease try again.\n";
        throw; 
    }
    catch (const exception& e) {
        cerr << "\nUNEXPECTED ERROR: " << e.what() << "\n";
        throw;
    }
}
Customer* CustomerManager::findCustomerByName(const AYstr& name) {
    AYstr searchName = name;
    searchName.ToLower();  
    searchName.trim();     
    for (int i = 0; i < customers.size(); ++i) {
        AYstr customerName = customers[i].getName();
        customerName.ToLower();  
        customerName.trim();     

        if (customerName.isequal(searchName)) {
            return &customers[i];
        }
    }
    for (int i = 0; i < customers.size(); ++i) {
        AYstr customerName = customers[i].getName();
        customerName.ToLower(); 
        customerName.trim();     

        if (customerName.find_first(searchName) != -1) {
            return &customers[i];
        }
    }
    int spacePos = searchName.find_first(' ');
    if (spacePos != -1) {
        AYstr firstName = searchName.substring(0, spacePos);
        AYstr lastName = searchName.substring(spacePos + 1, searchName.strlength() - spacePos - 1);

        for (int i = 0; i < customers.size(); ++i) {
            AYstr customerName = customers[i].getName();
            customerName.ToLower();  
            customerName.trim();     

            if (customerName.find_first(firstName) != -1 ||
                customerName.find_first(lastName) != -1) {
                return &customers[i];
            }
        }
    }

    return nullptr;
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
        std::cout << "\nError: Customer not found! Please check the name or register first.\n";
        return;
    }

    if (customer->getOrderHistory().empty()) {
        std::cout << "\nNo order history found for ";
        customer->getName().print();
        std::cout << "\n";
        return;
    }

    std::cout << "\nOrder History for ";
    customer->getName().print();
    std::cout << ":\n";

    for (int i = 0; i < customer->getOrderHistory().size(); ++i) {
        std::cout << i + 1 << ". ";
        customer->getOrderHistory()[i].print();
        std::cout << "\n";
    }
}
Customer* CustomerManager::authenticateCustomer(const AYstr& email, const AYstr& password) {
    for (int i = 0; i < customers.size(); ++i) {
        if (customers[i].getEmail().isequal(email) &&
            customers[i].getPassword().isequal(password)) {
            return &customers[i];
        }
    }
    return nullptr;
}
void CustomerManager::loadCustomersFromFile() {
    std::ifstream customerFile("customers.txt");
    if (!customerFile.is_open()) {
        std::cout << "Warning: Could not open customers.txt - starting with empty customer list\n";
        return;
    }

    char buffer[256];
    while (customerFile.getline(buffer, 256)) {
        AYstr line(buffer);
        int commaPos[7];
        int pos = 0;
        for (int i = 0; i < 7; i++) {
            pos = line.find_first(',', pos);
            if (pos == -1) break;
            commaPos[i] = pos;
            pos++;
        }

        if (commaPos[0] == -1 || commaPos[1] == -1 || commaPos[2] == -1 ||
            commaPos[3] == -1 || commaPos[4] == -1 || commaPos[5] == -1 || commaPos[6] == -1) {
            continue;
        } 
        AYstr idStr = line.substring(0, commaPos[0]);
        int id = idStr.strtoint(idStr.c_str());

        AYstr name = line.substring(commaPos[0] + 1, commaPos[1] - commaPos[0] - 1).trim();
        AYstr email = line.substring(commaPos[1] + 1, commaPos[2] - commaPos[1] - 1).trim();
        AYstr phone = line.substring(commaPos[2] + 1, commaPos[3] - commaPos[2] - 1).trim();
        AYstr address = line.substring(commaPos[3] + 1, commaPos[4] - commaPos[3] - 1).trim();
        AYstr preference = line.substring(commaPos[4] + 1, commaPos[5] - commaPos[4] - 1).trim();
        AYstr password = line.substring(commaPos[5] + 1, commaPos[6] - commaPos[5] - 1).trim();
        AYstr pointsStr = line.substring(commaPos[6] + 1, line.strlength(line.c_str()) - commaPos[6] - 1).trim();
        double points = pointsStr.strtodouble(pointsStr.c_str());

        customers.push_back(Customer(name, email, phone, address, preference, password, points, id));
    }
    customerFile.close();
}

void CustomerManager::saveAllCustomerData() {
    std::ofstream customerFile("customers.txt");
    if (customerFile.is_open()) {
        for (int i = 0; i < customers.size(); ++i) {
            Customer& c = customers[i];
            customerFile << c.getId() << ","
                << c.getName().c_str() << ","
                << c.getEmail().c_str() << ","
                << c.getPhone().c_str() << ","
                << c.getAddress().c_str() << ","
                << c.getPreference().c_str() << ","
                << c.getPassword().c_str() << ","
                << c.getLoyaltyPoints() << "\n";
        }
        customerFile.close();
    }
    Customer::saveIdCounter();
}

void CustomerManager::saveCustomerToFile(const Customer& customer) {
    std::ofstream customerFile("customers.txt", std::ios::app);
    if (customerFile.is_open()) {
        customerFile << customer.getId() << ","
            << customer.getName().c_str() << ","
            << customer.getEmail().c_str() << ","
            << customer.getPhone().c_str() << ","
            << customer.getAddress().c_str() << ","
            << customer.getPreference().c_str() << ","
            << customer.getPassword().c_str() << ","
            << customer.getLoyaltyPoints() << "\n";
        customerFile.close();
    }
}