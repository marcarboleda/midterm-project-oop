#include <iostream>
#include <string>
#include <cstdlib>
#include <iomanip>
#include <limits>
using namespace std;

class Item {
private:
    // Encapsulation: Private attributes, encapsulating the internal state of the item.
    string id;
    string name;
    int quantity;
    double price;
    string category;

public:
    // Constructor to initialize item
    Item(string id, string name, int quantity, double price, string category)
            : id(id), name(name), quantity(quantity), price(price), category(category) {}

    // Getter methods
    string getId() const { return id; }
    string getName() const { return name; }
    int getQuantity() const { return quantity; }
    double getPrice() const { return price; }
    string getCategory() const { return category; }

    // Encapsulation
    // Setter methods
    void setQuantity(int newQuantity) { quantity = newQuantity; }
    void setPrice(double newPrice) { price = newPrice; }

    // Abstraction
    // public method to display the items
    void displayItem() const {
        cout << left << setw(10) << id << setw(20) << name << setw(10) << quantity << setw(10) << price << setw(15) << category << endl;
    }
};


class InventoryBase {
protected:
    Item* items[100] = {};
    int itemCount = 0;

public:
    ~InventoryBase() {
        for (int i = 0; i < itemCount; ++i) {
            delete items[i];
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
            default: return "";
        }
    }

    // virtual functions
    virtual void addItem(string id, string name, int quantity, double price, int category) = 0;

    virtual void updateItem(string id) = 0;

    virtual void removeItem(string id) = 0;

    virtual void displayItemsByCategory(int category) = 0;

    virtual void displayAllItems() = 0;

    virtual void searchItem(string id) = 0;

    virtual void sortItems(bool byQuantity, bool ascending) = 0;

    bool isEmpty() const {
        return itemCount == 0;
    }

    virtual void displayLowStockItems() = 0;
};

class Inventory: public InventoryBase {
private:

public:

