#include "Cafeteria.h"
#include <iostream>

using namespace std;
const AYstr ADMIN_PASSWORD = "admin123";

bool checkAdminPassword() {
    char input[100];
    cout << "Enter admin password: ";
    cin.getline(input, 100);
    return AYstr(input).isequal(ADMIN_PASSWORD);
}

void displayMainMenu() {
    system("cls");
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
    system("cls");
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
    system("cls");
    cout << "\n ________________________________________ ";
    cout << "\n|           CUSTOMER MENU                |";
    cout << "\n|----------------------------------------|";
    cout << "\n|  1. LOG IN/SIGN UP AND PLACE ORDER     |";
    cout << "\n|  2. VIEW WHAT YOU'VE ORDERED BEFORE    |";
    cout << "\n|  3. BACK TO MAIN MENU                  |";
    cout << "\n|________________________________________|";
    cout << "\n\nEnter your choice (1-3): ";
}

int main() {
    Cafeteria cafeteria;
    int mainChoice, subChoice;
    bool running = true;

    while (running) {
        displayMainMenu();
        cin >> mainChoice;
        cin.ignore();

        switch (mainChoice) {
        case 1: {
           
            if (!checkAdminPassword()) {
                cout << "Incorrect password. Access denied.\n";
                break;
            }

            do {
                displayAdminMenu();
                cin >> subChoice;
                cin.ignore(); 

                switch (subChoice) {
                case 1: cafeteria.menuManagement(); break;
                case 2: cafeteria.customerManagement(); break;
                case 3: cafeteria.displayReportingOptions(); break;
                case 4: break;
                default: cout << "Invalid choice.\n";
                }
            } while (subChoice != 4);
            break;
        }

        case 2:
            do {
                displayCustomerMenu();
                cin >> subChoice;
                cin.ignore();

                switch (subChoice) {
                case 1: cafeteria.placeOrder(); break;
                case 2: cafeteria.displayOrderHistory(); break;
                case 3: break;
                default: cout << "Invalid choice.\n";
                }
            } while (subChoice != 3);
            break;

        case 3:
            running = false;
            cout << "Thank you for coming. Goodbye!\n";
            break;

        default:
            cout << "Invalid choice. Please try again.\n";
        }
    }

    cafeteria.saveData();
    return 0;
}