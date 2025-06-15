#ifndef CUSTOM_EXCEPTIONS_H
#define CUSTOM_EXCEPTIONS_H

#include <stdexcept>
#include "AYstr.h"

class InvalidInputException : public std::runtime_error {
public:
    InvalidInputException(const AYstr& message)
        : std::runtime_error(message.c_str()) {}
};

class FileException : public std::runtime_error {
public:
    FileException(const AYstr& message)
        : std::runtime_error(message.c_str()) {}
};

class AuthenticationException : public std::runtime_error {
public:
    AuthenticationException(const AYstr& message)
        : std::runtime_error(message.c_str()) {}
};

class PaymentException : public std::runtime_error {
public:
    PaymentException(const AYstr& message)
        : std::runtime_error(message.c_str()) {}
};

class StockException : public std::runtime_error {
public:
    StockException() : std::runtime_error("Stock error") {}
    StockException(const AYstr& message)
        : std::runtime_error(message.c_str()) {}
};
class OrderException : public std::runtime_error {
public:
    OrderException(const std::string& msg) : std::runtime_error(msg) {}
};

#endif