#include <iostream>
#include <string>
#include <vector>
#include <locale>
#include <fstream>
#include <iomanip>
#include <algorithm>
#include <limits>

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

float getValidatedDouble(const string& prompt) {
    double val;
    cout << prompt;
    while (!(cin >> val) || val < 0) {
        cout << "Некоректне значення. Спробуйте ще раз: ";
        cin.clear();
        cin.ignore(numeric_limits<streamsize>::max(), '\n');
    }
    return val;
}

int getValidatedInt(const string& prompt, bool allowZero = true) {
    int val;
    cout << prompt;
    while (!(cin >> val) || val < (allowZero ? 0 : 1)) {
        cout << "Некоректне значення. Спробуйте ще раз: ";
        cin.clear();
        cin.ignore(numeric_limits<streamsize>::max(), '\n');
    }
    return val;
}

string getNonEmptyString(const string& prompt) {
    string s;
    cout << prompt;
    getline(cin >> ws, s);
    while (s.empty()) {
        cout << "Поле не може бути порожнім. Спробуйте ще раз: ";
        getline(cin, s);
    }
    return s;
}

bool confirmAction(const string& prompt) {
    char answer;
    cout << prompt << " (Y/N): ";
    cin >> answer;
    answer = toupper(answer);
    while (answer != 'Y' && answer != 'N') {
        cout << "Будь ласка, введіть Y (так) або N (ні): ";
        cin >> answer;
        answer = toupper(answer);
    }
    cin.ignore(numeric_limits<streamsize>::max(), '\n');
    return answer == 'Y';
}

void warnIfLowStock(const Product& p) {
    if (p.quantity <= 5) {
        cout << "УВАГА! Товар \"" << p.name << "\" закінчується на складі. Залишилось " << p.quantity << " одиниць.\n";
    }
}

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
void printTable(const vector<Product>& store) {
    if (store.empty()) {
        cout << "Товарів поки немає.\n";
        return;
    }

    const int w_id = 4, w_name = 16, w_cat = 16, w_price = 10, w_qty = 10;

    cout << '+' << string(w_id, '-') << '+'
        << string(w_name, '-') << '+'
        << string(w_cat, '-') << '+'
        << string(w_price, '-') << '+'
        << string(w_qty, '-') << '+' << endl;

    cout << "|"
        << setw(w_id) << left << "ID"
        << "|"
        << setw(w_name) << left << "Назва"
        << "|"
        << setw(w_cat) << left << "Категорія"
        << "|"
        << setw(w_price) << left << "Ціна"
        << "|"
        << setw(w_qty) << left << "Кількість"
        << "|" << endl;

    cout << '+' << string(w_id, '-') << '+'
        << string(w_name, '-') << '+'
        << string(w_cat, '-') << '+'
        << string(w_price, '-') << '+'
        << string(w_qty, '-') << '+' << endl;

    for (const auto& p : store) {
        cout << "|"
            << setw(w_id) << left << p.id
            << "|"
            << setw(w_name) << left << p.name
            << "|"
            << setw(w_cat) << left << p.category
            << "|"
            << setw(w_price) << left << fixed << setprecision(2) << p.price
            << "|"
            << setw(w_qty) << left << p.quantity
            << "|" << endl;
    }

    cout << '+' << string(w_id, '-') << '+'
        << string(w_name, '-') << '+'
        << string(w_cat, '-') << '+'
        << string(w_price, '-') << '+'
        << string(w_qty, '-') << '+' << endl;
}
void printCart(const vector<CartItem>& cart, const vector<Product>& store) {
    double total = 0;
    const int w_name = 20, w_qty = 9, w_price = 7, w_sum = 10;
    cout << "\nВаш кошик:\n";
    cout << '+' << string(w_name, '-') << '+' << string(w_qty, '-') << '+'
        << string(w_price, '-') << '+' << string(w_sum, '-') << '+' << endl;
    cout << "|"
        << setw(w_name) << left << "Назва товару"
        << "|"
        << setw(w_qty) << left << "Кількість"
        << "|"
        << setw(w_price) << left << "Ціна"
        << "|"
        << setw(w_sum) << left << "Сума"
        << "|" << endl;
    cout << '+' << string(w_name, '-') << '+' << string(w_qty, '-') << '+'
        << string(w_price, '-') << '+' << string(w_sum, '-') << '+' << endl;
    for (const auto& item : cart) {
        for (const auto& p : store) {
            if (p.id == item.productId) {
                double subtotal = p.price * item.quantity;
                total += subtotal;
                cout << "|"
                    << setw(w_name) << left << p.name.substr(0, w_name - 1)
                    << "|"
                    << setw(w_qty) << right << item.quantity
                    << "|"
                    << setw(w_price) << right << fixed << setprecision(2) << p.price
                    << "|"
                    << setw(w_sum) << right << fixed << setprecision(2) << subtotal
                    << "|" << endl;
            }
        }
    }
    cout << '+' << string(w_name, '-') << '+' << string(w_qty, '-') << '+'
        << string(w_price, '-') << '+' << string(w_sum, '-') << '+' << endl;
    cout << "| Загальна сума:" << setw(w_name + w_qty + w_price + w_sum - 14) << right << fixed << setprecision(2) << total << " |\n";
    cout << '+' << string(w_name, '-') << '+' << string(w_qty, '-') << '+'
        << string(w_price, '-') << '+' << string(w_sum, '-') << '+' << endl;
}

