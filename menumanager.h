#ifndef MENUMANAGER_H
#define MENUMANAGER_H

#include "MenuItem.h"
#include "MyVector.h"

class MenuManager {
private:
    MyVector<MenuItem*> menu;
public:
    void loadMenuFromFile();
    void saveMenuToFile();
    void initializeDefaultMenu();

    MenuManager();
    ~MenuManager();

    void addMenuItem();
    void removeMenuItem();
    void updateStock();
    void updatePrice();
    void displayMenuByCategory();
    void displayCompleteMenu();

    void displayMenuOptions();
    double handleCustomizations();
    double getItemPrice(const AYstr& itemName) const;

    MyVector<MenuItem*>& getMenu();
};

#endif