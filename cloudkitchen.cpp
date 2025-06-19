#include <iostream>
#include <fstream>
#include <iomanip>
#include <ctime>
#include <vector>
#include <string>
using namespace std;

// ========== Abstract User ==========
class User {
protected:
    string name, contact;
public:
    User(string n = "", string c = "") : name(n), contact(c) {}
    virtual void displayProfile() = 0;
    string getName() { return name; }
    string getContact() { return contact; }
    virtual ~User() {}
};

// ========== Customer Classes ==========
class Customer : public User {
protected:
    vector<string> orderHistory;
public:
    Customer(string n = "", string c = "") : User(n, c) {}
    virtual float calculateBill(float baseAmount) = 0;

    void addToHistory(string item) {
        orderHistory.push_back(item);
    }

    void showHistory() {
        cout << "\n--- Your Order History ---\n";
        for (string item : orderHistory)
            cout << " * " << item << endl;
    }

    void saveToFile(float base, float final, string delivery, string type) {
        ofstream fout("orders.txt", ios::app);
        time_t now = time(0);
        fout << "-----------------------------\n";
        fout << "Customer: " << name << ", Contact: " << contact << ", Type: " << type << "\n";
        fout << "Time: " << ctime(&now);
        for (string item : orderHistory)
            fout << " - " << item << endl;
        fout << "Base Total: Rs. " << base << ", Final Bill: Rs. " << final << endl;
        fout << "Delivered By: " << delivery << "\n";
        fout << "-----------------------------\n\n";
        fout.close();
    }
};

class RegularCustomer : public Customer {
public:
    RegularCustomer(string n, string c) : Customer(n, c) {}
    float calculateBill(float baseAmount) {
        return baseAmount + 50;
    }
    void displayProfile() {
        cout << "[Regular Customer] Name: " << name << ", Contact: " << contact << endl;
    }
};

class PremiumCustomer : public Customer {
public:
    PremiumCustomer(string n, string c) : Customer(n, c) {}
    float calculateBill(float baseAmount) {
        return baseAmount * 0.9;
    }
    void displayProfile() {
        cout << "[Premium Customer] Name: " << name << ", Contact: " << contact << endl;
    }
};

// ========== Admin ==========
class Admin : public User {
private:
    string password;
public:
    Admin(string n = "Admin", string c = "N/A", string p = "1234") : User(n, c), password(p) {}

    bool login(string input) {
        return input == password;
    }

    void displayProfile() {
        cout << "[Admin] " << name << "\n";
    }

    void viewAllOrders() {
        ifstream fin("orders.txt");
        if (!fin.is_open()) {
            cout << "No orders found.\n";
            return;
        }
        string line;
        cout << "\n--- All Orders ---\n";
        while (getline(fin, line)) {
            cout << line << endl;
        }
        fin.close();
    }

    void searchCustomerOrder(string key) {
        ifstream fin("orders.txt");
        if (!fin.is_open()) {
            cout << "No orders found.\n";
            return;
        }
        string line;
        bool found = false;
        while (getline(fin, line)) {
            if (line.find(key) != string::npos) {
                found = true;
                cout << "\n" << line << endl;
                while (getline(fin, line) && line.find("-----------------------------") == string::npos) {
                    cout << line << endl;
                }
                cout << "-----------------------------\n";
            }
        }
        fin.close();
        if (!found)
            cout << "No record found for: " << key << endl;
    }
};

// ========== Food & Order ==========
class FoodItem {
    int id;
    string name;
    float price;
public:
    FoodItem(int i, string n, float p) : id(i), name(n), price(p) {}
    void display() {
        cout << id << ". " << name << " - Rs." << price << endl;
    }
    int getId() { return id; }
    string getName() { return name; }
    float getPrice() { return price; }
};

