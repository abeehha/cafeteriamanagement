#ifndef SALESRECORD_H
#define SALESRECORD_H
#include "aystr.h"
#include "date.h"


class SalesRecord {
    AYstr itemName;
    AYstr customerName;
    int quantity;
    float unitPrice;
    Date date;

public:
    SalesRecord();
    SalesRecord(const AYstr& itemName, const AYstr& customerName, int quantity, float price, int day, int month, int year);

    void print() const;

    int getQuantity() const;
    float getUnitPrice() const;
    AYstr getItemName() const;
    AYstr getCustomerName() const;
    float getTotal() const;
    const Date& getDate() const;
    bool isSameDate(int d, int m, int y) const;
    bool isSameMonth(int m, int y) const;
};


#endif
