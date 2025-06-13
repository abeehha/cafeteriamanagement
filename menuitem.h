#ifndef MENUITEM_H
#define MENUITEM_H

#include "aystr.h"

class MenuItem {
protected:
    AYstr name;
    double price;
    int stock;

public:
    MenuItem(AYstr name, double price, int stock);
    virtual ~MenuItem() = default;

    virtual AYstr getCategory() const = 0;
    virtual void displayItem() const;

    AYstr getName() const;
    double getPrice() const;
    int getStock() const;

    bool updateStock(int quantity);
    bool isAvailable(int quantity) const;
    void setStock(int newStock);
    void setPrice(double newPrice);
};

class VegetarianMenuItem : public MenuItem {
public:
    VegetarianMenuItem(AYstr name, double price, int stock);
    AYstr getCategory() const override;
    void displayItem() const override;
};

class VeganMenuItem : public MenuItem {
public:
    VeganMenuItem(AYstr name, double price, int stock);
    AYstr getCategory() const override;
    void displayItem() const override;
};

class GlutenFreeMenuItem : public MenuItem {
public:
    GlutenFreeMenuItem(AYstr name, double price, int stock);
    AYstr getCategory() const override;
    void displayItem() const override;
};

class SpecialtyMenuItem : public MenuItem {
public:
    SpecialtyMenuItem(AYstr name, double price, int stock);
    AYstr getCategory() const override;
    void displayItem() const override;
};

class DrinkMenuItem : public MenuItem {
public:
    DrinkMenuItem(AYstr name, double price, int stock);
    AYstr getCategory() const override;
    void displayItem() const override;
};

#endif
