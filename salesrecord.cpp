#include "salesrecord.h"
#include <iostream>

SalesRecord::SalesRecord() : quantity(0), unitPrice(0.0f), date() {}

SalesRecord::SalesRecord(const AYstr& item, const AYstr& customer, int qty, float price, int d, int m, int y)
    : itemName(item), customerName(customer), quantity(qty), unitPrice(price), date(d, m, y) {
}

void SalesRecord::print() const {
    std::cout << "Item: ";
    itemName.print();
    std::cout << ", Customer: ";
    customerName.print();
    std::cout << ", Quantity: " << quantity;
    std::cout << ", Unit Price: " << unitPrice;
    std::cout << ", Date: ";
    date.getFormattedDate().print();
    std::cout << ", Total: " << getTotal() << "\n";
}

int SalesRecord::getQuantity() const { return quantity; }
float SalesRecord::getUnitPrice() const { return unitPrice; }
AYstr SalesRecord::getItemName() const { return itemName; }
AYstr SalesRecord::getCustomerName() const { return customerName; }
float SalesRecord::getTotal() const { return quantity * unitPrice; }
const Date& SalesRecord::getDate() const {
    return date;
}
bool SalesRecord::isSameDate(int d, int m, int y) const {
    return date.getDay() == d && date.getMonth() == m && date.getYear() == y;
}