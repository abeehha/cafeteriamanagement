#include "Logger.h"
#include <iostream>

Logger& Logger::getInstance() {
    static Logger instance;
    return instance;
}

void Logger::initialize(const std::string& filename) {
    write.open(filename, std::ios::app); 
    if (!write.is_open()) {
        std::cerr << "Error: Failed to initialize log file." << std::endl;
        isInitialized = false;
        return;
    }
    isInitialized = true;

  
}
void Logger::cleanup() {
    if (write.is_open()) {
        write.close();
    }
    isInitialized = false;
}

void Logger::writeMessage(const std::string& error_message) {
    if (!isInitialized) {
        std::cerr << "Error: Logger not initialized." << std::endl;
        return;
    }

    std::time_t currentTime = std::time(nullptr);
    std::tm localTime;

   
#if defined(_WIN32) || defined(_WIN64)
    if (localtime_s(&localTime, &currentTime) != 0) {
        std::cerr << "Error: Failed to get local time." << std::endl;
        return;
    }
   
#else
    if (localtime_r(&currentTime, &localTime) == nullptr) {
        std::cerr << "Error: Failed to get local time." << std::endl;
        return;
    }
#endif

    char timeStr[20];
    std::strftime(timeStr, sizeof(timeStr), "%Y-%m-%d %H:%M:%S", &localTime);

    write << "[" << timeStr << "] ERROR: " << error_message << std::endl;
}

Logger::~Logger() {
    cleanup();
}