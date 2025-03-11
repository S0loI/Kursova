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
        setlocale(LC_CTYPE, "rus");

        while (true) {
            cout << "Виберіть акаунт для входу:" << endl;
            cout << "1. Продавець" << endl;
            cout << "2. Покупець" << endl;
            cout << "Ваш вибір: ";
            cin >> usertype;

            if (usertype == "1" || usertype == "продавець") {
                cout << "Ви успішно увійшли як Продавець." << endl;

                while (true) {
                    int action;
                    cout << endl;
                    cout << "Що ви хочете зробити?" << endl;
                    cout << "1. Додати товар" << endl;
                    cout << "2. Відкрити/Закрити магазин" << endl;
                    cout << "3. Створити акцію" << endl;
                    cout << "4. Додати промокод" << endl;
                    cout << "5. Додати бонус" << endl;
                    cout << "6. Показати всі товари" << endl;
                    cout << "7. Вийти" << endl;
                    cout << "Ваш вибір: ";
                    cin >> action;

                    if (action == 1) {
                        if (storeOpen == false) {
                            cout << "Магазин зараз закритий. Відкрийте магазин спочатку." << endl;
                            continue;
                        }

                        Product newProduct;
                        int categoryChoice;
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


            }
        }
        return 0;
    }