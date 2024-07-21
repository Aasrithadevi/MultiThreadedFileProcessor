#ifndef USERMANAGER_H
#define USERMANAGER_H

#include "Bill.h"
#include <string>
#include <vector>
#include <unordered_map>
#include <stack>
#include <mutex>
#include <thread>
#include <queue>
#include <condition_variable>
#include <functional>
#include <future>

// The UserManager class manages multiple users' bills and provides functionalities to save, update, and rollback bills.
class UserManager {
public:
    UserManager();
    ~UserManager();
    void addUser(const std::string& username);
    Bill& getUserBill(const std::string& username);
    void saveAllBills();
    void updateDailyTotal();
    void clearBillsFile();
    void saveBillToFile(const Bill& bill);
    void rollbackLastUser();
    void enqueueTask(std::function<void()> task);

private:
    std::unordered_map<std::string, std::stack<Bill>> users;
    std::stack<std::string> userOrder;
    std::string billsFileName = "today_bills.csv";
    std::string dailyTotalFileName = "daily_totals.csv";
    double dailyTotal;
    int dayCount;
    std::mutex mtx;
    std::vector<std::thread> workers;
    std::queue<std::function<void()>> tasks;
    std::condition_variable cv;
    bool stop;

    void workerThread();
};

#endif // USERMANAGER_H
