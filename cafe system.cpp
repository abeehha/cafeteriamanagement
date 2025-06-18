﻿#include "Cafeteria.h"
#include "CustomExceptions.h"
#include "Windows.h"
#include <iostream>
#include "Logger.h"
#include <limits>

using namespace std;

const AYstr ADMIN_PASSWORD = "admin123";

bool checkAdminPassword() {
    try {
        char input[100];
        cout << "Enter admin password: ";
        cin.getline(input, 100);

        if (AYstr(input).isequal(ADMIN_PASSWORD)) {
            return true;
        }
        throw AuthenticationException("Incorrect password");
    }
    catch (const exception& e) {
        cerr << "ERROR: " << e.what() << endl;
        return false;
    }
}

void displayMainMenu() {
    system("cls");
    cout << "\n ________________________________________\n"
        << "|      WELCOME TO BEAN N BYTE CAFE       |\n"
        << "|----------------------------------------|\n"
        << "|  1. ADMINISTRATOR                      |\n"
        << "|  2. CUSTOMER                           |\n"
        << "|  3. EXIT                               |\n"
        << "|________________________________________|\n"
        << "\nEnter your choice (1-3): ";
}

void displayAdminMenu() {
    system("cls");
    cout << "\n ________________________________________\n"
        << "|          ADMINISTRATOR MENU            |\n"
        << "|----------------------------------------|\n"
        << "|  1. MENU MANAGEMENT                    |\n"
        << "|  2. CUSTOMER MANAGEMENT                |\n"
        << "|  3. GENERATE REPORTS                   |\n"
        << "|  4. BACK TO MAIN MENU                  |\n"
        << "|________________________________________|\n"
        << "\nEnter your choice (1-4): ";
}

void displayCustomerMenu() {
    cout << "\n ________________________________________\n"
        << "|           CUSTOMER MENU                |\n"
        << "|----------------------------------------|\n"
        << "|  1. LOG IN/SIGN UP AND PLACE ORDER     |\n"
        << "|  2. VIEW WHAT YOU'VE ORDERED BEFORE    |\n"
        << "|  3. BACK TO MAIN MENU                  |\n"
        << "|________________________________________|\n"
        << "\nEnter your choice (1-3): ";
}
void SetColor(int bg, int fg) {
    SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), (bg << 4) | fg);
}
void ResetColor() {
    SetColor(0, 7);
}

int main() {
    SetColor(0,6);
    Logger::getInstance().initialize("log.txt");
    Cafeteria cafeteria;
    Logger::getInstance().writeMessage("cafeteria class made");
    int mainChoice = 0;
    bool running = true;

    try {
        while (running) {
            try {
                
                displayMainMenu();
                Logger::getInstance().writeMessage("main menu shown");
                if (!(cin >> mainChoice)) {
                    cin.clear();
                    cin.ignore();
                    throw InvalidInputException("Please enter a number (1-3)");
                }
                cin.ignore();

                switch (mainChoice) {
                case 1: { 
                    system("cls");
                    try {
                        if (!checkAdminPassword()) {
                            Logger::getInstance().writeMessage("admin password wrong");
                            throw AuthenticationException("Access denied");
                           
                        }

                        int adminChoice = 0;
                        do {
                            try {
                                displayAdminMenu();
                                Logger::getInstance().writeMessage("admin menu shown");

                                if (!(cin >> adminChoice)) {
                                    cin.clear();
                                    cin.ignore();
                                    throw InvalidInputException("Please enter a number (1-4)");
                                }
                                cin.ignore();

                                switch (adminChoice) {
                                case 1:
                                    cafeteria.menuManagement();
                                    Logger::getInstance().writeMessage("doing menu management");
                                    break;
                                    system("cls");
                                case 2:
                                    cafeteria.customerManagement();
                                    Logger::getInstance().writeMessage("doing customer management");
                                    break;
                                    system("cls");
                                case 3:
                                    cafeteria.displayReportingOptions();
                                    Logger::getInstance().writeMessage("showing reports");
                                    break;
                                    system("cls");
                                case 4:
                                    break;
                                default:
                                    throw InvalidInputException("Invalid choice (1-4 only)");
                                    Logger::getInstance().writeMessage("invalid choice made");
                                }
                            }
                            catch (const InvalidInputException& e) {
                                cerr << "INPUT ERROR: " << e.what() << endl;
                                cout << "Press Enter to continue...";
                                cin.ignore();
                            }
                            catch (const exception& e) {
                                cerr << "ADMIN ERROR: " << e.what() << endl;
                                cout << "Press Enter to continue...";
                                cin.ignore();
                            }
                        } while (adminChoice != 4);
                    }
                    catch (const AuthenticationException& e) {
                        cerr << "AUTH ERROR: " << e.what() << endl;
                        cout << "Press Enter to continue...";
                        cin.ignore();
                    }
                    break;
                    system("cls");
                }

                case 2: { 
                    system("cls");
                    int customerChoice = 0;
                   
                    do {
                        try {
                            displayCustomerMenu();
                            Logger::getInstance().writeMessage("customer menu shown");
                            if (!(cin >> customerChoice)) {
                                cin.clear();
                                cin.ignore();
                                throw InvalidInputException("Please enter a number (1-3)");
                            }
                            cin.ignore();

                            switch (customerChoice) {
                            case 1:
                                cafeteria.placeOrder();
                                Logger::getInstance().writeMessage("placing order");
                                break;
                                system("cls");
                            case 2:
                                cafeteria.displayOrderHistory();
                                Logger::getInstance().writeMessage("displaying history");
                                break;
                                system("cls");
                            case 3:
                                break;
                            default:
                                throw InvalidInputException("Invalid choice (1-3 only)");
                            }
                        }
                        catch (const OrderException& e) {
                            cerr << "ORDER ERROR: " << e.what() << endl;
                            cout << "Press Enter to continue...";
                            cin.ignore();
                        }
                        catch (const InvalidInputException& e) {
                            cerr << "INPUT ERROR: " << e.what() << endl;
                            cout << "Press Enter to continue...";
                            cin.ignore();
                        }
                        catch (const exception& e) {
                            cerr << "CUSTOMER ERROR: " << e.what() << endl;
                            cout << "Press Enter to continue...";
                            cin.ignore();
                        }
                    } while (customerChoice != 3);
                    break;
                    system("cls");
                }

                case 3: 
                    
                    running = false;
                    cout << "Thank you for coming. Goodbye!\n";
                    Logger::getInstance().writeMessage("exit choice");
                    break;
                    system("cls");

                default:
                    throw InvalidInputException("Invalid choice (1-3 only)");
                }
            }
            catch (const InvalidInputException& e) {
                cerr << "MENU ERROR: " << e.what() << endl;
                cout << "Press Enter to continue...";
                cin.ignore();
            }
            catch (const exception& e) {
                cerr << "SYSTEM ERROR: " << e.what() << endl;
                cout << "Press Enter to continue...";
                cin.ignore();
            }
        }

       
        try {
            cafeteria.saveData();
        }
        catch (const FileException& e) {
            cerr << "WARNING: Data may not have saved properly - " << e.what() << endl;
        }
    }
    catch (const exception& e) {
        cerr << "FATAL ERROR: " << e.what() << endl;
        return 1;
    }
    catch (...) {
        cerr << "UNKNOWN FATAL ERROR" << endl;
        return 1;
    }

    ResetColor();
    Logger::getInstance().writeMessage("code ending");
    return 0;
}