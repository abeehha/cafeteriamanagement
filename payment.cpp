#include "Payment.h"
#include <iostream>

Payment::Payment(const AYstr& method, const AYstr& accNum, double amt)
    : paymentMethod(method), accountNumber(accNum), amount(amt) {
}

bool Payment::processPayment() const {
    std::cout << "Processing payment via ";
    paymentMethod.print();
    std::cout << "...\n";

    if (paymentMethod.isequal("Cash")) {
        std::cout << "Payment of $";
        std::cout << amount;
        std::cout << " accepted in cash.\n";
        return true;
    }
    if (paymentMethod.isequal("JazzCash") || paymentMethod.isequal("EasyPaisa") ||
        paymentMethod.isequal("SadaPay") || paymentMethod.isequal("NayaPay")) {
        if (accountNumber.strlength() != 11) { 
            std::cout << "Error: Mobile account number must be 11 digits.\n";
            return false;
        }
    }
    else if (paymentMethod.isequal("Bank Transfer")) {
        if (accountNumber.strlength() != 14) { 
            std::cout << "Error: Bank account number must be 14 digits.\n";
            return false;
        }
    }

    std::cout << "Payment of $";
    std::cout << amount;
    std::cout << " from account: ";
    accountNumber.print();
    std::cout << " was successful!\n";

    return true;
}

AYstr Payment::getPaymentMethod() const { return paymentMethod; }
AYstr Payment::getAccountNumber() const { return accountNumber; }
double Payment::getAmount() const { return amount; }

void Payment::displayPaymentOptions() {
    std::cout << "\n--- Payment Options ---\n";
    std::cout << "1. JazzCash (11-digit account)\n";
    std::cout << "2. EasyPaisa (11-digit account)\n";
    std::cout << "3. SadaPay (11-digit account)\n";
    std::cout << "4. NayaPay (11-digit account)\n";
    std::cout << "5. Bank Transfer (14-digit account)\n";
    std::cout << "6. Cash\n";
}