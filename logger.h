#pragma once
#include <string>
#include <fstream>
#include <ctime>

class Logger {
public:
    static Logger& getInstance();
    void initialize(const std::string& filename);
    void cleanup();
    void writeMessage(const std::string& error_message);
    Logger(const Logger&) = delete;
    Logger& operator=(const Logger&) = delete;

private:
    Logger() = default; 
    ~Logger();

    std::ofstream write;     
    bool isInitialized = false; 
};