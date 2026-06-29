#include <iostream>
#include <string>
#include <vector>
#include <limits>
#include <sstream>
using namespace std;

class Base {
    int id;
public:
    Base() : id(0) {}
    explicit Base(int i) : id(i) {}
    Base(const Base& other) : id(other.id) {}
    virtual ~Base() {}

    int getId() const { return id; }
    void setId(int v) { id = v; }
    virtual void display() const = 0;  // ПОЗДНЕЕ связывание
};
class Person {
    string name;
public:
    Person() {}
    explicit Person(const string& n) : name(n) {}
    Person(const Person& p) : name(p.name) {}
    virtual ~Person() {}
    const string& getName() const { return name; }
};
class User : public Base, virtual public Person { // Виртуальное наследование 
    string login, password;
public:
    User() : Base(), Person(), login(""), password("") {}
    User(int id, const string& lg, const string& nm, const string& pw)
        : Base(id), Person(nm), login(lg), password(pw) {
    }
    User(const User& u) : Base(u), Person(u), login(u.login), password(u.password) {}
    virtual ~User() {}

    const string& getLogin() const { return login; }
    const string& getPassword() const { return password; }
    void setLogin(const string& lg) { login = lg; }
    void setPassword(const string& pw) { password = pw; }

    bool auth(const string& lg, const string& pw) const { return lg == login && pw == password; }// Зависимость от stringы
    virtual string getRole() const = 0; // Еще одна абстрактная функция
    void display() const { cout << "ID:" << getId() << " | " << getName(); }
};
class Admin : public User {
    int accessLevel;
public:
    Admin() : User(), accessLevel(1) {}
    Admin(int id, const string& lg, const string& nm, const string& pw, int access = 1)
        : User(id, lg, nm, pw), accessLevel(access) {
    }
    Admin(const Admin& other) : User(other), accessLevel(other.accessLevel) {}
    ~Admin() {}

