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

    vector<string> promotions;
    vector<string> promocodes;
    vector<string> bonuses;
};

int main() {
    string usertype;
    setlocale(LC_CTYPE, "rus");
    vector<Product> store;
    bool storeOpen = true;

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
                    if (!storeOpen) {
                        cout << "Магазин зараз закритий. Відкрийте магазин спочатку." << endl;
                        continue;
                    }

                    Product newProduct;
                    string categoryChoice;

                    cout << "Оберіть категорію товару: ";
                    cin.ignore();
                    getline(cin, categoryChoice);

                    if (categoryChoice == "іграшка") {
                        newProduct.category = "Іграшка";
                    }
                    else if (categoryChoice == "одежа") {
                        newProduct.category = "Одежа";
                    }
                    else if (categoryChoice == "взуття") {
                        newProduct.category = "Взуття";
                    }
                    else if (categoryChoice == "речі для дітей до 2 років") {
                        newProduct.category = "Речі до 2 років";
                    }
                    else {
                        cout << "Невірний вибір категорії." << endl;
                        continue;
                    }

                    cout << "Введіть назву товару: ";
                    getline(cin, newProduct.name);
                    cout << "Введіть ціну товару: ";
                    cin >> newProduct.price;
                    cout << "Введіть кількість товару: ";
                    cin >> newProduct.quantity;

                    store.push_back(newProduct);
                    cout << "Товар успішно доданий." << endl;
                }
                else if (action == 2) {
                    if (storeOpen) {
                        storeOpen = false;
                        cout << "Магазин зачинено." << endl;
                    }
                    else {
                        storeOpen = true;
                        cout << "Магазин відкрито." << endl;
                    }
                }
                else if (action == 3) {
                    string promo;
                    cout << "Введіть опис акції: ";
                    cin.ignore();
                    getline(cin, promo);
                    for (auto& product : store) {
                        product.promotions.push_back(promo);
                    }
                    cout << "Акція успішно додана." << endl;
                }
                else if (action == 4) {
                    string code;
                    cout << "Введіть промокод: ";
                    cin.ignore();
                    getline(cin, code);
                    for (auto& product : store) {
                        product.promocodes.push_back(code);
                    }
                    cout << "Промокод успішно доданий." << endl;
                }
                else if (action == 5) {
                    string bonus;
                    cout << "Введіть опис бонусу: ";
                    cin.ignore();
                    getline(cin, bonus);
                    for (auto& product : store) {
                        product.bonuses.push_back(bonus);
                    }
                    cout << "Бонус успішно доданий." << endl;
                }
                else if (action == 6) {
                    if (store.size() == 0) {
                        cout << "Товарів поки немає." << endl;
                    }
                    else {
                        cout << "Список товарів:" << endl;
                        for (int i = 0; i < store.size(); i++) {
                            cout << i + 1 << ". Назва: " << store[i].name << ", Категорія: " << store[i].category
                                << ", Ціна: " << store[i].price << ", Кількість: " << store[i].quantity << endl;
                            cout << "Акції: ";
                            for (const auto& promo : store[i].promotions) {
                                cout << promo << " ";
                            }
                            cout << endl;
                            cout << "Промокоди: ";
                            for (const auto& code : store[i].promocodes) {
                                cout << code << " ";
                            }
                            cout << endl;
                            cout << "Бонуси: ";
                            for (const auto& bonus : store[i].bonuses) {
                                cout << bonus << " ";
                            }
                            cout << endl;
                        }
                    }
                }
                else if (action == 7) {
                    cout << "Вихід з акаунту Продавець." << endl;
                    break;
                }
                else {
                    cout << "Невірний вибір, спробуйте ще раз." << endl;
                }
            }
        }
        else if (usertype == "2" || usertype == "покупець") {
            cout << "Ви успішно увійшли як Покупець." << endl;
            break;
        }
        else {
            cout << "Невірний вибір, спробуйте ще раз." << endl;
        }
    }

    return 0;
}
