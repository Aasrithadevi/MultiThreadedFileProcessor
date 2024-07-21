#ifndef BILL_H
#define BILL_H

#include <string>
#include <vector>
#include <fstream>

// The Bill class manages items, prices, quantities, and calculates total cost including tax.
class Bill {
public:
    Bill();
    Bill(const std::string& username);
    void addItem(const std::string& item, double price, int quantity);  // Add an item to the bill
    void calculateTotal();  // Calculate total cost including tax
    void saveBill(std::ofstream& outFile) const;  // Save bill to a file
    double getTotalAmount() const;  // Get the total amount of the bill
    std::string getUsername() const;  // Get the username

private:
    std::string username;
    std::vector<std::string> items;
    std::vector<double> prices;
    std::vector<int> quantities;
    double totalAmount;
};

#endif // BILL_H
