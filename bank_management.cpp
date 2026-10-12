#include <iostream>
#include <fstream>
#include <string>
#include <iomanip>
#include <limits>
#include <cstdio>
using namespace std;

class BankAccount {
private:
    int accountNumber;
    string name;
    double balance;

public:
    BankAccount() : accountNumber(0), name(""), balance(0.0) {}

    BankAccount(int accNo, const string& customerName, double amount)
        : accountNumber(accNo), name(customerName), balance(amount) {}

    int getAccountNumber() const { return accountNumber; }
    string getName() const { return name; }
    double getBalance() const { return balance; }

    bool deposit(double amount) {
        if (amount <= 0) {
            cout << "Enter a valid deposit amount!\n";
            return false;
        }
        balance += amount;
        return true;
    }

    bool withdraw(double amount) {
        if (amount <= 0) {
            cout << "Enter a valid withdrawal amount!\n";
            return false;
        }
        if (amount > balance) {
            cout << "Insufficient balance!\n";
            return false;
        }
        balance -= amount;
        return true;
    }

    void display() const {
        cout << "\nAccount Number : " << accountNumber
             << "\nCustomer Name : " << name
             << "\nBalance       : Rs. "
             << fixed << setprecision(2) << balance << "\n";
    }
};

const string FILE_NAME = "accounts.txt";

bool accountExists(int accNo) {
    ifstream file(FILE_NAME);
    int number;
    string customerName;
    double amount;

    while (file >> number >> quoted(customerName) >> amount) {
        if (number == accNo) return true;
    }
    return false;
}

void createAccount() {
    int accNo;
    string customerName;
    double amount;

    cout << "\nEnter Account Number: ";
    if (!(cin >> accNo) || accNo <= 0) {
        cout << "Invalid account number!\n";
        cin.clear();
        cin.ignore(numeric_limits<streamsize>::max(), '\n');
        return;
    }

    if (accountExists(accNo)) {
        cout << "Account already exists!\n";
        return;
    }

    cin.ignore(numeric_limits<streamsize>::max(), '\n');
    cout << "Enter Customer Name: ";
    getline(cin, customerName);
    if (customerName.empty()) {
        cout << "Name cannot be empty!\n";
        return;
    }

    cout << "Enter Initial Deposit: Rs. ";
    if (!(cin >> amount) || amount < 0) {
        cout << "Invalid amount!\n";
        cin.clear();
        cin.ignore(numeric_limits<streamsize>::max(), '\n');
        return;
    }

    ofstream file(FILE_NAME, ios::app);
    if (!file) {
        cout << "Unable to open account file!\n";
        return;
    }

    file << accNo << ' ' << quoted(customerName) << ' '
         << fixed << setprecision(2) << amount << '\n';
    cout << "Account created successfully!\n";
}

void displayAccount(int accNo) {
    ifstream file(FILE_NAME);
    int number;
    string customerName;
    double amount;

    while (file >> number >> quoted(customerName) >> amount) {
        if (number == accNo) {
            BankAccount account(number, customerName, amount);
            account.display();
            return;
        }
    }
    cout << "Account not found!\n";
}

void updateBalance(int accNo, double amount, bool isDeposit) {
    ifstream file(FILE_NAME);
    if (!file) {
        cout << "No accounts found. Create an account first.\n";
        return;
    }

    const string tempName = "temp.txt";
    ofstream temp(tempName);
    if (!temp) {
        cout << "Unable to create temporary file!\n";
        return;
    }

    int number;
    string customerName;
    double balance;
    bool found = false;
    bool valid = true;

    while (file >> number >> quoted(customerName) >> balance) {
        if (number == accNo) {
            found = true;
            BankAccount account(number, customerName, balance);

            if (isDeposit) {
                if (!account.deposit(amount)) {
                    valid = false;
                    break;
                }
                cout << "Deposit successful!\n";
            } else {
                if (!account.withdraw(amount)) {
                    valid = false;
                    break;
                }
                cout << "Withdrawal successful!\n";
            }
            balance = account.getBalance();
        }

        temp << number << ' ' << quoted(customerName) << ' '
             << fixed << setprecision(2) << balance << '\n';
    }

    file.close();
    temp.close();

    if (!valid) {
        remove(tempName.c_str());
        return;
    }

    if (!found) {
        remove(tempName.c_str());
        cout << "Account not found!\n";
        return;
    }

    if (remove(FILE_NAME.c_str()) != 0) {
        remove(tempName.c_str());
        cout << "Unable to update account file!\n";
        return;
    }

    if (rename(tempName.c_str(), FILE_NAME.c_str()) != 0) {
        cout << "Unable to save account changes!\n";
        return;
    }

    cout << "Balance updated successfully!\n";
}

int main() {
    int choice;
    int accNo;
    double amount;

    do {
        cout << "\n================================\n"
             << "      BANK MANAGEMENT SYSTEM\n"
             << "================================\n"
             << "1. Create Account\n"
             << "2. Deposit Money\n"
             << "3. Withdraw Money\n"
             << "4. Check Balance\n"
             << "5. Display Account Details\n"
             << "6. Exit\n"
             << "================================\n"
             << "Enter your choice: ";

        if (!(cin >> choice)) {
            cout << "Please enter a valid choice!\n";
            cin.clear();
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
            continue;
        }

        switch (choice) {
            case 1:
                createAccount();
                break;

            case 2:
            case 3:
                cout << "Enter Account Number: ";
                if (!(cin >> accNo) || accNo <= 0) {
                    cout << "Invalid account number!\n";
                    cin.clear();
                    cin.ignore(numeric_limits<streamsize>::max(), '\n');
                    break;
                }

                cout << (choice == 2 ? "Enter Deposit Amount: Rs. "
                                     : "Enter Withdrawal Amount: Rs. ");
                if (!(cin >> amount) || amount <= 0) {
                    cout << "Invalid amount!\n";
                    cin.clear();
                    cin.ignore(numeric_limits<streamsize>::max(), '\n');
                    break;
                }
                updateBalance(accNo, amount, choice == 2);
                break;

            case 4:
            case 5:
                cout << "Enter Account Number: ";
                if (!(cin >> accNo) || accNo <= 0) {
                    cout << "Invalid account number!\n";
                    cin.clear();
                    cin.ignore(numeric_limits<streamsize>::max(), '\n');
                    break;
                }
                displayAccount(accNo);
                break;

            case 6:
                cout << "Thank you for using our bank system!\n";
                break;

            default:
                cout << "Invalid choice! Try again.\n";
        }
    } while (choice != 6);

    return 0;
}