    string getRole() const { return "Admin"; }
    void display() const {
        User::display();
        cout << " | Роль: Админ | Уровень доступа: " << accessLevel << "\n";
    }
    int getAccessLevel() const { return accessLevel; }
    void setAccessLevel(int level) { accessLevel = level; }
};
// Вспомогательные функции ввода - чтобы не дублировать код проверок
string inputLine(const string& prompt) {
    cout << prompt;
    string s;
    getline(cin, s);
    while (s.empty()) {
        cout << "Ошибка! Ввод не может быть пустым. " << prompt;
        getline(cin, s);
    }
    return s;
}
int inputInt(const string& prompt, int lo, int hi) {
    int v;
    while (true) {
        cout << prompt;
        cin >> v;
        if (cin.fail() || v < lo || v > hi) {
            cout << "Ошибка! Введите число от " << lo << " до " << hi << "\n";
            cin.clear();
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
        }
        else {
            cin.ignore();
            return v;
        }
    }
}
// Проверка даты
bool isValidDate(int day, int month, int year) {
    if (year < 2025 || year > 2030) return false;
    if (month < 1 || month > 12) return false;
    if (day < 1) return false;
    int daysInMonth;
    switch (month) {
    case 1: case 3: case 5: case 7: case 8: case 10: case 12: daysInMonth = 31; break;
    case 4: case 6: case 9: case 11: daysInMonth = 30; break;
    case 2: daysInMonth = ((year % 4 == 0 && year % 100 != 0) || (year % 400 == 0)) ? 29 : 28; break;//високосный год 
    default: return false;
    }
    return day <= daysInMonth;
}
string inputDate(const string& prompt) {
    while (true) {
        string date = inputLine(prompt);
        if (date.length() != 10 || date[2] != '.' || date[5] != '.') {
            cout << "Ошибка! Используйте формат ДД.ММ.ГГГГ\n";
            continue;
        }
        for (int i = 0; i < 10; i++) {
            if (i != 2 && i != 5 && !isdigit(date[i])) { //isdigit(date[i]) - функция проверяет, является ли этот символ цифрой 
                cout << "Ошибка! Дата должна содержать только цифры и точки\n";
                continue;
            }
        }
        int day, month, year;
        try {
            day = stoi(date.substr(0, 2)); //Берет подстроку из date начиная с позиции 0, длиной 2 символа stoi = string to integer
            month = stoi(date.substr(3, 2));
            year = stoi(date.substr(6, 4));
        }
        catch (...) {
            cout << "Ошибка преобразования даты!\n";
            continue;
        }
        if (isValidDate(day, month, year)) return date;
        cout << "Ошибка! Некорректная дата. Год должен быть от 2025 до 2030\n";
    }
}
//на будующее
/*string inputTime(const string& prompt) {
    while (true) {
        string time = inputLine(prompt);
        if (time.length() != 5 || time[2] != ':') {
            cout << "Ошибка! Используйте формат ЧЧ:ММ\n";
            continue;
        }
        for (int i = 0; i < 5; i++) {
            if (i != 2 && !isdigit(time[i])) {
                cout << "Ошибка! Время должно содержать только цифры и двоеточие\n";
                continue;
            }
        }
        int hours = stoi(time.substr(0, 2));
        int minutes = stoi(time.substr(3, 2));
        if (hours >= 0 && hours <= 23 && minutes >= 0 && minutes <= 59) return time;
        cout << "Ошибка! Часы: 00-23, Минуты: 00-59\n";
    }
}
*/
// Перечисление направлений для удобства 
enum TrainingDirection { WEIGHT_LOSS = 1, MUSCLE_GAIN, YOGA, CARDIO };
string dirToStr(TrainingDirection d) {
    switch (d) {
    case WEIGHT_LOSS: return "Похудение";
    case MUSCLE_GAIN: return "Набор массы";
    case YOGA: return "Йога";
    case CARDIO: return "Кардио";
    default: return "Неизвестно";
    }
}
void showDirections() {
    cout << "1.Похудение 2.Набор массы 3.Йога 4.Кардио\n";
}
class Schedule {
    vector<string> hours;
    vector<string> busy;
public:
    Schedule() {
        // Стандартное рабочее время тренера
        hours.push_back("09:00");
        hours.push_back("10:00");
        hours.push_back("11:00");
        hours.push_back("12:00");
        hours.push_back("14:00");
        hours.push_back("15:00");
        hours.push_back("16:00");
        hours.push_back("17:00");
    }
    // Перегрузка операторов
    friend ostream& operator<<(ostream& os, const Schedule& s) {
        return os << "Занято слотов: " << s.busy.size();
    }
    friend istream& operator>>(istream& is, Schedule& s) {
        size_t n; if (is >> n) s.busy.clear(); return is;
    }
    bool book(const string& date, const string& time) { //свободен ли данное время для бронирования РАННЕЕ связывание
        string key = date + " " + time;
        for (size_t i = 0; i < busy.size(); i++)
            if (busy[i] == key) return false;
        busy.push_back(key); return true;
    }
    bool cancel(const string& date, const string& time) {//отменяет бронирование временного слота - удаляет его из списка занятых
        string key = date + " " + time;
        for (size_t i = 0; i < busy.size(); i++)
            if (busy[i] == key) { busy.erase(busy.begin() + i); return true; }
        return false;
    }
    vector<string> freeAt(const string& date) const {//возвращает список свободных временных слотов на указанную дату  Зависимость от vector<string>
        vector<string> free;
        for (size_t i = 0; i < hours.size(); i++) {
            string key = date + " " + hours[i];
            bool used = false;
            for (size_t j = 0; j < busy.size(); j++)
                if (busy[j] == key) { used = true; break; }
            if (!used) free.push_back(hours[i]);
        }
        return free;
    }
    int busyCount() const { return (int)busy.size(); }
    // Префиксные инкременты и дикрименты
    Schedule& operator++() {
        if (busy.size() < hours.size()) busy.push_back("AUTO 00:00"); return *this;
    }
    Schedule& operator--() {
        if (!busy.empty()) busy.pop_back(); return *this;
    }
    ~Schedule() {}
};
class Order : public Base {
public:
    enum Status { PENDING, CONFIRMED, COMPLETED, CLOSED, CANCELLED };
private:
    Status status;
    int clientId, trainerId;
    string dateTime;
    TrainingDirection direction; //Направление тренировки — это неотъемлемая часть заказа, определяющая его сущность
    double cost;
    int rating;
public:
    Order() : Base(), status(PENDING), clientId(0), trainerId(0), direction(WEIGHT_LOSS), cost(0.0), rating(0) {}
    Order(int id, int cId, int tId, const string& dt, TrainingDirection dir, double c)
        : Base(id), status(PENDING), clientId(cId), trainerId(tId), dateTime(dt), direction(dir), cost(c), rating(0) {
    }
    Order(const Order& o) : Base(o), status(o.status), clientId(o.clientId), trainerId(o.trainerId),
        dateTime(o.dateTime), direction(o.direction), cost(o.cost), rating(o.rating) {
    }
    Order(Order&& o) noexcept
        : Base(std::move(o)), status(o.status), clientId(o.clientId), trainerId(o.trainerId),
        dateTime(std::move(o.dateTime)), direction(o.direction), cost(o.cost), rating(o.rating) {
        o.status = CANCELLED;
        o.clientId = 0;
        o.trainerId = 0;
        o.direction = WEIGHT_LOSS;
        o.cost = 0.0;
        o.rating = 0;
    }
    ~Order() {}
    Status getStatus() const { return status; }
    int getClientId() const { return clientId; }
    int getTrainerId() const { return trainerId; }
    const string& getDateTime() const { return dateTime; }
    TrainingDirection getDirection() const { return direction; }
    double getCost() const { return cost; }
    int getRating() const { return rating; }
    void setCost(double c) { cost = c; }
    void changeStatus(Status s) { status = s; }
    // Добавление оценуи 
    bool addReview(int r) {
        if (r < 1 || r > 5) return false;
        rating = r; changeStatus(CLOSED); return true;
    }
    void markCompleted() { if (status == CONFIRMED) status = COMPLETED; }
    bool canAddReview() const { return status == COMPLETED && rating == 0; }

