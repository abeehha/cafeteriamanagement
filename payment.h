#ifndef PAYMENT_H
#define PAYMENT_H

#include "AYstr.h"
#include <iostream>

class Payment {
private:
    AYstr paymentMethod;
    AYstr accountNumber;
    double amount;

public:
    Payment(const AYstr& method, const AYstr& accNum, double amt);

    bool processPayment() const;
    AYstr getPaymentMethod() const;
    AYstr getAccountNumber() const;
    double getAmount() const;

    static void displayPaymentOptions();
};

#endif 