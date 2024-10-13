#include <iostream>
#include <string>
#include <cstdlib>
#include <iomanip>
#include <limits>
using namespace std;

class Item {
private:
    string id;
    string name;
    int quantity;
    double price;
    string category;

public:
    Item(string id, string name, int quantity, double price, string category)
            : id(id), name(name), quantity(quantity), price(price), category(category) {}

    // Getter methods
    string getId() const { return id; }
    string getName() const { return name; }
    int getQuantity() const { return quantity; }
    double getPrice() const { return price; }
    string getCategory() const { return category; }

    // Setter methods
    void setQuantity(int newQuantity) { quantity = newQuantity; }
    void setPrice(double newPrice) { price = newPrice; }

    // Display item details in a table format
    void displayItem() const {
        cout << left << setw(10) << id << setw(20) << name << setw(10) << quantity << setw(10) << price << setw(15) << category << endl;
    }
};

class Inventory {
private:
    Item* items[100];
    int itemCount;

public:
    Inventory() : itemCount(0) {}

    ~Inventory() {
        for (int i = 0; i < itemCount; ++i) {
            delete items[i];  // Free memory for each item
        }
    }

    int getItemCount() const { return itemCount; }

    bool isValidCategory(int category) const {
        return category >= 1 && category <= 3;
    }

    string categoryToString(int category) const {
        switch (category) {
            case 1: return "Clothing";
            case 2: return "Electronics";
            case 3: return "Entertainment";
            default: return "Unknown";
        }
    }

    // Add new item to inventory
    void addItem(string id, string name, int quantity, double price, int category) {
        if (!isValidCategory(category)) {
            cout << "Category does not exist!" << endl;
            return;
        }

        if (itemCount < 100) {
            items[itemCount++] = new Item(id, name, quantity, price, categoryToString(category));
            cout << "Item added successfully!" << endl;
        } else {
            cout << "Inventory is full!" << endl;
        }
    }

    // Update item quantity or price
    void updateItem(string id) {
        for (int i = 0; i < itemCount; ++i) {
            if (items[i]->getId() == id) {
                int choice;
                cout << "[1] Update Quantity\n[2] Update Price\nEnter choice: ";
                cin >> choice;

                if (choice == 1) {
                    int newQuantity;
                    cout << "Enter new quantity: ";
                    cin >> newQuantity;
                    cout << "Quantity of Item " << items[i]->getName() << " is updated from " << items[i]->getQuantity() << " to " << newQuantity << endl;
                    items[i]->setQuantity(newQuantity);
                } else if (choice == 2) {
                    double newPrice;
                    cout << "Enter new price: ";
                    cin >> newPrice;
                    cout << "Price of Item " << items[i]->getName() << " is updated from " << items[i]->getPrice() << " to " << newPrice << endl;
                    items[i]->setPrice(newPrice);
                } else {
                    cout << "Invalid choice!" << endl;
                }
                return;
            }
        }
        cout << "Item not found!" << endl;
    }

    // Remove item from inventory
    void removeItem(string id) {
        for (int i = 0; i < itemCount; ++i) {
            if (items[i]->getId() == id) {
                cout << "Item " << items[i]->getName() << " has been removed from the inventory." << endl;
                delete items[i];  // Free memory
                items[i] = items[--itemCount];  // Replace with last item
                return;
            }
        }
        cout << "Item not found!" << endl;
    }

    // Display all items by category
    void displayItemsByCategory(int category) const {
        if (!isValidCategory(category)) {
            cout << "Category does not exist!" << endl;
            return;
        }

        bool found = false;
        cout << left << setw(10) << "ID" << setw(20) << "Name" << setw(10) << "Quantity" << setw(10) << "Price" << endl;
        cout << "----------------------------------------------------------" << endl;

        for (int i = 0; i < itemCount; ++i) {
            if (items[i]->getCategory() == categoryToString(category)) {
                items[i]->displayItem();
                found = true;
            }
        }
        if (!found) cout << "No items found in this category." << endl;
    }

    // Display all items in a table format
    void displayAllItems() const {
        if (itemCount == 0) {
            cout << "No items in the inventory." << endl;
        } else {
            cout << left << setw(10) << "ID" << setw(20) << "Name" << setw(10) << "Quantity" << setw(10) << "Price" << setw(15) << "Category" << endl;
            cout << "---------------------------------------------------------------------" << endl;
            for (int i = 0; i < itemCount; ++i) {
                items[i]->displayItem();
            }
        }
    }

    // Search item by ID
    void searchItem(string id) const {
        for (int i = 0; i < itemCount; ++i) {
            if (items[i]->getId() == id) {
                cout << left << setw(10) << "ID" << setw(20) << "Name" << setw(10) << "Quantity" << setw(10) << "Price" << setw(15) << "Category" << endl;
                cout << "---------------------------------------------------------------------" << endl;
                items[i]->displayItem();
                return;
            }
        }
        cout << "Item not found!" << endl;
    }

    // Sort items (by quantity or price, ascending or descending)
    void sortItems(bool byQuantity, bool ascending) {
        for (int i = 0; i < itemCount - 1; ++i) {
            for (int j = 0; j < itemCount - i - 1; ++j) {
                bool condition = byQuantity
                                 ? (ascending ? items[j]->getQuantity() > items[j + 1]->getQuantity()
                                              : items[j]->getQuantity() < items[j + 1]->getQuantity())
                                 : (ascending ? items[j]->getPrice() > items[j + 1]->getPrice()
                                              : items[j]->getPrice() < items[j + 1]->getPrice());

                if (condition) {
                    Item* temp = items[j];
                    items[j] = items[j + 1];
                    items[j + 1] = temp;
                }
            }
        }

        // Display sorted items
        cout << left << setw(10) << "ID" << setw(20) << "Name" << setw(10) << "Quantity" << setw(10) << "Price" << endl;
        cout << "----------------------------------------------------------" << endl;
        for (int i = 0; i < itemCount; ++i) {
            items[i]->displayItem();
        }
    }

