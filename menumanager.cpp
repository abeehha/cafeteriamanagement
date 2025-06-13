#include "MenuManager.h"
#include <fstream>
#include <iostream>
using namespace std;

MenuManager::MenuManager() {
    loadMenuFromFile();
}

MenuManager::~MenuManager() {
    for (int i = 0; i < menu.size(); ++i) {
        delete menu[i];
    }
    menu.clear();
}

void MenuManager::saveMenuToFile() {
    std::ofstream menuFile("menu.txt");
    if (!menuFile.is_open()) {
        std::cout << "Error: Unable to open menu file for writing\n";
        return;
    }

    for (int i = 0; i < menu.size(); ++i) {
        menuFile << menu[i]->getCategory().c_str() << ","
            << menu[i]->getName().c_str() << ","
            << menu[i]->getPrice() << ","
            << menu[i]->getStock() << "\n";
    }
    menuFile.close();
}

void MenuManager::loadMenuFromFile() {
    std::ifstream menuFile("menu.txt");
    if (!menuFile.is_open()) {
        std::cout << "Menu file not found. Initializing default menu.\n";
        initializeDefaultMenu();
        return;
    }

    char buffer[256];
    while (menuFile.getline(buffer, 256)) {
        AYstr line(buffer);
        int commaPos[3];
        int pos = 0;
        for (int i = 0; i < 3; i++) {
            pos = line.find_first(',', pos);
            if (pos == -1) break;
            commaPos[i] = pos;
            pos++;
        }

        if (commaPos[0] == -1 || commaPos[1] == -1 || commaPos[2] == -1) continue;

        AYstr category = line.substring(0, commaPos[0]);
        AYstr name = line.substring(commaPos[0] + 1, commaPos[1] - commaPos[0] - 1);
        AYstr priceStr = line.substring(commaPos[1] + 1, commaPos[2] - commaPos[1] - 1);

        AYstr stockStr = line.substring(commaPos[2] + 1, line.strlength(line.c_str()) - commaPos[2] - 1);

        double price = priceStr.strtodouble(priceStr.c_str());
        int stock = stockStr.strtoint(stockStr.c_str());

        MenuItem* newItem = nullptr;
        if (category.isequal(AYstr("Vegetarian"))) {
            newItem = new VegetarianMenuItem(name, price, stock);
        }
        else if (category.isequal(AYstr("Vegan"))) {
            newItem = new VeganMenuItem(name, price, stock);
        }
        else if (category.isequal(AYstr("Gluten-Free"))) {
            newItem = new GlutenFreeMenuItem(name, price, stock);
        }
        else if (category.isequal(AYstr("Specialty"))) {
            newItem = new SpecialtyMenuItem(name, price, stock);
        }
        else if (category.isequal(AYstr("Drinks"))) {
            newItem = new DrinkMenuItem(name, price, stock);
        }

        if (newItem) {
            menu.push_back(newItem);
        }
    }
    menuFile.close();
}
void MenuManager::initializeDefaultMenu() {
    menu.push_back(new VegetarianMenuItem("Veggie Burger", 8.99, 25));
    menu.push_back(new VegetarianMenuItem("Veggie Wrap", 7.49, 20));
    menu.push_back(new VegetarianMenuItem("Grilled Cheese Sandwich", 5.99, 30));
    menu.push_back(new VegetarianMenuItem("Vegetable Stir Fry", 9.49, 18));

    menu.push_back(new VeganMenuItem("Vegan Buddha Bowl", 10.99, 15));
    menu.push_back(new VeganMenuItem("Vegan Tacos", 8.49, 20));
    menu.push_back(new VeganMenuItem("Vegan Quinoa Salad", 9.29, 12));
    menu.push_back(new VeganMenuItem("Vegan Chili", 7.99, 25));

    menu.push_back(new GlutenFreeMenuItem("Gluten-Free Margherita Pizza", 11.49, 10));
    menu.push_back(new GlutenFreeMenuItem("Grilled Chicken Salad (Gluten-Free)", 10.99, 18));
    menu.push_back(new GlutenFreeMenuItem("Gluten-Free Pancakes", 6.99, 15));
    menu.push_back(new GlutenFreeMenuItem("Gluten-Free Pasta Primavera", 12.49, 14));

    menu.push_back(new SpecialtyMenuItem("Classic Cheeseburger", 9.99, 20));
    menu.push_back(new SpecialtyMenuItem("BBQ Pulled Pork Sandwich", 9.49, 15));
    menu.push_back(new SpecialtyMenuItem("Chicken Caesar Wrap", 8.99, 25));
    menu.push_back(new SpecialtyMenuItem("Grilled Salmon Plate", 13.49, 10));

    menu.push_back(new DrinkMenuItem("Iced Latte", 4.29, 30));
    menu.push_back(new DrinkMenuItem("Fresh Fruit Smoothie", 5.99, 20));
    menu.push_back(new DrinkMenuItem("Cappuccino", 3.49, 25));
    menu.push_back(new DrinkMenuItem("Lemonade", 2.99, 35));
}
void MenuManager::addMenuItem() {
    char name[100];
    double price;
    int stock;
    int categoryChoice;

    std::cout << "\n--- Add New Menu Item ---\n";
    std::cout << "Enter item name: ";
    std::cin.ignore();
    std::cin.getline(name, 100);
    AYstr itemName(name);

    std::cout << "Enter item price: ";
    while (!(std::cin >> price) || price <= 0) {
        std::cin.clear();
        std::cin.ignore(10000, '\n');
        std::cout << "Invalid price. Please enter a positive number: ";
    }

    std::cout << "Enter item stock: ";
    while (!(std::cin >> stock) || stock < 0) {
        std::cin.clear();
        std::cin.ignore(10000, '\n');
        std::cout << "Invalid stock. Please enter a non-negative integer: ";
    }

    std::cout << "Select category:\n"
        << "1. Vegetarian\n"
        << "2. Vegan\n"
        << "3. Gluten-Free\n"
        << "4. Specialty\n"
        << "5. Drinks\n"
        << "Choice: ";
    std::cin >> categoryChoice;

    MenuItem* newItem = nullptr;
    switch (categoryChoice) {
    case 1: newItem = new VegetarianMenuItem(itemName, price, stock); break;
    case 2: newItem = new VeganMenuItem(itemName, price, stock); break;
    case 3: newItem = new GlutenFreeMenuItem(itemName, price, stock); break;
    case 4: newItem = new SpecialtyMenuItem(itemName, price, stock); break;
    case 5: newItem = new DrinkMenuItem(itemName, price, stock); break;
    default:
        std::cout << "Invalid category selection.\n";
        return;
    }

    if (newItem) {
        menu.push_back(newItem);
        std::cout << "\nItem added successfully!\n";
        saveMenuToFile();
    }
}