    void display() const {
        cout << "Заказ#" << getId() << " | " << dateTime << " | " << dirToStr(direction)
            << " | " << cost << " руб. | статус=";
        switch (status) {
        case PENDING: cout << "ожид."; break; case CONFIRMED: cout << "подтв."; break;
        case COMPLETED: cout << "провед."; break; case CLOSED: cout << "закрыт"; break;
        case CANCELLED: cout << "отменен"; break;
        }
        if (rating) cout << " | рейтинг=" << rating;
        cout << "\n";
    }
    void displayDetailed() const { display(); }
    friend class AuditOrder; // Дружественный класс 
    // Еще перегрузки 
    /*
    friend ostream& operator<<(ostream& os, const Order& o){
        return os << "Order#" << o.getId() << " [" << o.dateTime << "] cost=" << o.cost;
    }
    friend double operator+(const Order& a, const Order& b){ return a.cost + b.cost; }
    double operator*(double k) const { return cost * k; }
    */
};
//Демонстрации friend-класса (имеет доступ к приватным данным Order) для демонстрации 
class AuditOrder {
public:
    static void print(const Order& o) { cout << "[AUDIT] id=" << o.getId() << " cost=" << o.getCost() << "\n"; }
};
class Trainer : public User {
    Schedule schedule;// КОМПОЗИЦИЯ - Schedule не существует без Trainer
    double rating;
    vector<TrainingDirection> specs;
    double baseCost;
    int totalRatings, ratingsCount;
public:
    Trainer() : User(), rating(0), baseCost(1500), totalRatings(0), ratingsCount(0) {}
    Trainer(int id, const string& lg, const string& name, const string& pw, double r = 0, double cost = 1500)
        : User(id, lg, name, pw), rating(r), baseCost(cost), totalRatings(0), ratingsCount(0) {
        // Если передан начальный рейтинг, инициализируем историю оценок
        if (r > 0) { totalRatings = (int)(r * 10); ratingsCount = 10; }
    }
    Trainer(const Trainer& t) : User(t), schedule(t.schedule), rating(t.rating), specs(t.specs),
        baseCost(t.baseCost), totalRatings(t.totalRatings), ratingsCount(t.ratingsCount) {
    }
    ~Trainer() {}

