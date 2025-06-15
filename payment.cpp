#include "Payment.h"
#include <iostream>
#include "CustomExceptions.h"

Payment::Payment(const AYstr& method, const AYstr& accNum, double amt)
    : paymentMethod(method), accountNumber(accNum), amount(amt) {
}

bool Payment::processPayment() const {
    try {
        
        if (amount <= 0) {
            throw PaymentException("Payment amount must be positive");
        }

     
        if (paymentMethod.isequal("Cash")) {
            std::cout << "Processing cash payment of $" << amount << "...\n";
            return true;
        }

        
        if (accountNumber.strlength() == 0) {
            throw PaymentException("Account number is required");
        }

       
        if (paymentMethod.isequal("JazzCash") ||
            paymentMethod.isequal("EasyPaisa") ||
            paymentMethod.isequal("SadaPay") ||
            paymentMethod.isequal("NayaPay")) {

            if (accountNumber.strlength() != 11) {
                throw PaymentException("Mobile account must be 11 digits");
            }

            for (int i = 0; i < accountNumber.strlength(); i++) {
                if (accountNumber[i] < '0' || accountNumber[i] > '9') {
                    throw PaymentException("Account number must contain only digits");
                }
            }
        }
        
        else if (paymentMethod.isequal("Bank Transfer")) {
            if (accountNumber.strlength() != 14) {
                throw PaymentException("Bank account must be 14 digits");
            }
        }
        std::cout << "Processing " << paymentMethod.c_str()
            << " payment from account " << accountNumber.c_str()
            << " for $" << amount << "...\n";

        return true;
    }
    catch (const PaymentException& e) {
        std::cerr << "PAYMENT FAILED: " << e.what() << std::endl;
        return false;
    }
    catch (const std::exception& e) {
        std::cerr << "PAYMENT ERROR: " << e.what() << std::endl;
        return false;
    }
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