void MenuManager::removeMenuItem() {
    if (menu.empty()) {
        std::cout << "Menu is empty.\n";
        return;
    }

    char name[100];
    std::cout << "\n--- Remove Menu Item ---\n";
    std::cout << "Enter the name of the item to remove: ";
    std::cin.ignore();
    std::cin.getline(name, 100);
    AYstr itemName(name);

    for (int i = 0; i < menu.size(); ++i) {
        if (menu[i]->getName().isequal(itemName)) {
            delete menu[i];
            menu.erase_at(i);
            std::cout << "Item removed successfully!\n";
            saveMenuToFile();
            return;
        }
    }
    std::cout << "Item not found in the menu.\n";
}

void MenuManager::updateStock() {
    if (menu.empty()) {
        std::cout << "Menu is empty.\n";
        return;
    }

    char name[100];
    int newStock;

    std::cout << "\n--- Update Item Stock ---\n";
    std::cout << "Enter item name: ";
    std::cin.ignore();
    std::cin.getline(name, 100);
    AYstr itemName(name);

    std::cout << "Enter new stock quantity: ";
    while (!(std::cin >> newStock) || newStock < 0) {
        std::cin.clear();
        std::cin.ignore(10000, '\n');
        std::cout << "Invalid stock. Please enter a non-negative integer: ";
    }

    for (int i = 0; i < menu.size(); ++i) {
        if (menu[i]->getName().isequal(itemName)) {
            menu[i]->setStock(newStock);
            std::cout << "Stock updated successfully!\n";
            saveMenuToFile();
            return;
        }
    }
    std::cout << "Item not found in the menu.\n";
}

