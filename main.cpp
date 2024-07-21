#include "UserManager.h"
#include <iostream>
#include <string>
#include <thread>
#include <limits>

// Function to get a valid price input from the user
bool getValidPrice(double& price) {
    std::cout << "Enter price: ";
    while (!(std::cin >> price) || price <= 0) {
        std::cout << "Please enter a positive number for price: ";
        std::cin.clear();
        std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
    }
    return true;
}

// Function to get a valid quantity input from the user
bool getValidQuantity(int& quantity) {
    std::cout << "Enter quantity: ";
    while (!(std::cin >> quantity) || quantity <= 0) {
        std::cout << "Please enter a whole number for quantity: ";
        std::cin.clear();
        std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
    }
    return true;
}

// Function to get item inputs from the user and save them in the bill
void getInput(UserManager& manager, const std::string& username) {
    Bill& bill = manager.getUserBill(username);
    std::string item;
    double price;
    int quantity;

    while (true) {
        std::cout << "Enter item name (or 'done' to finish): ";
        std::cin >> item;
        if (item == "done") break;

        if (getValidPrice(price) && getValidQuantity(quantity)) {
            bill.addItem(item, price, quantity);
        }
    }

    bill.calculateTotal();
    manager.saveBillToFile(bill);
}

// Main function to manage the user interface and overall functionality
int main() {
    UserManager manager;
    manager.clearBillsFile();  // Clear previous bills at the start

    int choice;
    while (true) {
        std::cout << "\nChoose a number: 1. Add Customer 2. Rollback 3. Exit\n";
        std::cout << "Enter your choice: ";
        while (!(std::cin >> choice) || choice < 1 || choice > 3) {
            std::cout << "Invalid choice. Please enter 1, 2, or 3: ";
            std::cin.clear();
            std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
        }

        if (choice == 3) break;
        if (choice == 2) {
            manager.rollbackLastUser();
            continue;
        }
        if (choice == 1) {
            std::string username;
            std::cout << "Enter username: ";
            std::cin >> username;
            manager.addUser(username);
            getInput(manager, username);
        }
    }

    manager.updateDailyTotal();  // Save the total amount of all the bills for the day
    manager.clearBillsFile();    // Clear the bills file for the next day

    return 0;
}
