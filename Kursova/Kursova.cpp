#include <iostream>
#include <string>
#include <vector>
#include <locale>
#include <fstream>
#include <iomanip>

using namespace std;

struct Product {
    int id = 0;
    string name = "", category = "";
    double price = 0.0;
    int quantity = 0;
    vector<string> promotions, promocodes, bonuses;
};

struct CartItem {
    int productId;
    int quantity;
};

struct Order {
    vector<CartItem> items;
};

vector<Order> orders;

void save(const vector<Product>& store) {
    ofstream file("products.txt");
    for (const auto& p : store) {
        file << p.id << ";" << p.name << ";" << p.category << ";" << p.price << ";" << p.quantity << "\n";
    }
    file.close();
}

vector<Product> load() {
    vector<Product> store;
    ifstream file("products.txt");
    string line;
    while (getline(file, line)) {
        Product p;
        size_t pos = 0;

        pos = line.find(';');
        p.id = stoi(line.substr(0, pos));
        line.erase(0, pos + 1);

        pos = line.find(';');
        p.name = line.substr(0, pos);
        line.erase(0, pos + 1);

        pos = line.find(';');
        p.category = line.substr(0, pos);
        line.erase(0, pos + 1);

        pos = line.find(';');
        p.price = stod(line.substr(0, pos));
        line.erase(0, pos + 1);

        p.quantity = stoi(line);

        store.push_back(p);
    }
    file.close();
    return store;
}

string centerText(const string& text, size_t width) {
    if (text.size() > width) return text;
    size_t padding = (width - text.size()) / 2;
    return string(padding, ' ') + text + string(width - text.size() - padding, ' ');
}

void printTable(const vector<Product>& store) {
    if (store.empty()) {
        cout << "Товарів поки немає.\n";
        return;
    }

    cout << "╔═════╦═════════════════════════╦═════════════════════════╦═════════════╦═════════════╗\n";
    cout << "║ ID  ║        Назва            ║       Категорія         ║    Ціна     ║  Кількість  ║\n";
    cout << "╠═════╬═════════════════════════╬═════════════════════════╬═════════════╬═════════════╣\n";

    for (const auto& p : store) {
        cout << "║ " << setw(5) << centerText(to_string(p.id), 5) << " ║ "
            << setw(25) << centerText(p.name, 25) << " ║ "
            << setw(25) << centerText(p.category, 25) << " ║ "
            << setw(11) << centerText(to_string(static_cast<int>(p.price)), 11) << " ║ "
            << setw(11) << centerText(to_string(p.quantity), 11) << " ║\n";
    }

    cout << "╚═════╩═════════════════════════╩═════════════════════════╩═════════════╩═════════════╝\n";
}

void printCart(const vector<CartItem>& cart, const vector<Product>& store) {
    double total = 0;
    cout << "Ваш кошик:\n";
    cout << "╔════════════════════════════════╦════════════╦════════════╦════════════╗\n";
    cout << "║ Назва товару                  ║ Кількість  ║ Ціна       ║ Сума       ║\n";
    cout << "╠════════════════════════════════╬════════════╬════════════╬════════════╣\n";

    for (const auto& item : cart) {
        for (const auto& p : store) {
            if (p.id == item.productId) {
                double subtotal = p.price * item.quantity;
                total += subtotal;
                cout << "║ " << setw(30) << left << p.name
                    << " ║ " << setw(10) << right << item.quantity
                    << " ║ " << setw(10) << right << fixed << setprecision(2) << p.price
                    << " ║ " << setw(10) << right << fixed << setprecision(2) << subtotal
                    << " ║\n";
            }
        }
    }

    cout << "╠════════════════════════════════╩════════════╩════════════╬════════════╣\n";
    cout << "║ Загальна сума                                            ║ " << setw(10) << right << fixed << setprecision(2) << total << " ║\n";
    cout << "╚═════════════════════════════════════════════════════════════════════╝\n";
}

// Функція для збереження повної інформації про кошик
void saveFullCartInfo(const vector<CartItem>& cart, const vector<Product>& store) {
    ofstream file("cart_full.txt");
    double total = 0;

    for (const auto& item : cart) {
        for (const auto& p : store) {
            if (p.id == item.productId) {
                double subtotal = p.price * item.quantity;
                total += subtotal;

                file << "Назва: " << p.name << "\n";
                file << "Категорія: " << p.category << "\n";
                file << "Ціна: " << fixed << setprecision(2) << p.price << "\n";
                file << "Кількість: " << item.quantity << "\n";
                file << "Сума: " << fixed << setprecision(2) << subtotal << "\n";

                file << "Акції: ";
                if (!p.promotions.empty()) {
                    for (const auto& promo : p.promotions)
                        file << promo << ", ";
                    file.seekp(-2, ios_base::cur);
                }
                else file << "немає";
                file << "\n";

                file << "Промокоди: ";
                if (!p.promocodes.empty()) {
                    for (const auto& code : p.promocodes)
                        file << code << ", ";
                    file.seekp(-2, ios_base::cur);
                }
                else file << "немає";
                file << "\n";

                file << "Бонуси: ";
                if (!p.bonuses.empty()) {
                    for (const auto& bonus : p.bonuses)
                        file << bonus << ", ";
                    file.seekp(-2, ios_base::cur);
                }
                else file << "немає";
                file << "\n";

                file << "---------------------------------------------\n";
            }
        }
    }

    file << "Загальна сума: " << fixed << setprecision(2) << total << "\n";
    file.close();
}

