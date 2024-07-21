#include "Bill.h"
#include <iomanip>

Bill::Bill() : username(""), totalAmount(0.0) {}

Bill::Bill(const std::string& username) : username(username), totalAmount(0.0) {}

void Bill::addItem(const std::string& item, double price, int quantity) {
    items.push_back(item);
    prices.push_back(price);
    quantities.push_back(quantity);
}

void Bill::calculateTotal() {
    totalAmount = 0.0;
    for (size_t i = 0; i < prices.size(); ++i) {
        totalAmount += prices[i] * quantities[i];
    }
    totalAmount *= 1.10; // Add 10% tax
}

void Bill::saveBill(std::ofstream& outFile) const {
    outFile << username << ",";
    for (size_t i = 0; i < items.size(); ++i) {
        outFile << items[i] << "," << prices[i] << "," << quantities[i] << ",";
    }
    outFile << totalAmount << "\n";
}

double Bill::getTotalAmount() const {
    return totalAmount;
}

std::string Bill::getUsername() const {
    return username;
}
