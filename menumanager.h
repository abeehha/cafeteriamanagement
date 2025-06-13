#ifndef MENUMANAGER_H
#define MENUMANAGER_H

#include "MenuItem.h"
#include "myvector.h"

class MenuManager {
    MyVector<MenuItem*> menu;
public:
    void saveMenuToFile();
    void loadMenuFromFile();
    void initializeDefaultMenu();
    MenuManager();
    virtual ~MenuManager();
    void addMenuItem();
    void removeMenuItem();
    void updateStock();
    void updatePrice();
    void displayMenuByCategory();
    void displayCompleteMenu();
    void displayMenuOptions();
    double handleCustomizations();
    MyVector<MenuItem*>& getMenu();
    double getItemPrice(const AYstr& itemName) const;
};

#endif
