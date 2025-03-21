#include <iostream>
#include <string>
#include <vector>
#include <locale>

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
    setlocale(LC_ALL, "uk_UA.UTF-8");

    string usertype;
    vector<Product> store;
    bool storeOpen = true;

    while (true) {
        cout << "Виберіть акаунт для входу:\n";
        cout << "1. Продавець\n";
        cout << "2. Покупець\n";
        cout << "Ваш вибір: ";
        cin >> usertype;

        if (usertype == "1" || usertype == "продавець") {
            cout << "Ви успішно увійшли як Продавець.\n";

            while (true) {
                int action;
                cout << "\nЩо ви хочете зробити?\n";
                cout << "1. Додати товар\n";
                cout << "2. Закрити магазин\n";
                cout << "3. Створити акцію\n";
                cout << "4. Додати промокод\n";
                cout << "5. Додати бонус\n";
                cout << "6. Показати всі товари\n";
                cout << "7. Вийти\n";
                cout << "Ваш вибір: ";
                cin >> action;
                cin.ignore();

                if (action == 1) {
                    if (!storeOpen) {
                        cout << "Магазин закритий. Відкрийте магазин спочатку.\n";
                        continue;
                    }

                    Product newProduct;
                    cout << "Оберіть категорію товару (іграшка, одежа, взуття, речі для дітей до 2 років): ";
                    getline(cin, newProduct.category);

                    if (newProduct.category != "іграшка" && newProduct.category != "одежа" &&
                        newProduct.category != "взуття" && newProduct.category != "речі для дітей до 2 років") {
                        cout << "Невірний вибір категорії.\n";
                        continue;
                    }

                    cout << "Введіть назву товару: ";
                    getline(cin, newProduct.name);
                    cout << "Введіть ціну товару: ";
                    cin >> newProduct.price;
                    cout << "Введіть кількість товару: ";
                    cin >> newProduct.quantity;
                    cin.ignore();

                    store.push_back(newProduct);
                    cout << "Товар успішно доданий.\n";
                }
                else if (action == 2) {
                    storeOpen = false;
                    cout << "Магазин закритий.\n";
                }
                else if (action == 3) {
                    string promo;
                    cout << "Введіть опис акції: ";
                    getline(cin, promo);
                    for (auto& product : store) {
                        product.promotions.push_back(promo);
                    }
                    cout << "Акція додана.\n";
                }
                else if (action == 4) {
                    string code;
                    cout << "Введіть промокод: ";
                    getline(cin, code);
                    for (auto& product : store) {
                        product.promocodes.push_back(code);
                    }
                    cout << "Промокод додано.\n";
                }
                else if (action == 5) {
                    string bonus;
                    cout << "Введіть опис бонусу: ";
                    getline(cin, bonus);
                    for (auto& product : store) {
                        product.bonuses.push_back(bonus);
                    }
                    cout << "Бонус додано.\n";
                }
                else if (action == 6) {
                    if (store.empty()) {
                        cout << "Товарів поки немає.\n";
                    }
                    else {
                        cout << "Список товарів:\n";
                        for (size_t i = 0; i < store.size(); i++) {
                            cout << i + 1 << ". Назва: " << store[i].name << ", Категорія: " << store[i].category
                                << ", Ціна: " << store[i].price << ", Кількість: " << store[i].quantity << "\n";
                            cout << "Акції: ";
                            for (const auto& promo : store[i].promotions) cout << promo << " ";
                            cout << "\nПромокоди: ";
                            for (const auto& code : store[i].promocodes) cout << code << " ";
                            cout << "\nБонуси: ";
                            for (const auto& bonus : store[i].bonuses) cout << bonus << " ";
                            cout << "\n";
                        }
                    }
                }
                else if (action == 7) {
                    cout << "Вихід з акаунту Продавець.\n";
                    break;
                }
                else {
                    cout << "Невірний вибір, спробуйте ще раз.\n";
                }
            }
        }
        else if (usertype == "2" || usertype == "покупець") {
            cout << "Ви успішно увійшли як Покупець.\n";
            break;
        }
        else {
            cout << "Невірний вибір, спробуйте ще раз.\n";
        }
    }

    return 0;
}
