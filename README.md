# MultiThreadedFileProcessor
File I/O operation , MultiThreading and Exception Handling
## Project Description

This project is an attempt to learn and implement various concepts such as object-oriented programming, multithreading, and file handling. The application allows users to manage customer bills by adding orders, rolling back the most recent order, and saving the data to CSV files. Multithreading is used to handle tasks concurrently, ensuring efficient and responsive processing.

## Features

- Add customer orders with item details, prices, and quantities.
- Calculate total bill amounts, including a 10% tax.
- Roll back the most recent customer order.
- Save all customer bills to `today_bills.csv`.
- Maintain a daily total of all bills in `daily_totals.csv`.
- Clear the bills file upon exiting the program.
- Multithreaded processing for efficient task handling.

## Future Work

- Create a graphical user interface (GUI) for better user experience.
- Integrate a SQLite database to store customer and bill information.
- Develop REST APIs to enable remote access and management of the billing system.

## Project Structure

```
MultiThreadedFileProcessor/
├── Bill.h
├── Bill.cpp
├── UserManager.h
├── UserManager.cpp
├── main.cpp
├── CMakeLists.txt
├── build/
└── README.md
```

## Prerequisites

- C++ compiler (supports C++11 standard or later)
- CMake (version 3.10 or later)
- pthread library (for Unix-based systems)

## How to Build and Run

1. **Create the Build Directory**
   ```sh
   mkdir build
   cd build
   ```

2. **Run CMake**
   ```sh
   cmake ..
   ```

3. **Build the Project**
   ```sh
   cmake --build .
   ```

4. **Run the Executable**
   ```sh
   ./FileProcessorApp
   ```

## Usage Instructions

1. **Choose a Number**
   - `1` to add a new customer.
   - `2` to roll back the most recent customer.
   - `3` to exit.

2. **Add Customer**
   - Enter the username.
   - Enter item details (name, price, quantity) until you type `done`.

3. **Rollback**
   - The most recent customer's bill will be removed and the totals will be updated.

4. **Exit**
   - The program will save the daily total and clear the bills file for the next day.

## Example Usage

```
Choose a number: 1. Add Customer 2. Rollback 3. Exit
Enter your choice: 1
Enter username: john
Enter item name (or 'done' to finish): apple
Enter price: 1.5
Enter quantity: 4
Enter item name (or 'done' to finish): banana
Enter price: 0.5
Enter quantity: 6
Enter item name (or 'done' to finish): done

Choose a number: 1. Add Customer 2. Rollback 3. Exit
Enter your choice: 3
```

## Future Work

### 1. Create GUI

Develop a graphical user interface (GUI) to enhance the user experience. The GUI can be created using frameworks like Qt or wxWidgets.

### 2. Integrate SQLite Database

Replace the CSV files with a SQLite database to store customer and bill information. This will provide better data management and retrieval capabilities.

### 3. Develop REST APIs

Create REST APIs to enable remote access and management of the billing system. This will allow integration with other applications and services.

## Concepts Used

### Object-Oriented Programming (OOP)
- **Bill Class**: Represents a customer's bill, encapsulating the items, prices, quantities, and total amount.
- **UserManager Class**: Manages multiple Bill objects, providing functionalities to add customers, save bills, update daily totals, and roll back the most recent customer.

### Multithreading
- **Thread Pool**: A fixed number of worker threads are created to handle tasks concurrently. Tasks such as saving bills and updating totals are submitted to the thread pool for execution.
- **Mutex and Locking**: A mutex (`mtx`) is used to ensure that only one thread can access and modify shared data at a time, preventing data races.
- **Condition Variables**: Used to block threads until a condition is met, such as the availability of tasks in the task queue.

### File Handling
- **CSV Files**: Customer bills and daily totals are stored in CSV files. The `saveBillToFile` method writes data to `today_bills.csv`, and the `updateDailyTotal` method writes data to `daily_totals.csv`.
- **Clearing Files**: The `clearBillsFile` method is used to clear the content of `today_bills.csv` at the beginning of each day.

### Standard Library Usage
- **STL Containers**: The project uses `std::unordered_map` to map usernames to their respective Bill objects, `std::vector` to store items, prices, and quantities, and `std::stack` to manage the order of customer entries for rollback.
- **STL Algorithms**: Standard algorithms are implicitly used through container operations such as insertion, deletion, and iteration.

### Concurrency Control
- **Task Queue**: A queue is used to store tasks submitted to the thread pool. Worker threads continuously fetch and execute tasks from the queue.

### Error Handling
- **Input Validation**: The project ensures that price and quantity inputs are valid (positive numbers for price and whole numbers for quantity).
- **Exception Handling**: Exception handling is implemented to manage file I/O errors, ensuring the program handles errors gracefully.

### Command-Line Interface (CLI)
- **User Interaction**: The program provides a simple CLI that prompts the user to choose an action (add customer, roll back, or exit) and handles user input accordingly.
