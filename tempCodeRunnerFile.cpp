double getValidDouble() {
    double value;
    while (true) {
        cin >> value;
        if (cin.fail() || value <= 0) {
            cin.clear(); 
            cin.ignore(numeric_limits<streamsize>::max(), '\n'); 
            cout << "Invalid input. Please enter a positive number: ";
        } else {
            cin.ignore(numeric_limits<streamsize>::max(), '\n'); 
            return value;
        }
    }
}