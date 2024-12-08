#include <iostream>
#include "tc.h"
#include "mysql.cpp"

using namespace std;

int main() {
    dbConnection db;

    string username, password;

    while (true) {
        system("cls");

        cout << "Enter id: ";
        cin >> username;
        cout << "Enter password: ";
        cin >> password;

        if (db.login(username, password)) {
            cout << "Welcome, " << username << "!" << endl;
            break;
        }
    }

    return 0;
}