    string getRole() const { return "Trainer"; }
    void display() const {
        User::display();
        cout << " | Роль: Тренер | Рейтинг: " << rating << " | Оценок: " << ratingsCount
            << " | Базовая цена: " << baseCost << "\n";
        if (!specs.empty()) {
            cout << "   Специализации: ";
            for (size_t i = 0; i < specs.size(); i++) {
                cout << dirToStr(specs[i]);
                if (i + 1 < specs.size()) cout << ", ";
            }
            cout << "\n";
        }
    }
    vector<string> getAvailableSlots(const string& date) const { return schedule.freeAt(date); }//свободные временные слоты тренера на указанную дату
    bool book(const string& date, const string& time) { return schedule.book(date, time); }//Забронировать временной слот
    bool cancelBooking(const string& date, const string& time) { return schedule.cancel(date, time); }//Отменить бронирование
    int busyCount() const { return schedule.busyCount(); }
    bool hasDir(TrainingDirection d) const { // есть ли у тренера специализация в указанном направлении
        for (size_t i = 0; i < specs.size(); i++) if (specs[i] == d) return true; return false;
    }
    bool addSpecialization(TrainingDirection d) {
        if (!hasDir(d)) { specs.push_back(d); return true; } return false;
    }
    double priceFor(TrainingDirection) const { return baseCost; }
    // ПЕРЕГРУЗКА ОПЕРАТОРА + ЧЕРЕЗ ОБЫЧНУЮ ФУНКЦИЮ-ЧЛЕН
    double operator+(const Trainer& other) const {
        return this->baseCost + other.baseCost;
    }
    // Также можно добавить перегрузку для сложения с числом
    double operator+(double value) const {
        return this->baseCost + value;
    }
    // Обновление рейтинга
    void addRating(int newRating) {
        totalRatings += newRating; ratingsCount++;
        rating = (double)totalRatings / ratingsCount;
        cout << "Добавлена оценка: " << newRating << ". Новый рейтинг: " << rating
            << " (всего оценок: " << ratingsCount << ")\n";
    }
    double getRating() const { return rating; }
    int getRatingsCount() const { return ratingsCount; }
    int getTotalRatings() const { return totalRatings; }
};
class Client : public User {
    vector<int> history; // АГРЕГАЦИЯ - ID заказов могут существовать без Client
public:
    Client() : User() {}
    Client(int id, const string& lg, const string& name, const string& pw)
        : User(id, lg, name, pw) {
    }
    Client(const Client& c) : User(c), history(c.history) {}
    ~Client() {}