void displayProductTable(const Product& p) {
    const int width = 38;
    cout << '+' << string(width, '=') << "+\n";
    cout << "|" << setw(width) << left << " Інформація про товар" << "|\n";
    cout << '+' << string(width, '-') << "+\n";
    cout << "| Назва      : " << setw(width - 14) << left << p.getName() << "|\n";
    cout << "| Ціна       : " << setw(width - 14) << left << fixed << setprecision(2) << p.getPrice() << "|\n";
    cout << "| Знижка (%) : " << setw(width - 14) << left << p.getDiscount() << "|\n";
    cout << "| Бонуси     : " << setw(width - 14) << left << p.getBonuses() << "|\n";
    cout << "| До сплати  : " << setw(width - 14) << left << fixed << setprecision(2) << p.getFinalPrice() << "|\n";
    cout << '+' << string(width, '=') << "+\n";
}
void showSellerInstructions() {
    cout << "\n=== Інструкція для продавця ===\n";
    cout << "Ваша роль — управляти асортиментом магазину, обробляти замовлення та стежити за продажами.\n";
    cout << "Доступні можливості:\n";
    cout << "  1. Додавати нові товари до магазину (назва, ціна, категорія, кількість, акції, бонуси).\n";
    cout << "  2. Редагувати інформацію про товари (змінювати ціну, кількість, опис, акції).\n";
    cout << "  3. Видаляти товари з асортименту (якщо товар більше не продається).\n";
    cout << "  4. Переглядати список усіх товарів магазину.\n";
    cout << "  5. Переглядати список оформлених замовлень від покупців.\n";
    cout << "  6. Оновлювати статус замовлень (наприклад, \"В обробці\", \"Відправлено\", \"Виконано\").\n";
    cout << "  7. Аналізувати продажі: найпопулярніші товари, статистика по категоріях, доходи.\n";
    cout << "  8. Шукати товари за назвою чи категорією для швидкого редагування.\n";
    cout << "  9. Застосовувати акції та бонусні пропозиції для залучення покупців.\n";
    cout << "Поради:\n";
    cout << "  - Регулярно оновлюйте наявність та ціни товарів.\n";
    cout << "  - Слідкуйте за статусами замовлень та своєчасно їх обробляйте.\n";
    cout << "  - Використовуйте акції та бонуси для стимулювання продажів.\n";
    cout << "  - Аналізуйте статистику для покращення асортименту та сервісу.\n";
    cout << "\nБажаємо успішних продажів та задоволених клієнтів!\n";
}