    // Display low stock items (quantity <= 5)
    void displayLowStockItems() const {
        bool found = false;
        cout << left << setw(10) << "ID" << setw(20) << "Name" << setw(10) << "Quantity" << setw(10) << "Price" << setw(15) << "Category" << endl;
        cout << "---------------------------------------------------------------------" << endl;
        for (int i = 0; i < itemCount; ++i) {
            if (items[i]->getQuantity() <= 5) {
                items[i]->displayItem();
                found = true;
            }
        }
        if (!found) cout << "No low stock items found." << endl;
    }

    bool isEmpty() const {
        return itemCount == 0;
    }
};

bool isValidItemId(const string& id) {
    if (id.length() != 3) {
        cout << "Invalid ID. It should must be three numbers." << endl;
        return false;
    }
    for (size_t i = 0; i < id.length(); ++i) {
        if (!isdigit(id[i])) {
            cout << "Invalid ID. Please input numbers only" << endl;
            return false;
        }
    }
    return true;
}


double getValidDouble() {
    double value;
    while (true) {
        cin >> value;
        if (cin.fail() || value <= 0) {
            cin.clear();
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
            cout << "Invalid input. Please enter a positive number: ";
        } else {
            return value;
        }
    }
}

int getValidInt() {
    int value;
    while (true) {
        cin >> value;
        if (cin.fail() || value <= 0) {
            cin.clear();
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
            cout << "Invalid input. Please enter a positive integer: ";
        } else {
            return value;
        }
    }
}

int main() {
    Inventory inventory;
    int choice;

    do {
        cout << "\n==================== MENU ====================\n";
        cout << "[1] - Add Item\n";
        cout << "[2] - Update Item\n";
        cout << "[3] - Remove Item\n";
        cout << "[4] - Display Items by Category\n";
        cout << "[5] - Display All Items\n";
        cout << "[6] - Search Item\n";
        cout << "[7] - Sort Items\n";
        cout << "[8] - Display Low Stock Items\n";
        cout << "[9] - Exit\n";
        cout << "\n==============================================\n";
        cout << "Enter your choice: ";

        while (true) {
            cin >> choice;
            if (cin.fail()) {
                cin.clear();
                cin.ignore(numeric_limits<streamsize>::max(), '\n');
                cout << "Invalid input! Please enter a number (1-9): ";
            } else {
                cin.ignore(numeric_limits<streamsize>::max(), '\n');
                break;
            }
        }

        switch (choice) {
            case 1: {
                cout << "Select category (1: Clothing, 2: Electronics, 3: Entertainment): ";
                int category = getValidInt();
                string id, name;
                cout << "Enter item ID (3 digits): ";
                cin >> id;

                if (!isValidItemId(id)) {
                    cout << "Invalid ID format. ID should be 3 digits." << endl;
                    break;
                }
                cout << "Enter item name: ";
                cin.ignore();
                getline(cin, name);
                cout << "Enter quantity: ";
                int quantity = getValidInt();
                cout << "Enter price: ";
                double price = getValidDouble();

                inventory.addItem(id, name, quantity, price, category);
                break;
            }
            case 2: {
                if (inventory.isEmpty()) {
                    system("cls");
                    cout << "No items added yet!" << endl;
                } else {
                    string id;
                    cout << "Enter item ID to update: ";
                    cin >> id;
                    inventory.updateItem(id);
                }
            break;
            }
            case 3: {
                if (inventory.isEmpty()) {
                    system("cls");
                    cout << "No items added yet!" << endl;
                } else {
                    string id;
                    cout << "Enter item ID to remove: ";
                    cin >> id;
                    inventory.removeItem(id);
                }
                break;
            }
            case 4: {
                if (inventory.isEmpty()) {
                    system("cls");
                    cout << "No items added yet!" << endl;
                } else {
                    cout << "Select category (1: Clothing, 2: Electronics, 3: Entertainment): ";
                    int category = getValidInt();
                    inventory.displayItemsByCategory(category);
                }
                break;
            }
            case 5:
                if (inventory.isEmpty()) {
                    system("cls");
                    cout << "No items added yet!" << endl;
                } else {
                    inventory.displayAllItems();
                }
                break;
            case 6: {
                if (inventory.isEmpty()) {
                    system("cls");
                    cout << "No items added yet!" << endl;
                } else {
                    string id;
                    cout << "Enter item ID to search: ";
                    cin >> id;
                    inventory.searchItem(id);
                }
                break;
            }
            case 7: {
                if (inventory.isEmpty()) {
                    system("cls");
                    cout << "No items added yet!" << endl;
                } else {
                    int choice;
                    cout << "[1] Sort by Quantity\n[2] Sort by Price\nEnter choice: ";
                    cin >> choice;

                    bool byQuantity = (choice == 1);
                    cout << "[1] Ascending\n[2] Descending\nEnter choice: ";
                    cin >> choice;
                    bool ascending = (choice == 1);
                    inventory.sortItems(byQuantity, ascending);
                }
                break;
            }
            case 8:
                if (inventory.isEmpty()) {
                    system("cls");
                    cout << "No items added yet!" << endl;
                } else {
                    inventory.displayLowStockItems();
                }
                break;
            case 9:
                cout << "Exiting the program." << endl;
                break;
            default:
                cout << "Invalid choice! Please try again." << endl;
                break;
        }
        cout << "\n";
        system("pause");
        system("cls");
    } while (choice != 9);

    return 0;
}