void MenuManager::updatePrice() {
    if (menu.empty()) {
        std::cout << "Menu is empty.\n";
        return;
    }

    char name[100];
    double newPrice;

    std::cout << "\n--- Update Item Price ---\n";
    std::cout << "Enter item name: ";
    std::cin.ignore();
    std::cin.getline(name, 100);
    AYstr itemName(name);

    std::cout << "Enter new price: ";
    while (!(std::cin >> newPrice) || newPrice <= 0) {
        std::cin.clear();
        std::cin.ignore(10000, '\n');
        std::cout << "Invalid price. Please enter a positive number: ";
    }

    for (int i = 0; i < menu.size(); ++i) {
        if (menu[i]->getName().isequal(itemName)) {
            menu[i]->setPrice(newPrice);
            std::cout << "Price updated successfully!\n";
            saveMenuToFile();
            return;
        }
    }
    std::cout << "Item not found in the menu.\n";
}

void MenuManager::displayMenuByCategory() {
    if (menu.empty()) {
        std::cout << "Menu is empty.\n";
        return;
    }

    int categoryChoice;
    AYstr category;

    std::cout << "\n--- Menu by Category ---\n"
        << "1. Vegetarian\n"
        << "2. Vegan\n"
        << "3. Gluten-Free\n"
        << "4. Specialty\n"
        << "5. Drinks\n"
        << "Enter category number: ";
    std::cin >> categoryChoice;

    switch (categoryChoice) {
    case 1: category = "Vegetarian"; break;
    case 2: category = "Vegan"; break;
    case 3: category = "Gluten-Free"; break;
    case 4: category = "Specialty"; break;
    case 5: category = "Drinks"; break;
    default:
        std::cout << "Invalid category selection.\n";
        return;
    }

    std::cout << "\n--- " << category.c_str() << " Menu ---\n";
    bool found = false;
    for (int i = 0; i < menu.size(); ++i) {
        if (menu[i]->getCategory().isequal(category)) {
            std::cout << (i + 1) << ". " << menu[i]->getName().c_str()
                << " - $" << menu[i]->getPrice()
                << " (" << menu[i]->getStock() << " in stock)\n";
            found = true;
        }
    }

    if (!found) {
        std::cout << "No items found in this category.\n";
    }
}

void MenuManager::displayCompleteMenu() {
    if (menu.empty()) {
        std::cout << "Menu is empty.\n";
        return;
    }

    std::cout << "\n--- Complete Menu ---\n";
    for (int i = 0; i < menu.size(); ++i) {
        std::cout << (i + 1) << ". [" << menu[i]->getCategory().c_str() << "] "
            << menu[i]->getName().c_str() << " - $" << menu[i]->getPrice()
            << " (" << menu[i]->getStock() << " in stock)\n";
    }
}

void MenuManager::displayMenuOptions() {
    int choice;
    do {
        std::cout << "\n--- Menu Management System ---\n"
            << "1. Add Menu Item\n"
            << "2. Remove Menu Item\n"
            << "3. Update Stock\n"
            << "4. Update Price\n"
            << "5. Display Menu by Category\n"
            << "6. Display Complete Menu\n"
            << "7. Back to Main Menu\n"
            << "Enter your choice: ";
        std::cin >> choice;

        switch (choice) {
        case 1: addMenuItem(); break;
        case 2: removeMenuItem(); break;
        case 3: updateStock(); break;
        case 4: updatePrice(); break;
        case 5: displayMenuByCategory(); break;
        case 6: displayCompleteMenu(); break;
        case 7: break;
        default: std::cout << "Invalid choice. Please try again.\n";
        }
    } while (choice != 7);
}

double MenuManager::handleCustomizations() {
    double extraCost = 0.0;
    int choice;

    std::cout << "\n--- Item Customizations ---\n"
        << "1. Extra Cheese (+$0.50)\n"
        << "2. Extra Sauce (+$0.30)\n"
        << "3. Spicy Level (+$0.20)\n"
        << "4. No Customizations\n"
        << "Enter your choice: ";
    std::cin >> choice;

    switch (choice) {
    case 1: extraCost += 0.50; break;
    case 2: extraCost += 0.30; break;
    case 3: extraCost += 0.20; break;
    case 4: break;
    default: std::cout << "Invalid choice. No customizations added.\n";
    }

    return extraCost;
}
MyVector<MenuItem*>& MenuManager::getMenu() {
    return menu;
}

double MenuManager::getItemPrice(const AYstr& itemName) const {
    for (int i = 0; i < menu.size(); ++i) {
        if (menu[i]->getName().isequal(itemName)) {
            return menu[i]->getPrice();
        }
    }
    return 0.0;
}