class Menu {
    vector<FoodItem> items;
public:
    Menu() {
        items.push_back(FoodItem(1, "Zinger Burger", 350));
        items.push_back(FoodItem(2, "Large Pizza", 1200));
        items.push_back(FoodItem(3, "Fries", 150));
        items.push_back(FoodItem(4, "Cold Drink", 80));
        items.push_back(FoodItem(5, "Biryani", 250));
        items.push_back(FoodItem(6, "Shawarma", 200));
        items.push_back(FoodItem(7, "Pasta", 300));
        items.push_back(FoodItem(8, "Ice Cream", 100));
        items.push_back(FoodItem(9, "Nuggets", 180));
        items.push_back(FoodItem(10, "Garlic Bread", 130));
        items.push_back(FoodItem(11, "Roll Paratha", 150));
    }

    void showMenu() {
        cout << "\n--- MENU ---\n";
        for (FoodItem f : items)
            f.display();
    }

    FoodItem* getItem(int id) {
        for (int i = 0; i < items.size(); i++) {
            if (items[i].getId() == id)
                return &items[i];
        }
        return NULL;
    }
};

class DeliveryPartner {
    string name;
    string vehicle;
public:
    DeliveryPartner(string n = "Rider Ahmed", string v = "Bike") : name(n), vehicle(v) {}
    string getName() { return name; }
    void show() {
        cout << "Delivered by: " << name << " via " << vehicle << endl;
    }
};

// ========== Kitchen System ==========
class KitchenSystem {
    Menu menu;
    DeliveryPartner dp;
public:
    void run() {
        while (true) {
            int choice;
            cout << "\n--- CLOUD KITCHEN ---\n";
            cout << "1. Customer Mode\n2. Admin Mode\n3. Exit\nChoose Option: ";
            cin >> choice;

            if (choice == 3) break;

            if (choice == 2) {
                Admin a("Admin", "N/A", "1234");
                string pass;
                cout << "Enter Admin Password: ";
                cin >> pass;
                if (a.login(pass)) {
                    cout << "\n1. View All Orders\n2. Search Customer History\nChoose: ";
                    int adminOpt;
                    cin >> adminOpt;
                    if (adminOpt == 1)
                        a.viewAllOrders();
                    else if (adminOpt == 2) {
                        cin.ignore();
                        string key;
                        cout << "Enter name or contact: ";
                        getline(cin, key);
                        a.searchCustomerOrder(key);
                    }
                } else {
                    cout << "Incorrect Password!\n";
                }
            }

            else if (choice == 1) {
                int type;
                cout << "\n1. Regular Customer\n2. Premium Customer\nChoose Type: ";
                cin >> type;
                cin.ignore();
                string name, contact;
                cout << "Enter Name: "; getline(cin, name);
                cout << "Enter Contact: "; getline(cin, contact);

                Customer* cust;
                string custType;
                if (type == 1) {
                    cust = new RegularCustomer(name, contact);
                    custType = "Regular";
                } else {
                    cust = new PremiumCustomer(name, contact);
                    custType = "Premium";
                }

                cust->displayProfile();
                char more = 'y';
                float total = 0;
                while (more == 'y' || more == 'Y') {
                    menu.showMenu();
                    int id, qty;
                    cout << "Enter Item ID: "; cin >> id;
                    cout << "Quantity: "; cin >> qty;
                    FoodItem* item = menu.getItem(id);
                    if (item) {
                        string entry = item->getName() + " x" + to_string(qty);
                        cust->addToHistory(entry);
                        total += item->getPrice() * qty;
                        cout << "Added!\n";
                    } else {
                        cout << "Invalid item.\n";
                    }
                    cout << "Add more? (y/n): ";
                    cin >> more;
                }

                float final = cust->calculateBill(total);
                cout << "\nTotal: Rs. " << total << "\nFinal Bill: Rs. " << final << endl;
                dp.show();
                cust->showHistory();
                cust->saveToFile(total, final, dp.getName(), custType);

                delete cust;
            }
        }
    }
};

// ========== Main ==========
int main() {
    KitchenSystem ks;
    ks.run();
    return 0;
}
