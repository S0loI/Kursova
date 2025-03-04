#include <iostream>
#include <string>
#include <lcoal.h>


using namespace std;

int main() {
    string usertype;
    set local(LC_CTYPE, "rus");

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