    // Add new item to inventory
    void addItem(string id, string name, int quantity, double price, int category) override {
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
    void updateItem(string id) override  {
        // convert id to lowercase
        string lowercaseId = id;
        for (size_t i = 0; i < id.length(); ++i) {
            lowercaseId[i] = tolower(id[i]);
        }
        for (int i = 0; i < itemCount; ++i) {
            auto lowercaseItemId = items[i]->getId();
            for (size_t i = 0; i < lowercaseItemId.length(); ++i) {
                lowercaseItemId[i] = tolower(lowercaseItemId[i]);
            }
            if (lowercaseId == lowercaseItemId) {
                int choice;
                while (true) {
                    cout << "\n[1] Update Quantity\n[2] Update Price\nEnter choice: ";
                    cin >> choice;

                    if (cin.fail() || (choice != 1 && choice != 2)) {
                        cin.clear();
                        cin.ignore(numeric_limits<streamsize>::max(), '\n');
                        cout << "Invalid choice! Please enter 1 or 2." << endl;
                    } else {
                        cin.ignore(numeric_limits<streamsize>::max(), '\n');
                        break;
                    }
                }

                if (choice == 1) {
                    int newQuantity;
                    while (true) {
                        cout << "Enter new quantity: ";
                        cin >> newQuantity;

                        if (cin.fail() || newQuantity <= 0) {
                            cin.clear();
                            cin.ignore(numeric_limits<streamsize>::max(), '\n');
                            cout << "Invalid input. Please enter a positive integer." << endl;
                        } else {
                            cout << "Quantity of Item " << items[i]->getName() << " is updated from " << items[i]->getQuantity() << " to " << newQuantity << endl;
                            items[i]->setQuantity(newQuantity);
                            break;
                        }
                    }
                } else if (choice == 2) {
                    double newPrice;
                    while (true) {
                        cout << "Enter new price: ";
                        cin >> newPrice;

                        if (cin.fail() || newPrice <= 0) {
                            cin.clear();
                            cin.ignore(numeric_limits<streamsize>::max(), '\n');
                            cout << "Invalid input. Please enter a positive number." << endl;
                        } else {
                            cout << "Price of Item " << items[i]->getName() << " is updated from " << items[i]->getPrice() << " to " << newPrice << endl;
                            items[i]->setPrice(newPrice);
                            break;
                        }
                    }
                }
                return;
            }
        }
        cout << "Item not found!" << endl;
    }

    // Remove item from inventory
    void removeItem(string id) override {
        for (int i = 0; i < itemCount; ++i) {
            if (items[i]->getId() == id) {
                cout << "Item " << items[i]->getName() << " has been removed from the inventory." << endl;
                delete items[i];
                items[i] = items[--itemCount];
                return;
            }
        }
        cout << "Item not found!" << endl;
    }

    // Display all items by category
    void displayItemsByCategory(int category) override {
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
    void displayAllItems() override {
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
    void searchItem(const string id) override {
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
    void sortItems(bool byQuantity, bool ascending) override {
        for (int i = 0; i < itemCount - 1; ++i) {
            for (int j = 0; j < itemCount - i - 1; ++j) {
                bool condition;

                if (byQuantity) {
                    // Sorting by quantity
                    condition = ascending
                                ? (items[j]->getQuantity() > items[j + 1]->getQuantity())
                                : (items[j]->getQuantity() < items[j + 1]->getQuantity());
                } else {
                    // Sorting by price
                    condition = ascending
                                ? (items[j]->getPrice() > items[j + 1]->getPrice())
                                : (items[j]->getPrice() < items[j + 1]->getPrice());
                }

                // Swap if the condition is met
                if (condition) {
                    Item* temp = items[j];
                    items[j] = items[j + 1];
                    items[j + 1] = temp;
                }
            }
        }

        // Display sorted items
        cout << left << setw(10) << "ID" << setw(20) << "Name" << setw(10) << "Quantity" << setw(10) << "Price" << setw(15) << "Category" << endl;
        cout << "----------------------------------------------------------" << endl;
        for (int i = 0; i < itemCount; ++i) {
            items[i]->displayItem();
        }
    }

    // Display low stock items
    void displayLowStockItems() override {
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


};

bool isValidItemId(const string& id) {
    // if (id.length() != 3) {
    //     cout << "Invalid ID. It should must be three numbers." << endl;
    //     return false;
    // }
    for (size_t i = 0; i < id.length(); ++i) {
        if (!isalnum(id[i])) {
            cout << "Invalid ID. Try again" << endl;
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
            cin.clear(); // Clear the error flag
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
            cout << "Invalid input. Please enter a positive number: ";
        } else {
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
            return value;
        }
    }
}

int getValidInt() {
    int value;
    while (true) {
        cin >> value;
        if (cin.fail() || value <= 0) {
            cin.clear(); // Clear the error flag
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
            cout << "Invalid input. Please enter a positive integer: ";
        } else {
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
            return value;
        }
    }
}

int main() {
    Inventory inventory;
    string choice;

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
        cout << "==============================================\n";
        cout << "Enter your choice: ";
        cin >> choice;

        if (cin.fail() || (choice != "1" && choice != "2" && choice != "3" &&
                           choice != "4" && choice != "5" && choice != "6" &&
                           choice != "7" && choice != "8" && choice != "9")) {
            cin.clear();
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
            cout << "\nInvalid input. Please enter a valid option." << endl;
            cout << endl;
            continue;
        }

        if (choice == "1") {
            string id, name;
            int quantity, category;
            double price;

            while (true) {
                    cout << "\nSelect category:\n[1] Clothing\n[2] Electronics\n[3] Entertainment\nEnter choice: ";
                    category = getValidInt();

                    if (cin.fail() || (category != 1 && category != 2 && category != 3)) {
                        cin.clear();
                        cout << "Invalid choice! Please enter 1, 2, or 3." << endl;
                    } else {
                        break;
                    }
                }

            cout << "Enter item ID: ";
            cin >> id;
            while (!isValidItemId(id)) {
                cout << "Enter item ID: ";
                cin >> id;
            }

            cout << "Enter item name: ";
            cin.ignore();
            getline(cin, name);
            cout << "Enter quantity: ";
            quantity = getValidInt();
            cout << "Enter price: ";
            price = getValidDouble();

            inventory.addItem(id, name, quantity, price, category);
        }

        else if (choice == "2") {
            if (inventory.isEmpty()) {
                cout << "No items added yet!" << endl;
            } else {
                string id;
                cout << "\nEnter item ID to update: ";
                cin >> id;
                inventory.updateItem(id);
                cout << "\n";
            }
        }

        else if (choice == "3") {
            if (inventory.isEmpty()) {
                cout << "No items added yet!" << endl;
            } else {
                string id;
                cout << "Enter item ID to remove: ";
                cin >> id;
                inventory.removeItem(id);
                cout << "\n";
            }
        }

        else if (choice == "4") {
            if (inventory.isEmpty()) {
                cout << "No items added yet!" << endl;
            }
            else {
                int category;
                while (true) {
                    cout << "\nSelect category:\n[1] Clothing\n[2] Electronics\n[3] Entertainment\nEnter choice: ";
                    category = getValidInt();

                    if (cin.fail() || (category != 1 && category != 2 && category != 3)) {
                        cin.clear();
                        cout << "Invalid choice! Please enter 1, 2, or 3." << endl;
                    } else {
                        break;
                    }
                }
                cout << "\n";
                inventory.displayItemsByCategory(category);
                cout << "\n";
            }
        }

        else if (choice == "5") {
            if (inventory.isEmpty()) {
                cout << "No items added yet!" << endl;
            } else {
                cout << "\n";
                inventory.displayAllItems();
                cout << "\n";
            }
        }

        else if (choice == "6") {
            if (inventory.isEmpty()) {
                cout << "No items added yet!" << endl;
            } else {
                string id;
                cout << "\nEnter item ID to search: ";
                cin >> id;
                inventory.searchItem(id);
                cout << "\n";
            }
        }

        else if (choice == "7") {
            if (inventory.isEmpty()) {
                cout << "No items added yet!" << endl;
            } else {
                int sortType, sortOrder;

                while (true) {
                    cout << "\n[1] Sort by Quantity\n[2] Sort by Price\nEnter choice: ";
                    sortType = getValidInt();

                    if (sortType == 1 || sortType == 2) {
                        break;
                    } else {
                        cout << "Invalid choice. Please enter 1 or 2." << endl;
                    }
                }

                bool byQuantity = (sortType == 1);

                while (true) {
                    cout << "\n[1] Ascending\n[2] Descending\nEnter choice: ";
                    sortOrder = getValidInt();

                    if (sortOrder == 1 || sortOrder == 2) {
                        break;
                    } else {
                        cout << "Invalid choice. Please enter 1 or 2." << endl;
                    }
                }

                bool ascending = (sortOrder == 1);
                cout << "\n";
                inventory.sortItems(byQuantity, ascending);
            }
            cout << "\n";
        }

        else if (choice == "8") {
            if (inventory.isEmpty()) {
                cout << "No items added yet!" << endl;
            } else {
                cout << "\n";
                inventory.displayLowStockItems();
                cout << "\n";
            }
        }

        else if (choice == "9") {
            cout << "\n";
            cout << "Exiting program..." << endl;
        }

        else {
            cout << "\nInvalid option. Please try again." << endl;
            cout << endl;
            continue;
        }
    } while (choice != "9");

    return 0;
}