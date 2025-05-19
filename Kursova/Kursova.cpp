#include <iostream>
#include <string>
#include <vector>
#include <locale>
#include <fstream>
#include <iomanip>
#include <algorithm>

using namespace std;

struct Product {
    int id = 0;
    string name = "", category = "";
    double price = 0.0;
    int quantity = 0;
    vector<string> promotions, promocodes, bonuses;

    Product() = default;
    Product(int i, string n, string c, double p, int q) : id(i), name(n), category(c), price(p), quantity(q) {}

    Product(string n, double p, int disc, int bon) {
        name = n;
        price = p;
        if (disc > 100) disc = 100;
        if (disc < 0) disc = 0;
        promotions.push_back(to_string(disc) + "%");
        bonuses.push_back(to_string(bon));
    }

    string getName() const { return name; }
    double getPrice() const { return price; }

    string getDiscount() const {
        for (auto& s : promotions) {
            if (s.find('%') != string::npos) return s;
        }
        return "0%";
    }

    string getBonuses() const {
        if (!bonuses.empty()) return bonuses[0];
        return "0";
    }

    double getFinalPrice() const {
        double discount = 0;
        for (auto& s : promotions) {
            if (s.find('%') != string::npos) {
                discount = stod(s.substr(0, s.find('%')));
                break;
            }
        }
        return price * (1.0 - discount / 100);
    }
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
    cout << "╔════════════════════════════════╦════════════╦════════════╦══════════════╗\n";
    cout << "║ Назва товару                   ║ Кількість  ║ Ціна       ║ Сума         ║\n";
    cout << "╠════════════════════════════════╬════════════╬════════════╬══════════════╣\n";

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

    cout << "╠════════════════════════════════╩═════════════╩═════════════╬════════════╣\n";
    cout << "║ Загальна сума                                              ║ " << setw(10) << right << fixed << setprecision(2) << total << " ║\n";
    cout << "╚════════════════════════════════════════════════════════════╩════════════╝\n";
}

void displayProductTable(const Product& p) {
    cout << "╔════════════════════════════════════════════════════╗\n";
    cout << "║                  Інформація про товар              ║\n";
    cout << "╠════════════════════════════════════════════════════╣\n";
    cout << "║ Назва       : " << setw(40) << left << p.getName() << "║\n";
    cout << "║ Ціна        : " << setw(40) << left << fixed << setprecision(2) << p.getPrice() << "║\n";
    cout << "║ Знижка (%)  : " << setw(40) << left << p.getDiscount() << "║\n";
    cout << "║ Бонуси      : " << setw(40) << left << p.getBonuses() << "║\n";
    cout << "║ До сплати   : " << setw(40) << left << fixed << setprecision(2) << p.getFinalPrice() << "║\n";
    cout << "╚════════════════════════════════════════════════════╝\n";
}

void addProduct(vector<Product>& products) {
    string name, category;
    double price;
    int discount;
    int bonuses;
    int id = 1 + (products.empty() ? 0 : products.back().id);

    cout << "Введіть назву товару: ";
    cin.ignore();
    getline(cin, name);

    cout << "Введіть категорію товару: ";
    getline(cin, category);

    cout << "Введіть ціну товару: ";
    cin >> price;

    cout << "Введіть знижку (%): ";
    cin >> discount;

    cout << "Введіть кількість бонусів: ";
    cin >> bonuses;

    cin.ignore();
    string promoCode;
    cout << "Введіть промокод (або натисніть Enter): ";
    getline(cin, promoCode);

    for (int i = 0; i < promoCode.length(); i++) {
        promoCode[i] = toupper(promoCode[i]);
    }

    if (promoCode == "PROMO10") {
        discount += 10;
        cout << "Промокод застосовано. +10% знижки\n";
    }

    if (discount > 100) discount = 100;
    if (discount < 0) discount = 0;

    Product product(name, price, discount, bonuses);
    product.id = id;
    product.category = category;
    products.push_back(product);

    ofstream file("products.txt", ios::app);
    if (file.is_open()) {
        file << product.id << ";" << product.name << ";" << product.category << ";" << product.price << ";" << product.quantity << "\n";
        file.close();
    }
    else {
        cout << "Помилка відкриття файлу для запису\n";
    }

    cout << "\nТовар успішно додано!\n\n";
    displayProductTable(product);
}

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

void searchProduct(const vector<Product>& store) {
    cin.ignore();
    string keyword;
    cout << "Введіть назву або категорію для пошуку: ";
    getline(cin, keyword);

    bool found = false;
    cout << "\nРезультати пошуку:\n";

    for (const auto& p : store) {
        if (p.name.find(keyword) != string::npos || p.category.find(keyword) != string::npos) {
            cout << "ID: " << p.id << ", Назва: " << p.name << ", Категорія: " << p.category
                << ", Ціна: " << p.price << ", Кількість: " << p.quantity << "\n";
            found = true;
        }
    }

    if (!found) {
        cout << "Товарів за запитом не знайдено.\n";
    }
}

