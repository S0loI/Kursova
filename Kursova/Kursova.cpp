#include <iostream>
#include <string>
#include <vector>
#include <locale.h>


using namespace std;

struct Product {
    string name;
    string category;
    double price;
    int quantity;

    vector<Product> store;
    bool storeOpen = true;
    vector<string> promotions;
    vector<string> promocodes;
    vector<string> bonuses;

int main() {
    string usertype;
    setlocale (LC_CTYPE, "rus");

    while (true) {
        cout << "Виберіть акаунт для входу:" << endl ;
        cout << "1. Продавець"<< endl;
        cout << "2. Покупець" << endl;
        cout << "Ваш вибір: ";
        cin >> usertype;

        if (usertype == "1" || usertype == "продавець") {
            cout << "Ви успішно увійшли як Продавець." << endl;
            break;
        }
        else if (usertype == "2" || usertype == "покупець") {
            cout << "Ви успішно увійшли як Покупець." << endl;
            break;
        }
        else {
            cout << "Невірний вибір, спробуйте ще раз.";
        }
    }

    return 0;
}