void showBuyerInstructions() {
    cout << "\n=== Інструкція для покупця ===\n";
    cout << "Ваша роль — обирати товари, додавати їх до кошика та здійснювати покупки.\n";
    cout << "Доступні можливості:\n";
    cout << "  1. Переглядати асортимент магазину (назва, ціна, категорія, наявність, акції, бонуси).\n";
    cout << "  2. Додавати вибрані товари у кошик (вказуйте потрібну кількість).\n";
    cout << "  3. Переглядати вміст кошика: список товарів, кількість, ціна, підсумкова сума.\n";
    cout << "  4. Видаляти товари з кошика або очищати кошик повністю.\n";
    cout << "  5. Оформлювати замовлення: підтверджуйте покупку, отримуйте повідомлення про успіх.\n";
    cout << "  6. Шукати товари за назвою чи категорією (для зручного пошуку).\n";
    cout << "  7. Сортувати товари за ціною, назвою, категорією.\n";
    cout << "  8. Переглядати найдешевший/найдорожчий товар магазину.\n";
    cout << "  9. Використовувати промокоди та бонуси, якщо вони доступні.\n";
    cout << "Поради:\n";
    cout << "  - Перед оформленням замовлення перевіряйте вміст та суму кошика.\n";
    cout << "  - Якщо товару немає у наявності — спробуйте вибрати інший або зачекайте поповнення.\n";
    cout << "  - Використовуйте пошук і сортування для швидкого знаходження потрібного товару.\n";
    cout << "  - Звертайте увагу на діючі акції та промокоди — це допоможе заощадити!\n";
    cout << "\nБажаємо приємних покупок та гарного настрою!\n";
}

void showInstructions() {
    cout << "--------------------------------------------------\n";
    cout << "ІНСТРУКЦІЯ КОРИСТУВАЧА\n";
    cout << "--------------------------------------------------\n";
    cout << "Ця програма — симуляція роботи магазину для двох ролей:\n";
    cout << "  1. Продавець\n";
    cout << "  2. Покупець\n";
    cout << "\n";
    cout << "Основні принципи роботи:\n";
    cout << "------------------------\n";
    cout << "Після запуску програми оберіть свою роль.\n";
    cout << "\n";
    cout << "=== Режим ПРОДАВЕЦЬ ===\n";
    cout << "  - Додавайте нові товари (назва, категорія, ціна, кількість, акції, бонуси, промокоди).\n";
    cout << "  - Видаляйте або редагуйте існуючі товари.\n";
    cout << "  - Переглядайте повний список товарів та їх деталі.\n";
    cout << "  - Додавайте акції, промокоди чи бонуси окремо для кожного товару.\n";
    cout << "  - Стежте за залишками на складі: якщо товару мало — програма попередить.\n";
    cout << "  - Використовуйте пошук та сортування для швидкого знаходження потрібного товару.\n";
    cout << "  - Аналізуйте статистику: кількість замовлень, загальна виручка, популярні позиції.\n";
    cout << "  - Не забувайте зберігати зміни у базі (файл products.txt).\n";
    cout << "  - Ви можете очищати всю базу або збільшувати кількість товару на складі.\n";
    cout << "\n";
    cout << "=== Режим ПОКУПЕЦЬ ===\n";
    cout << "  - Переглядайте асортимент магазину (з цінами, знижками, бонусами).\n";
    cout << "  - Додавайте вибрані товари у кошик із вказанням кількості.\n";
    cout << "  - Переглядайте та змінюйте вміст кошика: змінюйте кількість, видаляйте товари, очищайте кошик.\n";
    cout << "  - Оформлюйте замовлення одним натисканням (кошик автоматично очищається).\n";
    cout << "  - Слідкуйте за підрахунком суми до сплати, використовуйте промокоди та акції.\n";
    cout << "  - Після оформлення замовлення дані зберігаються для перегляду продавцем у статистиці.\n";
    cout << "  - Можна шукати товари за назвою чи категорією, дивитися найдешевший/найдорожчий товар.\n";
    cout << "\n";
    cout << "Додатково:\n";
    cout << "----------\n";
    cout << "  - Пункти меню містять підказки для кожної дії.\n";
    cout << "  - Програма автоматично попереджає про помилки вводу.\n";
    cout << "  - Можна зберегти вміст кошика в окремий файл для подальшого використання.\n";
    cout << "  - Дані про всі товари та замовлення зберігаються у файлах (products.txt, cart_full.txt).\n";
    cout << "\n";
    cout << "Поради:\n";
    cout << "-------\n";
    cout << "  - Завжди перевіряйте вміст кошика перед оформленням замовлення.\n";
    cout << "  - Продавцю рекомендується регулярно переглядати залишки на складі.\n";
    cout << "  - Якщо виникають питання по роботі — повертайтесь до цієї інструкції через меню.\n";
    cout << "\n";
    cout << "Бажаємо приємної роботи та гарних покупок!\n";
    cout << "--------------------------------------------------\n";
}

