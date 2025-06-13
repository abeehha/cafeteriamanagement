#include "menuitem.h"
#include <iostream>
#include <iomanip> //for setprecision 
using namespace std;

MenuItem::MenuItem(AYstr name, double price, int stock)
    : name(name), price(price), stock(stock) {
}

void MenuItem::displayItem() const {
    name.print();
    cout << " - $" << fixed << setprecision(2) << price
        << " (Available: " << stock << ")" << endl;
}

AYstr MenuItem::getName() const { return name; }
double MenuItem::getPrice() const { return price; }
int MenuItem::getStock() const { return stock; }

bool MenuItem::updateStock(int quantity) {
    if (quantity > stock) return false;
    stock -= quantity;
    return true;
}

bool MenuItem::isAvailable(int quantity) const { return stock >= quantity; }

void MenuItem::setStock(int newStock) {
    if (newStock < 0) {
        cout << "Error: Stock cannot be negative" << endl;
        return;
    }
    stock = newStock;
    cout << "Stock updated successfully to " << stock << ".\n";
}

void MenuItem::setPrice(double newPrice) {
    if (newPrice < 0.0) {
        cout << "Error: Price cannot be negative" << endl;
        return;
    }
    price = newPrice;
    cout << "Price updated successfully to $" << fixed << setprecision(2) << price << ".\n";
}

VegetarianMenuItem::VegetarianMenuItem(AYstr name, double price, int stock)
    : MenuItem(name, price, stock) {
}

AYstr VegetarianMenuItem::getCategory() const { return "Vegetarian"; }

void VegetarianMenuItem::displayItem() const {
    cout << "[Vegetarian] ";
    MenuItem::displayItem();
}


VeganMenuItem::VeganMenuItem(AYstr name, double price, int stock)
    : MenuItem(name, price, stock) {
}

AYstr VeganMenuItem::getCategory() const { return "Vegan"; }

void VeganMenuItem::displayItem() const {
    cout << "[Vegan] ";
    MenuItem::displayItem();
}

GlutenFreeMenuItem::GlutenFreeMenuItem(AYstr name, double price, int stock)
    : MenuItem(name, price, stock) {
}

AYstr GlutenFreeMenuItem::getCategory() const { return "Gluten-Free"; }

void GlutenFreeMenuItem::displayItem() const {
    cout << "[Gluten-Free] ";
    MenuItem::displayItem();
}

SpecialtyMenuItem::SpecialtyMenuItem(AYstr name, double price, int stock)
    : MenuItem(name, price, stock) {
}

AYstr SpecialtyMenuItem::getCategory() const { return "Specialty"; }

void SpecialtyMenuItem::displayItem() const {
    cout << "[Specialty] ";
    MenuItem::displayItem();
}

DrinkMenuItem::DrinkMenuItem(AYstr name, double price, int stock)
    : MenuItem(name, price, stock) {
}

AYstr DrinkMenuItem::getCategory() const { return "Drinks"; }

void DrinkMenuItem::displayItem() const {
    cout << "[Drinks] ";
    MenuItem::displayItem();
}