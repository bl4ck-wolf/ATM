#include <iostream>
#include <fstream>
#include <vector>
#include <limits>
#include <nlohmann/json.hpp>

using namespace std;
using json = nlohmann::json;

struct Account {
    string username;
    string pin;
    double balance;
    vector<string> history;
};

// ---------- FILE HANDLING ----------
vector<Account> loadAccounts() {
    vector<Account> accounts;
    ifstream file("accounts.json");

    if (!file.is_open()) return accounts;

    json j;
    file >> j;

    for (auto &item : j) {
        Account acc;
        acc.username = item["username"];
        acc.pin = item["pin"];
        acc.balance = item["balance"];
        acc.history = item["history"].get<vector<string>>();
        accounts.push_back(acc);
    }

    return accounts;
}

void saveAccounts(vector<Account> &accounts) {
    json j;

    for (auto &acc : accounts) {
        j.push_back({
            {"username", acc.username},
            {"pin", acc.pin},
            {"balance", acc.balance},
            {"history", acc.history}
        });
    }

    ofstream file("accounts.json");
    file << j.dump(4);
}

// ---------- UTIL ----------
void pause() {
    cout << "\nPress Enter...";
    cin.ignore(numeric_limits<streamsize>::max(), '\n');
    cin.get();
}

// ---------- SYSTEM ----------
Account* login(vector<Account> &accounts) {
    string username, pin;

    cout << "Username: ";
    cin >> username;
    cout << "PIN: ";
    cin >> pin;

    for (auto &acc : accounts) {
        if (acc.username == username && acc.pin == pin) {
            return &acc;
        }
    }

    cout << "Invalid credentials.\n";
    return nullptr;
}

void registerUser(vector<Account> &accounts) {
    Account acc;

    cout << "Create username: ";
    cin >> acc.username;

    cout << "Create PIN: ";
    cin >> acc.pin;

    acc.balance = 0;

    accounts.push_back(acc);

    cout << "Account created successfully.\n";
}

// ---------- OPERATIONS ----------
void checkBalance(Account &acc) {
    cout << "Balance: TZS " << acc.balance << endl;
}

void deposit(Account &acc) {
    int n5000, n10000;

    cout << "5000 notes: ";
    cin >> n5000;
    cout << "10000 notes: ";
    cin >> n10000;

    double amount = n5000 * 5000 + n10000 * 10000;

    acc.balance += amount;
    acc.history.push_back("Deposited: " + to_string(amount));

    cout << "Deposited TZS " << amount << endl;
}

void withdraw(Account &acc) {
    double amount;

    cout << "Enter amount: ";
    cin >> amount;

    if (amount <= 0 || amount > acc.balance || ((int)amount % 5000 != 0)) {
        cout << "Invalid amount.\n";
        return;
    }

    acc.balance -= amount;
    acc.history.push_back("Withdrawn: " + to_string(amount));

    cout << "Withdrawn TZS " << amount << endl;
}

void showHistory(Account &acc) {
    cout << "\nHistory:\n";
    for (auto &h : acc.history) {
        cout << "- " << h << endl;
    }
}

// ---------- USER MENU ----------
void userMenu(Account &acc, vector<Account> &accounts) {
    int choice;

    do {
        cout << "\n1. Balance\n2. Deposit\n3. Withdraw\n4. History\n5. Logout\nChoice: ";
        cin >> choice;

        switch (choice) {
            case 1: checkBalance(acc); break;
            case 2: deposit(acc); break;
            case 3: withdraw(acc); break;
            case 4: showHistory(acc); break;
            case 5: saveAccounts(accounts); break;
            default: cout << "Invalid choice\n";
        }

        if (choice != 5) pause();

    } while (choice != 5);
}

// ---------- MAIN ----------
int main() {
    vector<Account> accounts = loadAccounts();

    int choice;

    do {
        cout << "\n=== AMANA TERMINAL BANK ===\n";
        cout << "1. Login\n2. Register\n3. Exit\nChoice: ";
        cin >> choice;

        switch (choice) {
            case 1: {
                Account* user = login(accounts);
                if (user) userMenu(*user, accounts);
                break;
            }
            case 2:
                registerUser(accounts);
                saveAccounts(accounts);
                break;
            case 3:
                saveAccounts(accounts);
                cout << "Goodbye.\n";
                break;
            default:
                cout << "Invalid choice\n";
        }

    } while (choice != 3);

    return 0;
}