void printMinMaxProducts(const vector<Product>& store) {
    if (store.empty()) {
        cout << "Товарів немає.\n";
        return;
    }
    auto minmax = minmax_element(store.begin(), store.end(), [](const Product& a, const Product& b) {
        return a.price < b.price;
        });
    cout << "Найдешевший товар:\n";
    displayProductTable(*minmax.first);
    cout << "Найдорожчий товар:\n";
    displayProductTable(*minmax.second);
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

void printSalesStatistics(const vector<Order>& orders, const vector<Product>& store) {
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
    cout << "\n-------------------- Статистика продажів ---------------------\n";
    cout << "Загальна кількість продажів          : " << orders.size() << endl;
    cout << "Загальна кількість одиниць товарів   : " << totalUnitsSold << endl;
    cout << "Загальний дохід (грн)                : " << fixed << setprecision(2) << totalRevenue << endl;
    cout << "--------------------------------------------------------------\n";
}

void printSellerMenu() {
    cout << "╔══════════════════════════════════════════════╗\n";
    cout << "║            Меню Продавця                     ║\n";
    cout << "╠══════════════════════════════════════════════╣\n";
    cout << "║ 1. Інструкція                                ║\n";
    cout << "║ 2. Додати товар                              ║\n";
    cout << "║ 3. Видалити товар                            ║\n";
    cout << "║ 4. Редагувати товар                          ║\n";
    cout << "║ 5. Показати всі товари                       ║\n";
    cout << "║ 6. Додати акцію / промокод / бонус           ║\n";
    cout << "║ 7. Переглянути звіт про замовлення           ║\n";
    cout << "║ 8. Пошук товару                              ║\n";
    cout << "║ 9. Статистика по продажам                    ║\n";
    cout << "║10. Інформація про останній товар             ║\n";
    cout << "║11. Збільшити кількість товару                ║\n";
    cout << "║12. Очистити ВСІ товари з бази                ║\n";
    cout << "║13. Найдешевший/Найдорожчий товар             ║\n";
    cout << "║14. Назад                                     ║\n";
    cout << "╚══════════════════════════════════════════════╝\n";
    cout << "Ваш вибір: ";
}

void searchProduct(const vector<Product>& store) {
    string keyword;
    cout << "Введіть назву або категорію для пошуку: ";
    getline(cin >> ws, keyword); 

    bool found = false;
    cout << "\nРезультати пошуку:\n";
    for (const auto& p : store) {
        if (p.name.find(keyword) != string::npos || p.category.find(keyword) != string::npos) {
            cout << "ID: " << p.id
                << ", Назва: " << p.name
                << ", Категорія: " << p.category
                << ", Ціна: " << p.price
                << ", Кількість: " << p.quantity << "\n";
            found = true;
        }
    }
    if (!found) {
        cout << "Товарів за запитом не знайдено.\n";
    }
}

void printBuyerMenu() {
    cout << "╔══════════════════════════════════════════════╗\n";
    cout << "║              Меню Покупця                    ║\n";
    cout << "╠══════════════════════════════════════════════╣\n";
    cout << "║ 1. Інструкція                                ║\n";
    cout << "║ 2. Переглянути товари                        ║\n";
    cout << "║ 3. Додати товар до кошика                    ║\n";
    cout << "║ 4. Переглянути кошик                         ║\n";
    cout << "║ 5. Оформити замовлення                       ║\n";
    cout << "║ 6. Пошук товару                              ║\n";
    cout << "║ 7. Видалити товар із кошика                  ║\n";
    cout << "║ 8. Очистити весь кошик                       ║\n";
    cout << "║ 9. Найдешевший/Найдорожчий товар             ║\n";
    cout << "║10. Назад                                     ║\n";
    cout << "╚══════════════════════════════════════════════╝\n";
    cout << "Ваш вибір: ";
}

void addProduct(vector<Product>& products) {
    string name, category;
    double price;
    int discount;
    int bonuses;
    int id = 1 + (products.empty() ? 0 : products.back().id);

    cin.ignore(numeric_limits<streamsize>::max(), '\n');
    name = getNonEmptyString("Введіть назву товару: ");
    category = getNonEmptyString("Введіть категорію товару: ");
    price = getValidatedDouble("Введіть ціну товару: ");
    discount = getValidatedInt("Введіть знижку (%): ");
    bonuses = getValidatedInt("Введіть кількість бонусів: ");

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

    if (!confirmAction("Підтвердити додавання товару?")) {
        cout << "Додавання скасовано.\n";
        return;
    }

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
        cout << "║ 4. Інструкція користувача                    ║\n";
        cout << "╚══════════════════════════════════════════════╝\n";
        cout << "Ваш вибір: ";
        int role;
        cin >> role;

        if (role == 1) {
            while (true) {
                printSellerMenu();
                int action;
                cin >> action;
                cin.ignore();

                if (action == 1) {
                    showSellerInstructions();
                }
                else if (action == 2) {
                    addProduct(store);
                    save(store);
                }
                else if (action == 3) {
                    printTable(store);
                    int id = getValidatedInt("ID товару для видалення: ", false);
                    bool found = false;
                    for (size_t i = 0; i < store.size(); ++i) {
                        if (store[i].id == id) {
                            if (confirmAction("Ви точно хочете видалити цей товар?")) {
                                store.erase(store.begin() + i);
                                save(store);
                                cout << "Товар видалено.\n";
                            }
                            else {
                                cout << "Видалення скасовано.\n";
                            }
                            found = true;
                            break;
                        }
                    }
                    if (!found) cout << "Товар з таким ID не знайдений.\n";
                }
                else if (action == 4) {
                    printTable(store);
                    int id = getValidatedInt("ID товару для редагування: ", false);
                    bool found = false;
                    for (auto& p : store) {
                        if (p.id == id) {
                            if (confirmAction("Ви впевнені, що хочете редагувати цей товар?")) {
                                p.name = getNonEmptyString("Нова назва: ");
                                p.category = getNonEmptyString("Нова категорія: ");
                                p.price = getValidatedDouble("Нова ціна: ");
                                p.quantity = getValidatedInt("Нова кількість: ");
                                save(store);
                                cout << "Товар відредаговано.\n";
                                warnIfLowStock(p);
                            }
                            else {
                                cout << "Редагування скасовано.\n";
                            }
                            found = true;
                            break;
                        }
                    }
                    if (!found) cout << "Товар з таким ID не знайдений.\n";
                }
                else if (action == 5) {
                    sortAndPrintTable(store);
                }
                else if (action == 6) {
                    printTable(store);
                    int id = getValidatedInt("ID товару: ", false);
                    bool found = false;
                    for (auto& p : store) {
                        if (p.id == id) {
                            int opt = getValidatedInt("Що додати?\n1. Акція\n2. Промокод\n3. Бонус\nВаш вибір: ");
                            string value = getNonEmptyString("Введіть значення: ");
                            if (confirmAction("Підтвердити додавання?")) {
                                if (opt == 1) p.promotions.push_back(value);
                                else if (opt == 2) p.promocodes.push_back(value);
                                else if (opt == 3) p.bonuses.push_back(value);
                                else cout << "Невірний вибір.\n";
                                cout << "Додано.\n";
                            }
                            else {
                                cout << "Додавання скасовано.\n";
                            }
                            found = true;
                            break;
                        }
                    }
                    if (!found) cout << "Товар з таким ID не знайдений.\n";
                }
                else if (action == 7) {
                    for (const auto& order : orders) {
                        printCart(order.items, store);
                    }
                }
                else if (action == 8) {
                    searchProduct(store);
                }
                else if (action == 9) {
                    printSalesStatistics(orders, store);
                }
                else if (action == 10) {
                    if (!store.empty()) {
                        displayProductTable(store.back());
                    }
                    else {
                        cout << "Немає доданих товарів.\n";
                    }
                }
                else if (action == 11) {
                    printTable(store);
                    int id = getValidatedInt("ID товару для збільшення кількості: ", false);
                    bool found = false;
                    for (auto& p : store) {
                        if (p.id == id) {
                            int addQty = getValidatedInt("На скільки збільшити кількість: ");
                            if (confirmAction("Підтвердити збільшення кількості?")) {
                                p.quantity += addQty;
                                save(store);
                                cout << "Кількість товару збільшено. Нова кількість: " << p.quantity << endl;
                                warnIfLowStock(p);
                            }
                            else {
                                cout << "Дію скасовано.\n";
                            }
                            found = true;
                            break;
                        }
                    }
                    if (!found) cout << "Товар з таким ID не знайдений.\n";
                }
                else if (action == 12) {
                    if (confirmAction("УВАГА! Ви впевнені, що хочете видалити всі товари з бази?")) {
                        store.clear();
                        save(store);
                        cout << "Всі товари успішно видалено!\n";
                    }
                    else {
                        cout << "Дію скасовано.\n";
                    }
                }
                else if (action == 13) {
                    printMinMaxProducts(store);
                }
                else if (action == 14) {
                    if (confirmAction("Ви впевнені, що хочете повернутись до головного меню?")) {
                        break;
                    }
                }
            }
        }
        else if (role == 2) {
            while (true) {
                printBuyerMenu();
                int action;
                cin >> action;

                if (action == 1) {
                    showBuyerInstructions();
                }
                else if (action == 2) {
                    sortAndPrintTable(store);
                }
                else if (action == 3) {
                    printTable(store);
                    int id = getValidatedInt("ID товару: ", false);
                    int qty = getValidatedInt("Кількість: ", false);

                    bool productExists = false;
                    bool quantityAvailable = false;

                    for (auto& p : store) {
                        if (p.id == id) {
                            productExists = true;
                            if (p.quantity >= qty) {
                                quantityAvailable = true;
                                if (confirmAction("Підтвердити додавання до кошика?")) {
                                    p.quantity -= qty;
                                    cart.push_back({ id, qty });
                                    save(store);
                                    cout << "Товар успішно доданий до кошика!\n";
                                    warnIfLowStock(p);
                                }
                                else {
                                    cout << "Додавання скасовано.\n";
                                }
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
                }
                else if (action == 4) {
                    printCart(cart, store);
                }
                else if (action == 5) {
                    if (cart.empty()) {
                        cout << "Кошик порожній. Додайте товари перед оформленням замовлення.\n";
                        continue;
                    }
                    printCart(cart, store);
                    if (confirmAction("Ви впевнені, що хочете оформити замовлення?")) {
                        orders.push_back({ cart });
                        saveFullCartInfo(cart, store);
                        cart.clear();
                        cout << "Ваше замовлення оформлено! Дякуємо за покупку.\n";
                    }
                    else {
                        cout << "Оформлення замовлення скасовано.\n";
                    }
                }
                else if (action == 6) {
                    searchProduct(store);
                }
                else if (action == 7) {
                    if (cart.empty()) {
                        cout << "Кошик порожній.\n";
                        continue;
                    }
                    printCart(cart, store);
                    int id = getValidatedInt("Введіть ID товару, який бажаєте видалити з кошика: ", false);
                    bool found = false;
                    for (auto it = cart.begin(); it != cart.end(); ++it) {
                        if (it->productId == id) {
                            for (auto& p : store) {
                                if (p.id == id) {
                                    p.quantity += it->quantity;
                                    save(store);
                                    warnIfLowStock(p);
                                    break;
                                }
                            }
                            cart.erase(it);
                            cout << "Товар вилучено з кошика.\n";
                            found = true;
                            break;
                        }
                    }
                    if (!found) cout << "Товар з таким ID не знайдений у кошику.\n";
                }
                else if (action == 8) {
                    if (cart.empty()) {
                        cout << "Кошик і так порожній.\n";
                        continue;
                    }
                    if (confirmAction("Ви впевнені, що хочете повністю очистити кошик?")) {
                        for (const auto& item : cart) {
                            for (auto& p : store) {
                                if (p.id == item.productId) {
                                    p.quantity += item.quantity;
                                    warnIfLowStock(p);
                                }
                            }
                        }
                        cart.clear();
                        save(store);
                        cout << "Кошик очищено.\n";
                    }
                    else {
                        cout << "Очищення скасовано.\n";
                    }
                }
                else if (action == 9) {
                    printMinMaxProducts(store);
                }
                else if (action == 10) {
                    if (confirmAction("Ви впевнені, що хочете повернутись до головного меню?")) {
                        break;
                    }
                }
            }
        }
        else if (role == 3) {
            if (confirmAction("Ви впевнені, що хочете вийти з програми?")) {
                break;
            }
        }
        else if (role == 4) {
            showInstructions();
        }
        else {
            cout << "Невірний вибір. Спробуйте ще раз.\n";
        }
    }

    return 0;
}