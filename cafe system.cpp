#include "Cafeteria.h"
#include <iostream>

using namespace std;

void displayMainMenu() {
    cout << "\n ________________________________________ ";
    cout << "\n|      WELCOME TO BEAN N BYTE CAFE       |";
    cout << "\n|----------------------------------------|";
    cout << "\n|  1. ADMINISTRATOR                      |";
    cout << "\n|  2. CUSTOMER                           |";
    cout << "\n|  3. EXIT                               |";
    cout << "\n|________________________________________|";
    cout << "\n\nEnter your choice (1-3): ";
}

void displayAdminMenu() {
    cout << "\n ________________________________________ ";
    cout << "\n|          ADMINISTRATOR MENU            |";
    cout << "\n|----------------------------------------|";
    cout << "\n|  1. MENU MANAGEMENT                    |";
    cout << "\n|  2. CUSTOMER MANAGEMENT                |";
    cout << "\n|  3. GENERATE REPORTS                   |";
    cout << "\n|  4. BACK TO MAIN MENU                  |";
    cout << "\n|________________________________________|";
    cout << "\n\nEnter your choice (1-4): ";
}

void displayCustomerMenu() {
    cout << "\n ________________________________________ ";
    cout << "\n|           CUSTOMER MENU                |";
    cout << "\n|----------------------------------------|";
    cout << "\n|  1. MANAGE DETAILS                     |";
    cout << "\n|  2. PLACE ORDER                        |";
    cout << "\n|  3. VIEW ORDER HISTORY                 |";
    cout << "\n|  4. BACK TO MAIN MENU                  |";
    cout << "\n|________________________________________|";
    cout << "\n\nEnter your choice (1-4): ";
}

int main() {
    Cafeteria cafeteria;
    int mainChoice, subChoice;
    bool running = true;

    while (running) {
        displayMainMenu();
        cin >> mainChoice;

        switch (mainChoice) {
        case 1:
            do {
                displayAdminMenu();
                cin >> subChoice;
                switch (subChoice) {
                case 1: cafeteria.menuManagement(); break;
                case 2: cafeteria.customerManagement(); break;
                case 3: cafeteria.displayReportingOptions(); break;
                case 4: break;
                default: cout << "Invalid choice.\n";
                }
            } while (subChoice != 4);
            break;

        case 2:
            do {
                displayCustomerMenu();
                cin >> subChoice;
                switch (subChoice) {
                case 1:
                    cafeteria.customerManagement();
                    break;
                case 2:
                    cafeteria.placeOrder();
                    break;
                case 3:
                    cafeteria.displayOrderHistory();
                    break;
                case 4: break;
                default: cout << "Invalid choice.\n";
                }
            } while (subChoice != 4);
            break;

        case 3:
            running = false;
            cout << "thank you for coming. Goodbye!\n";
            break;

        default:
            cout << "Invalid choice. Please try again.\n";
        }
    }

    cafeteria.saveData();
    return 0;
}