    string getRole() const { return "Client"; }
    void addOrderId(int oid) { history.push_back(oid); }
    int getHistoryCount() const { return (int)history.size(); }//Возвращает количество заказов в истории клиента
    int getOrderFromHistory(int idx) const { return history.at((size_t)idx); }
    void display() const {
        User::display();
        cout << " | Роль: Клиент | Заказов: " << history.size() << "\n";
    }
};
// Главный класс системы 
class FitnessManager {
private:
    vector<Client*> clients;// АССОЦИАЦИЯ - Client существуют независимо
    vector<Trainer*> trainers;
    vector<Admin*> admins;
    vector<Order*> orders;
    User* currentUser;
    int nextId;
    int genId() { return nextId++; }
    Trainer* getTrainerById(int id) const { //Найти потльзоваьтеля в системе по его ID
        for (size_t i = 0; i < trainers.size(); i++)
            if (trainers[i]->getId() == id) return trainers[i];
        return NULL;
    }
    Client* getClientById(int id) const {
        for (size_t i = 0; i < clients.size(); i++)
            if (clients[i]->getId() == id) return clients[i];
        return NULL;
    }
    Order* getOrderById(int id) const {
        for (size_t i = 0; i < orders.size(); i++)
            if (orders[i]->getId() == id) return orders[i];
        return NULL;
    }
    void updateTrainerRating(int trainerId, int newRating) {
        Trainer* t = getTrainerById(trainerId);
        if (t) t->addRating(newRating);
    }
    bool isLoginTaken(const string& login) const { //занят ли логин в системе
        for (size_t i = 0; i < clients.size(); i++)
            if (clients[i]->getLogin() == login) return true;
        for (size_t i = 0; i < trainers.size(); i++)
            if (trainers[i]->getLogin() == login) return true;
        for (size_t i = 0; i < admins.size(); i++)
            if (admins[i]->getLogin() == login) return true;
        return false;
    }
    pair<string, string> splitDateTime(const string& dt) const {//Преобразует строку формата "ДД.ММ.ГГГГ ЧЧ:ММ" в пару (дата, время)
        size_t sp = dt.find(' ');
        if (sp == string::npos) {
            return pair<string, string>(dt, string());
        }
        else {
            return pair<string, string>(dt.substr(0, sp), dt.substr(sp + 1));
        }
    }
public:
    FitnessManager() : currentUser(NULL), nextId(1) {
        // Инициализация тестовыми данными - для демонстрации
        Trainer* t1 = new Trainer(genId(), "t1", "Иван Петров", "1111", 4.8, 1500);
        t1->addSpecialization(WEIGHT_LOSS); t1->addSpecialization(CARDIO);

        Trainer* t2 = new Trainer(genId(), "t2", "Анна Сидорова", "2222", 4.9, 2000);
        t2->addSpecialization(MUSCLE_GAIN); t2->addSpecialization(YOGA);

        Trainer* t3 = new Trainer(genId(), "t3", "Петр Иванов", "3333", 4.7, 1200);
        t3->addSpecialization(YOGA); t3->addSpecialization(CARDIO);

        trainers.push_back(t1);
        trainers.push_back(t2);
        trainers.push_back(t3);

        Client* c1 = new Client(genId(), "c1", "Сергей Иванов", "0001");
        Client* c2 = new Client(genId(), "c2", "Мария Смирнова", "0002");
        clients.push_back(c1);
        clients.push_back(c2);

        Admin* a1 = new Admin(genId(), "a1", "Админ Админов", "1234", 3);
        admins.push_back(a1);

        // Создаем тестовые заказы
        int id1 = genId();
        Order* o1 = new Order(id1, c1->getId(), t1->getId(), "01.11.2025 10:00", WEIGHT_LOSS, 1500);
        orders.push_back(o1); c1->addOrderId(id1);

        int id2 = genId();
        Order* o2 = new Order(id2, c2->getId(), t2->getId(), "02.11.2025 11:00", YOGA, 2000);
        orders.push_back(o2); c2->addOrderId(id2);
    }
    ~FitnessManager() {
        // Не забываем очистить память!
        for (size_t i = 0; i < clients.size(); i++) delete clients[i];
        for (size_t i = 0; i < trainers.size(); i++) delete trainers[i];
        for (size_t i = 0; i < admins.size(); i++) delete admins[i];
        for (size_t i = 0; i < orders.size(); i++) delete orders[i];
    }
    bool login(const string& lg, const string& pw) {
        for (size_t i = 0; i < clients.size(); i++)
            if (clients[i]->auth(lg, pw)) { currentUser = clients[i]; return true; }
        for (size_t i = 0; i < trainers.size(); i++)
            if (trainers[i]->auth(lg, pw)) { currentUser = trainers[i]; return true; }
        for (size_t i = 0; i < admins.size(); i++)
            if (admins[i]->auth(lg, pw)) { currentUser = admins[i]; return true; }
        return false;
    }
    void registerClient() {
        cout << "\n=== Регистрация нового клиента ===\n";
        string lg, pw, nm;
        while (true) {
            lg = inputLine("Логин: ");
            if (isLoginTaken(lg)) cout << "Ошибка! Этот логин уже занят.\n";
            else break;
        }
        while (true) {
            pw = inputLine("Пароль: ");
            if (pw.length() < 4) cout << "Ошибка! Пароль должен содержать минимум 4 символа.\n";
            else break;
        }
        nm = inputLine("ФИО: ");
        clients.push_back(new Client(genId(), lg, nm, pw));
        cout << "Регистрация выполнена!\n";
    }

