#include "UserManager.h"
#include <iostream>
#include <fstream>

UserManager::UserManager() : dailyTotal(0.0), dayCount(0), stop(false) {
    // Initialize the day count based on existing records in daily_totals.csv
    std::ifstream inFile(dailyTotalFileName);
    std::string line;
    while (std::getline(inFile, line)) {
        if (line.find("Day") != std::string::npos) {
            dayCount++;
        }
    }
    inFile.close();

    // Start worker threads
    unsigned int numThreads = std::thread::hardware_concurrency();
    for (unsigned int i = 0; i < numThreads; ++i) {
        workers.emplace_back(&UserManager::workerThread, this);
    }
}

UserManager::~UserManager() {
    {
        std::unique_lock<std::mutex> lock(mtx);
        stop = true;
    }
    cv.notify_all();
    for (std::thread &worker : workers) {
        worker.join();
    }
}

void UserManager::workerThread() {
    while (true) {
        std::function<void()> task;
        {
            std::unique_lock<std::mutex> lock(mtx);
            cv.wait(lock, [this] { return stop || !tasks.empty(); });
            if (stop && tasks.empty()) return;
            task = std::move(tasks.front());
            tasks.pop();
        }
        task();
    }
}

void UserManager::enqueueTask(std::function<void()> task) {
    {
        std::unique_lock<std::mutex> lock(mtx);
        tasks.emplace(task);
    }
    cv.notify_one();
}

void UserManager::addUser(const std::string& username) {
    if (users.find(username) == users.end()) {
        users[username] = std::stack<Bill>();
    }
    users[username].push(Bill(username));
    userOrder.push(username);
}

Bill& UserManager::getUserBill(const std::string& username) {
    return users[username].top();
}

void UserManager::saveAllBills() {
    std::ofstream outFile(billsFileName, std::ios::app);
    for (const auto& userStack : users) {
        std::stack<Bill> bills = userStack.second;
        while (!bills.empty()) {
            bills.top().saveBill(outFile);
            bills.pop();
        }
    }
    outFile.close();
}

void UserManager::updateDailyTotal() {
    std::lock_guard<std::mutex> lock(mtx);
    std::ofstream outFile(dailyTotalFileName, std::ios::app);
    outFile << "Day " << dayCount << "," << dailyTotal << "\n";
    outFile.close();
}

void UserManager::clearBillsFile() {
    std::ofstream outFile(billsFileName, std::ios::trunc);
    outFile.close();
}

void UserManager::saveBillToFile(const Bill& bill) {
    enqueueTask([this, bill]() {
        std::lock_guard<std::mutex> lock(mtx);
        std::ofstream outFile(billsFileName, std::ios::app);
        if (outFile.is_open()) {
            bill.saveBill(outFile);
            outFile.close();
        } else {
            std::cerr << "Unable to open file " << billsFileName << "\n";
        }

        // Update daily total
        dailyTotal += bill.getTotalAmount();
        std::ofstream dailyTotalOut(dailyTotalFileName, std::ios::app);
        if (dailyTotalOut.is_open()) {
            dailyTotalOut << "Updated total for the day: " << dailyTotal << "\n";
            dailyTotalOut.close();
        } else {
            std::cerr << "Unable to open file " << dailyTotalFileName << "\n";
        }
    });
}

void UserManager::rollbackLastUser() {
    enqueueTask([this]() {
        std::lock_guard<std::mutex> lock(mtx);
        if (userOrder.empty()) {
            std::cerr << "No user to rollback.\n";
            return;
        }
        std::string lastUser = userOrder.top();
        userOrder.pop();

        if (users.find(lastUser) != users.end() && !users[lastUser].empty()) {
            double lastTotal = users[lastUser].top().getTotalAmount();
            dailyTotal -= lastTotal;
            users[lastUser].pop();

            if (users[lastUser].empty()) {
                users.erase(lastUser);
            }

            std::ofstream dailyTotalOut(dailyTotalFileName, std::ios::app);
            dailyTotalOut << "Rolled back user " << lastUser << ", Updated total for the day: " << dailyTotal << "\n";
            dailyTotalOut.close();

            clearBillsFile();
            std::ofstream outFile(billsFileName, std::ios::app);
            for (const auto& userStack : users) {
                std::stack<Bill> bills = userStack.second;
                while (!bills.empty()) {
                    bills.top().saveBill(outFile);
                    bills.pop();
                }
            }
            outFile.close();
        } else {
            std::cerr << "No bill found for user " << lastUser << ".\n";
        }
    });
}