int main() {
    setlocale(LC_ALL, "uk_UA.UTF-8");
    vector<Product> store = load();
    vector<CartItem> cart;

    while (true) {
        cout << "╔══════════════════════════════════════════════╗\n";
        cout << "║       Оберіть роль:                          ║\n";
        cout << "╠══════════════════════════════════════════════╣\n";
        cout << "║ 1. Продавець                                 ║\n";
        cout << "║ 2. Покупець                                  ║\n";
        cout << "║ 3. Вихід                                     ║\n";
        cout << "╚══════════════════════════════════════════════╝\n";
        cout << "Ваш вибір: ";
        int role;
        cin >> role;

        if (role == 1) {
            while (true) {
                cout << "╔══════════════════════════════════════════════╗\n";
                cout << "║            Меню Продавця                     ║\n";
                cout << "╠══════════════════════════════════════════════╣\n";
                cout << "║ 1. Додати товар                              ║\n";
                cout << "║ 2. Видалити товар                            ║\n";
                cout << "║ 3. Редагувати товар                          ║\n";
                cout << "║ 4. Показати всі товари                       ║\n";
                cout << "║ 5. Переглянути звіт про замовлення           ║\n";
                cout << "║ 6. Назад                                     ║\n";
                cout << "╚══════════════════════════════════════════════╝\n";
                cout << "Ваш вибір: ";
                int action;
                cin >> action;
                cin.ignore();

                if (action == 1) {
                    Product p;
                    cout << "ID: "; cin >> p.id; cin.ignore();
                    cout << "Назва: "; getline(cin, p.name);
                    cout << "Категорія: "; getline(cin, p.category);
                    cout << "Ціна: "; cin >> p.price;
                    cout << "Кількість: "; cin >> p.quantity;
                    store.push_back(p);
                    save(store);
                }
                else if (action == 2) {
                    printTable(store);
                    int id;
                    cout << "ID товару для видалення: "; cin >> id;
                    for (size_t i = 0; i < store.size(); ++i) {
                        if (store[i].id == id) {
                            store.erase(store.begin() + i);
                            break;
                        }
                    }
                    save(store);
                }
                else if (action == 3) {
                    printTable(store);
                    int id;
                    cout << "ID товару для редагування: "; cin >> id; cin.ignore();
                    for (auto& p : store) {
                        if (p.id == id) {
                            cout << "Нова назва: "; getline(cin, p.name);
                            cout << "Нова ціна: "; cin >> p.price;
                            cout << "Нова кількість: "; cin >> p.quantity;
                            break;
                        }
                    }
                    save(store);
                }
                else if (action == 4) {
                    printTable(store);
                }
                else if (action == 5) {
                    for (const auto& order : orders) {
                        printCart(order.items, store);
                    }
                }
                else if (action == 6) {
                    break;
                }
            }
        }
        else if (role == 2) {
            while (true) {
                cout << "╔══════════════════════════════════════════════╗\n";
                cout << "║              Меню Покупця                    ║\n";
                cout << "╠══════════════════════════════════════════════╣\n";
                cout << "║ 1. Переглянути товари                        ║\n";
                cout << "║ 2. Додати товар до кошика                    ║\n";
                cout << "║ 3. Переглянути кошик                         ║\n";
                cout << "║ 4. Оформити замовлення                       ║\n";
                cout << "║ 5. Назад                                     ║\n";
                cout << "╚══════════════════════════════════════════════╝\n";
                cout << "Ваш вибір: ";
                int action;
                cin >> action;

                if (action == 1) {
                    printTable(store);
                }
                else if (action == 2) {
                    printTable(store);
                    int id, qty;
                    cout << "ID товару: "; cin >> id;
                    cout << "Кількість: "; cin >> qty;

                    bool productExists = false;
                    bool quantityAvailable = false;

                    for (auto& p : store) {
                        if (p.id == id) {
                            productExists = true;
                            if (p.quantity >= qty) {
                                quantityAvailable = true;
                                p.quantity -= qty;  // Зменшуємо кількість товару в магазині
                                cart.push_back({ id, qty });
                                save(store);  // Збереження змін у магазині
                                break;
                            }
                            else {
                                cout << "На складі недостатньо товару. Залишилось лише " << p.quantity << " одиниць.\n";
                                break;
                            }
                        }
                    }

                    if (!productExists) {
                        cout << "Товар з таким ID не знайдений.\n";
                    }
                    if (quantityAvailable) {
                        cout << "Товар успішно доданий до кошика!\n";
                    }
                }
                else if (action == 3) {
                    printCart(cart, store);
                }
                else if (action == 4) {
                    orders.push_back({ cart });
                    saveFullCartInfo(cart, store);
                    cart.clear();
                    cout << "Ваше замовлення оформлено! Дякуємо за покупку.\n";
                }
                else if (action == 5) {
                    break;
                }
            }
        }
        else if (role == 3) {
            break;
        }
    }

    return 0;
}