    void showAllTrainers() {
        cout << "\n=== Список тренеров ===\n";
        if (trainers.empty()) { cout << "Тренеры не найдены.\n"; return; }
        for (size_t i = 0; i < trainers.size(); i++) {
            cout << i + 1 << ". "; trainers[i]->display();
        }
    }
    // Возвращает список тренеров по направлению
    vector<Trainer*> getTrainersByDir(TrainingDirection d) {
        vector<Trainer*> result;
        for (size_t i = 0; i < trainers.size(); i++) {
            if (trainers[i]->hasDir(d)) {
                result.push_back(trainers[i]);
            }
        }
        return result;
    }
    void showTrainersByDir(TrainingDirection d) {
        cout << "\n=== Тренеры по направлению: " << dirToStr(d) << " ===\n";
        vector<Trainer*> availableTrainers = getTrainersByDir(d);
        if (availableTrainers.empty()) {
            cout << "Нет тренеров с этим направлением.\n";
            return;
        }
        for (size_t i = 0; i < availableTrainers.size(); i++) {
            cout << i + 1 << ". "; availableTrainers[i]->display();
        }
    }
    // Основная логика бронирования 
    bool bookTraining(int clientId, Trainer* trainer, const string& date, const string& time, TrainingDirection d) {
        if (!trainer->book(date, time)) return false;
        string dt = date + " " + time;
        int oid = genId();
        orders.push_back(new Order(oid, clientId, trainer->getId(), dt, d, trainer->priceFor(d)));
        Client* c = getClientById(clientId);
        if (c) c->addOrderId(oid);
        cout << "Бронь создана: " << dt << "\n";
        return true;
    }
    void showClientMenu() {
        Client* client = dynamic_cast<Client*>(currentUser);
        if (!client) return;

        while (true) {
            cout << "\n=== МЕНЮ КЛИЕНТА ===\n";
            cout << "1. Записаться на тренировку\n2. Посмотреть мои тренировки\n";
            cout << "3. Оставить оценку\n4. Показать всех тренеров\n5. Выйти из аккаунта\n";

            int choice = inputInt("Выберите действие: ", 1, 5);
            if (choice == 1) {
                cout << "\n=== Запись на тренировку ===\n";
                showDirections();
                TrainingDirection dir = (TrainingDirection)inputInt("Выберите направление (1-4): ", 1, 4);

                // Получаем доступных тренеров по направлению
                vector<Trainer*> availableTrainers = getTrainersByDir(dir);
                if (availableTrainers.empty()) {
                    cout << "Нет доступных тренеров по этому направлению.\n";
                    continue;
                }
                // Показываем только доступных тренеров
                cout << "\n=== Доступные тренеры ===\n";
                for (size_t i = 0; i < availableTrainers.size(); i++) {
                    cout << i + 1 << ". "; availableTrainers[i]->display();
                }
                // Выбор тренера из доступных
                int trainerChoice = inputInt("Тренер (номер): ", 1, (int)availableTrainers.size());
                Trainer* selectedTrainer = availableTrainers[trainerChoice - 1];
                string date = inputDate("Дата (ДД.ММ.ГГГГ): ");
                vector<string> slots = selectedTrainer->getAvailableSlots(date);
                if (slots.empty()) {
                    cout << "На эту дату свободных слотов нет.\n";
                    continue;
                }
                cout << "Доступное время: ";
                for (size_t i = 0; i < slots.size(); i++)
                    cout << i + 1 << "." << slots[i] << " ";
                cout << "\n";
                int timeChoice = inputInt("Выберите время (номер): ", 1, (int)slots.size());
                bool ok = bookTraining(client->getId(), selectedTrainer, date, slots[timeChoice - 1], dir);
                cout << (ok ? "Тренировка забронирована.\n" : "Не удалось забронировать.\n");
            }
            else if (choice == 2) {
                cout << "\n=== Ваши тренировки ===\n";
                int hc = client->getHistoryCount();
                if (hc <= 0) { cout << "Записей нет.\n"; }
                else for (int i = 0; i < hc; i++) {
                    int oid = client->getOrderFromHistory(i);
                    Order* order = getOrderById(oid);
                    if (order) order->display();
                    else cout << "Заказ #" << oid << " не найден.\n";
                }
            }
            else if (choice == 3) {
                cout << "\n=== Оставить оценку ===\n";
                vector<Order*> completedOrders;
                for (int i = 0; i < client->getHistoryCount(); i++) {
                    int oid = client->getOrderFromHistory(i);
                    Order* order = getOrderById(oid);
                    if (order && order->canAddReview()) completedOrders.push_back(order);
                }
                if (completedOrders.empty()) { cout << "Нет завершенных тренировок для оценки.\n"; continue; }
                cout << "Завершенные тренировки для оценки:\n";
                for (size_t i = 0; i < completedOrders.size(); i++) {
                    cout << i + 1 << ". "; completedOrders[i]->display();
                }
                int orderChoice = inputInt("Выберите тренировку для оценки: ", 1, (int)completedOrders.size());
                Order* selectedOrder = completedOrders[orderChoice - 1];
                int rating = inputInt("Оцените тренировку (1-5): ", 1, 5);
                if (selectedOrder->addReview(rating)) {
                    updateTrainerRating(selectedOrder->getTrainerId(), rating);
                    cout << "Спасибо за вашу оценку!\n";
                    Trainer* trainer = getTrainerById(selectedOrder->getTrainerId());
                    if (trainer) cout << "Обновленный рейтинг тренера " << trainer->getName() << ": " << trainer->getRating() << "\n";
                }
                else cout << "Ошибка при добавлении оценки.\n";
            }
            else if (choice == 4) showAllTrainers();
            else { currentUser = NULL; cout << "Вы вышли из аккаунта.\n"; return; }
        }
    }
    void showTrainerMenu() {
        Trainer* trainer = dynamic_cast<Trainer*>(currentUser);
        if (!trainer) return;
        while (true) {
            cout << "\n=== МЕНЮ ТРЕНЕРА ===\n";
            cout << "1. Просмотреть мои заказы\n2. Мое расписание\n";
            cout << "3. Подтвердить/отказать заказ\n4. Завершить тренировку\n5. Выйти из аккаунта\n";

            int choice = inputInt("Выберите действие: ", 1, 5);
            if (choice == 1) {
                cout << "\n=== Заказы тренера ===\n";
                int cnt = 0;
                for (size_t i = 0; i < orders.size(); i++)
                    if (orders[i]->getTrainerId() == trainer->getId()) {
                        orders[i]->displayDetailed(); ++cnt; cout << "\n";
                    }
                if (cnt == 0) cout << "Заказов не найдено.\n";
            }
            else if (choice == 2) {
                string date = inputDate("Дата (ДД.ММ.ГГГГ): ");
                vector<string> free = trainer->getAvailableSlots(date);
                if (free.empty()) cout << "На эту дату свободных слотов нет.\n";
                else {
                    cout << "Свободное время: ";
                    for (size_t i = 0; i < free.size(); i++) cout << free[i] << ' '; cout << '\n';
                }
            }
            else if (choice == 3) {
                vector<Order*> pending;
                for (size_t i = 0; i < orders.size(); i++)
                    if (orders[i]->getTrainerId() == trainer->getId() && orders[i]->getStatus() == Order::PENDING)
                        pending.push_back(orders[i]);
                if (pending.empty()) { cout << "Нет заказов в ожидании.\n"; continue; }
                cout << "\n=== Ожидающие заказы ===\n";
                for (size_t i = 0; i < pending.size(); i++) {
                    cout << i + 1 << ". "; pending[i]->display();
                }
                int pick = inputInt("Выберите заказ: ", 1, (int)pending.size());
                Order* target = pending[pick - 1];
                cout << "1. Подтвердить 2. Отказать\n";
                int act = inputInt("> ", 1, 2);
                if (act == 1) { target->changeStatus(Order::CONFIRMED); cout << "Подтверждено.\n"; }
                else {
                    pair<string, string> dt = splitDateTime(target->getDateTime());
                    trainer->cancelBooking(dt.first, dt.second);
                    target->changeStatus(Order::CANCELLED); cout << "Отказано, слот освобождён.\n";
                }
            }
            else if (choice == 4) {
                vector<Order*> confirmedOrders;
                for (size_t i = 0; i < orders.size(); i++)
                    if (orders[i]->getTrainerId() == trainer->getId() && orders[i]->getStatus() == Order::CONFIRMED)
                        confirmedOrders.push_back(orders[i]);
                if (confirmedOrders.empty()) { cout << "Нет подтвержденных тренировок для завершения.\n"; continue; }
                cout << "\n=== Подтвержденные тренировки ===\n";
                for (size_t i = 0; i < confirmedOrders.size(); i++) {
                    cout << i + 1 << ". "; confirmedOrders[i]->display();
                }
                int orderChoice = inputInt("Выберите тренировку для завершения: ", 1, (int)confirmedOrders.size());
                Order* selectedOrder = confirmedOrders[orderChoice - 1];
                selectedOrder->markCompleted();
                cout << "Тренировка отмечена как завершенная.\n";
            }
            else { currentUser = NULL; cout << "Вы вышли из аккаунта.\n"; return; }
        }
    }
    void showAdminMenu() {
        Admin* admin = dynamic_cast<Admin*>(currentUser);
        if (!admin) return;

        while (true) {
            cout << "\n=== МЕНЮ АДМИНИСТРАТОРА ===\n";
            cout << "1. Просмотреть всех пользователей\n2. Просмотреть все заказы\n";
            cout << "3. Информация о системе\n4. Выйти из аккаунта\n";

            int choice = inputInt("Выберите действие: ", 1, 4);
            if (choice == 1) {
                cout << "\n=== Все пользователи системы ===\n";
                cout << "Клиенты (" << clients.size() << "):\n";
                for (size_t i = 0; i < clients.size(); i++) { cout << "  "; clients[i]->display(); }
                cout << "\nТренеры (" << trainers.size() << "):\n";
                for (size_t i = 0; i < trainers.size(); i++) { cout << "  "; trainers[i]->display(); }
                cout << "\nАдминистраторы (" << admins.size() << "):\n";
                for (size_t i = 0; i < admins.size(); i++) { cout << "  "; admins[i]->display(); }
            }
            else if (choice == 2) {
                cout << "\n=== Все заказы системы ===\n";
                for (size_t i = 0; i < orders.size(); i++) orders[i]->displayDetailed();
            }
            else if (choice == 3) {
                cout << "\n=== Информация о системе ===\n";
                cout << "Всего пользователей: " << (clients.size() + trainers.size() + admins.size()) << "\n";
                cout << "Всего заказов: " << orders.size() << "\n";
                cout << "Следующий ID: " << nextId << "\n";
                admin->display();
            }
            else { currentUser = NULL; cout << "Вы вышли из аккаунта.\n"; return; }
        }
    }
    // Главный цикл приложения
    void run() {
        while (true) {
            cout << "\n==================================================\n";
            cout << "                 ФИТНЕС МЕНЕДЖЕР\n";
            cout << "==================================================\n";
            cout << "1. Войти в систему\n2. Зарегистрироваться\n3. Выйти из приложения\n";

            int ch = inputInt("Выберите действие: ", 1, 3);
            if (ch == 1) {
                string lg = inputLine("Логин: ");
                string pw = inputLine("Пароль: ");
                if (login(lg, pw)) {
                    cout << "\nУспешный вход! Добро пожаловать, " << currentUser->getName() << "!\n";
                    string role = currentUser->getRole();
                    if (role == "Client") showClientMenu();
                    else if (role == "Trainer") showTrainerMenu();
                    else if (role == "Admin") showAdminMenu();
                }
                else cout << "Ошибка: неверный логин или пароль!\n";
            }
            else if (ch == 2) registerClient();
            else { cout << "До свидания!\n"; return; }
        }
    }
};
int main() {
    setlocale(LC_ALL, "Russian");
    FitnessManager app;
    app.run();
    return 0;
}