void sortAndPrintTable(vector<Product>& store) {
    cout << "╔══════════════════════════════════════════════╗\n";
    cout << "║  Виберіть поле для сортування:               ║\n";
    cout << "╠══════════════════════════════════════════════╣\n";
    cout << "║ 1. ID                                        ║\n";
    cout << "║ 2. Назва                                     ║\n";
    cout << "║ 3. Категорія                                 ║\n";
    cout << "║ 4. Ціна                                      ║\n";
    cout << "║ 5. Кількість                                 ║\n";
    cout << "╚══════════════════════════════════════════════╝\n";
    cout << "Ваш вибір: ";
    int choice;
    cin >> choice;

    switch (choice) {
    case 1:
        sort(store.begin(), store.end(), [](const Product& a, const Product& b) {
            return a.id < b.id;
            });
        break;
    case 2:
        sort(store.begin(), store.end(), [](const Product& a, const Product& b) {
            return a.name < b.name;
            });
        break;
    case 3:
        sort(store.begin(), store.end(), [](const Product& a, const Product& b) {
            return a.category < b.category;
            });
        break;
    case 4:
        sort(store.begin(), store.end(), [](const Product& a, const Product& b) {
            return a.price < b.price;
            });
        break;
    case 5:
        sort(store.begin(), store.end(), [](const Product& a, const Product& b) {
            return a.quantity < b.quantity;
            });
        break;
    default:
        cout << "Невірний вибір.\n";
        return;
    }

    printTable(store);
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
                cout << "║ 5. Додати акцію / промокод / бонус           ║\n";
                cout << "║ 6. Переглянути звіт про замовлення           ║\n";
                cout << "║ 7. Пошук товару                              ║\n";
                cout << "║ 8. Статистика по продажам                    ║\n";
                cout << "║ 9. Інформація про останній товар             ║\n";
                cout << "║10. Назад                                     ║\n";
                cout << "╚══════════════════════════════════════════════╝\n";
                cout << "Ваш вибір: ";
                int action;
                cin >> action;
                cin.ignore();

                if (action == 1) {
                    addProduct(store);
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
                            cout << "Нова категорія: "; getline(cin, p.category);
                            cout << "Нова ціна: "; cin >> p.price;
                            cout << "Нова кількість: "; cin >> p.quantity;
                            cin.ignore();
                            break;
                        }
                    }
                    save(store);
                }
                else if (action == 4) {
                    sortAndPrintTable(store);
                }
                else if (action == 5) {
                    printTable(store);
                    int id;
                    cout << "ID товару: "; cin >> id;
                    cin.ignore();

                    for (auto& p : store) {
                        if (p.id == id) {
                            int opt;
                            cout << "Що додати?\n1. Акція\n2. Промокод\n3. Бонус\nВаш вибір: ";
                            cin >> opt; cin.ignore();
                            string value;
                            cout << "Введіть значення: ";
                            getline(cin, value);

                            if (opt == 1) p.promotions.push_back(value);
                            else if (opt == 2) p.promocodes.push_back(value);
                            else if (opt == 3) p.bonuses.push_back(value);
                            else cout << "Невірний вибір.\n";
                            break;
                        }
                    }
                }
                else if (action == 6) {
                    for (const auto& order : orders) {
                        printCart(order.items, store);
                    }
                }
                else if (action == 7) {
                    searchProduct(store);
                }
                else if (action == 8) {
                    for (const auto& order : orders) {
                        printCart(order.items, store);
                    }
                    int totalUnitsSold = 0;
                    double totalRevenue = 0.0;

                    for (const auto& order : orders) {
                        for (const auto& item : order.items) {
                            totalUnitsSold += item.quantity;
                            for (const auto& p : store) {
                                if (p.id == item.productId) {
                                    totalRevenue += item.quantity * p.price;
                                }
                            }
                        }
                    }
                    cout << "╔════════════════════════════════════════════════════════════════════════╗\n";
                    cout << "║Загальна статистика продажів:                                           ║\n";
                    cout << "╠═════════════════════════════════════════════════╦══════════════════════╣\n";
                    cout << "║ Показник                                        ║ Значення             ║\n";
                    cout << "╠═════════════════════════════════════════════════╬══════════════════════╣\n";
                    cout << "║ Загальна кількість продажів                     ║ " << setw(20) << right << orders.size() << " ║\n";
                    cout << "║ Загальна кількість одиниць товарів продано      ║ " << setw(20) << right << totalUnitsSold << " ║\n";
                    cout << "║ Загальний дохід (грн)                           ║ " << setw(20) << right << fixed << setprecision(2) << totalRevenue << " ║\n";
                    cout << "╚═════════════════════════════════════════════════╩══════════════════════╝\n";
                }
                else if (action == 9) {
                    if (!store.empty()) {
                        displayProductTable(store.back());
                    }
                    else {
                        cout << "Немає доданих товарів.\n";
                    }
                }
                else if (action == 10) {
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
                cout << "║ 5. Пошук товару                              ║\n";
                cout << "║ 6. Назад                                     ║\n";
                cout << "╚══════════════════════════════════════════════╝\n";
                cout << "Ваш вибір: ";
                int action;
                cin >> action;

                if (action == 1) {
                    sortAndPrintTable(store);
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
                                p.quantity -= qty; 
                                cart.push_back({ id, qty });
                                save(store);  
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
                    searchProduct(store);
                }
                else if (action == 6) {
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