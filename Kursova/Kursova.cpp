#include <iostream>
#include <string>
#include <vector>
#include <locale>
#include <fstream>

using namespace std;

struct Product {
    int id;
    string name, category;
    double price;
    int quantity;
    vector<string> promotions, promocodes, bonuses;
};

void save(const vector<Product>& store) {
    ofstream file("products.txt");
    for (const auto& p : store) {
        file << p.id << " " << p.name << " " << p.category << " "
            << p.price << " " << p.quantity << "\n";
    }
    file.close();
}

void printTable(const vector<Product>& store) {
    cout << "\n========================Іграшковий магазин 'Капітошка' ========================\n";
    if (store.empty()) {
        cout << "Товарів поки немає.\n";
        return;
    }

    cout << "======================================================================\n";
    cout << "| ID | Назва          | Категорія      | Ціна   | Кількість |\n";
    cout << "----------------------------------------------------------------------\n";

    for (const auto& p : store) {
        printf("| %-3d | %-14s | %-14s | %-6.2f | %-9d |\n",
            p.id, p.name.c_str(), p.category.c_str(), p.price, p.quantity);
    }
    cout << "======================================================================\n";
}

void printMenu() {
    cout << "\n========================Іграшковий магазин 'Капітошка' ========================\n";
    cout << "\nЩо ви хочете зробити?\n"
        << "1. Додати товар\n"
        << "2. Видалити товар\n"
        << "3. Редагувати товар\n"
        << "4. Закрити магазин\n"
        << "5. Створити акцію\n"
        << "6. Додати промокод\n"
        << "7. Додати бонус\n"
        << "8. Показати всі товари\n"
        << "9. Вийти\n"
        << "Ваш вибір: ";
}

int main() {
    setlocale(LC_ALL, "uk_UA.UTF-8");

    vector<Product> store;
    bool storeOpen = true;

    while (true) {
        cout << "\n========================Іграшковий магазин 'Капітошка' ========================\n";
        cout << "Виберіть акаунт:\n1. Продавець\n2. Покупець\nВаш вибір: ";

        string usertype;
        cin >> usertype;
        cin.ignore();

        if (usertype == "1" || usertype == "продавець") {
            cout << "Ви увійшли як Продавець.\n";

            while (true) {
                int action;
                printMenu();
                cin >> action;
                cin.ignore();

                if (action == 1) {
                    if (!storeOpen) {
                        cout << "Магазин закритий.\n";
                        continue;
                    }

                    Product p;
                    cout << "ID товару: ";
                    cin >> p.id;
                    cin.ignore();

                    cout << "Категорія (іграшка, одежа, взуття, для дітей до 2 років): ";
                    getline(cin, p.category);

                    cout << "Назва: ";
                    getline(cin, p.name);

                    cout << "Ціна: ";
                    cin >> p.price;

                    cout << "Кількість: ";
                    cin >> p.quantity;
                    cin.ignore();

                    store.push_back(p);
                    cout << "Товар додано. ID: " << p.id << "\n";
                    save(store);
                }
                else if (action == 2) {
                    int id;
                    printTable(store);
                    cout << "ID для видалення: ";
                    cin >> id;
                    cin.ignore();

                    for (size_t i = 0; i < store.size(); i++) {
                        if (store[i].id == id) {
                            store.erase(store.begin() + i);
                            cout << "Товар видалено.\n";
                            save(store);
                            break;
                        }
                    }
                }
                else if (action == 3) {
                    int id;
                    printTable(store);
                    cout << "ID для редагування: ";
                    cin >> id;
                    cin.ignore();

                    for (auto& p : store) {
                        if (p.id == id) {
                            cout << "Назва: ";
                            getline(cin, p.name);

                            cout << "Ціна: ";
                            cin >> p.price;
                            cin.ignore();

                            cout << "Кількість: ";
                            cin >> p.quantity;
                            cin.ignore();

                            cout << "Оновлено.\n";
                            save(store);
                            break;
                        }
                    }
                }
                else if (action == 4) {
                    storeOpen = false;
                    cout << "Магазин закритий.\n";
                }
                else if (action == 5) {
                    string promo;
                    cout << "Введіть опис акції: ";
                    getline(cin, promo);
                    for (auto& p : store) {
                        p.promotions.push_back(promo);
                    }
                    cout << "Акція додана.\n";
                    save(store);
                }
                else if (action == 6) {
                    string code;
                    cout << "Введіть промокод: ";
                    getline(cin, code);
                    for (auto& p : store) {
                        p.promocodes.push_back(code);
                    }
                    cout << "Промокод додано.\n";
                    save(store);
                }
                else if (action == 7) {
                    string bonus;
                    cout << "Введіть опис бонусу: ";
                    getline(cin, bonus);
                    for (auto& p : store) {
                        p.bonuses.push_back(bonus);
                    }
                    cout << "Бонус додано.\n";
                    save(store);
                }
                else if (action == 8) {
                    printTable(store);
                }
                else if (action == 9) {
                    cout << "Вихід з акаунту.\n";
                    save(store);
                    break;
                }
                else {
                    cout << "Невірний вибір.\n";
                }
            }
        }
    }
    return 0;
}
