// проектная работа.cpp : Этот файл содержит функцию "main". Здесь начинается и заканчивается выполнение программы.
//

/*#include <iostream>
#include <vector>
#include <string>
#include <memory> // Для std::unique_ptr
#include <cstdlib>
#include <ctime>
#include <fstream>
#include <sstream>
//#include <SFML/Graphics.hpp>
//#include <SFML/Window/Mouse.hpp>

//#define CHANCE_

struct Loot {
    std::string name;
    int quantity;
    int price;
    Loot(std::string n, int q, int p) : name(n), quantity(q), price(p) {}
};

class Excavation {
private:
    std::string name;
    std::string terrain_type;
    int cost;
    int food_required;

public:
    Excavation(std::string n, std::string t, int c, int f) : name(n), terrain_type(t), cost(c), food_required(f) {}
    virtual void excavate(std::vector<Loot>& loot) = 0; // Виртуальная функция для обязательной реализации в классах-наследниках
    virtual void effect(std::vector<Loot>& loot) = 0; // = 0 - нет реализации в этом классе, т.е. он базисный 

    std::string getName() const { return name; }
    int getCost() const { return cost; }
    int getFoodRequired() const { return food_required; }
};

class ExcavationUnderWater : public Excavation {
public:
    ExcavationUnderWater() : Excavation("Раскопки под водой - стоимость 200 - еда 1", "Вода", 200, 1) {}

    void excavate(std::vector<Loot>& loot) override { //явное переопределение метода + проверка сигнатуры(компилятор выдаст ошибку)
        std::cout << "Вы начинаете раскопки под водой!" << std::endl;
        effect(loot);
    }

    void effect(std::vector<Loot>& loot) override {
        int randomValue = std::rand() % 100; //генерация числа от 0 до RAND_MAX

        if (randomValue < 50) {
            std::cout << "Ты нашел раковину с жемчужиной! Повезло!" << std::endl;
            loot.push_back(Loot("Раковина с жемчужиной", 1, 30));
        }
        else if (randomValue < 80) {
            std::cout << "Ты нашел немного водорослей." << std::endl;
            loot.push_back(Loot("Морские водоросли", 2, 10));
        }
        else {
            std::cout << "Вы ничего не нашли!" << std::endl;
        }
    }
};

class ExcavationWood : public Excavation {
public:
    ExcavationWood() : Excavation("Раскопки Леса - стоимость 100 - еда 1", "Лес", 100, 1) {}
    void excavate(std::vector<Loot>& loot) override {
        std::cout << "Вы начинаете раскопки леса!" << std::endl;
        effect(loot);
    }

    void effect(std::vector<Loot>& loot) override {
        int randomValue = std::rand() % 100;

        if (randomValue < 50) {
            std::cout << "Вы нашли редкий цветок! Ура!!!" << std::endl;
            loot.push_back(Loot("Редкий цветок", 1, 10));
        }
        else if (randomValue < 80) {
            std::cout << "Вы нашли немного камня." << std::endl;
            loot.push_back(Loot("Обычный камень", 2, 3));
        }
        else {
            std::cout << "Вы ничего не нашли. Не грусти:(...." << std::endl;
        }
    }
};

class ExcavationMoon : public Excavation {
public:
    ExcavationMoon() : Excavation("Раскопки на Луне - стоимость 200 - еда 3", "Ничего", 200, 3) {}
    void excavate(std::vector<Loot>& loot) override {
        std::cout << "Ты начал раскопки на Луне!" << std::endl;
        effect(loot);
    }

    void effect(std::vector<Loot>& loot) override {
        int randomValue = std::rand() % 100;

        if (randomValue < 50) {
            std::cout << "Ты нашел останки инопланетной рассы!!! ВОТ ЭТО ДА!" << std::endl;
            loot.push_back(Loot("Слизь", 1, 35));

        }
        else if (randomValue < 80) {
            std::cout << "Ты нашел немного лунного камня." << std::endl;
            loot.push_back(Loot("Лунный камень", 2, 15));
        }
        else {
            std::cout << "Ты ничего не нашёл... Будут и лучшие времена:)" << std::endl;
        }
    }
};

class Game {
private:
    int All_Money;
    int All_Food;
    std::vector<std::unique_ptr<Excavation>> excavations; // теперь экспедиции типа умный указатель
    std::vector<Loot> foundLoot;
    std::vector<std::string> equipment;
    std::vector<int> equipmentPrices;
public:
    Game() : All_Money(500), All_Food(15) {

        excavations.push_back(std::make_unique<ExcavationUnderWater>()); // делаем на конкретные экспедиции указатели и заносим их в вектор
        excavations.push_back(std::make_unique<ExcavationWood>());
        excavations.push_back(std::make_unique<ExcavationMoon>());

        equipment = { "Кирка", "Лопата", "Щётка" };
        equipmentPrices = { 50, 30, 10 };

        loadProgress();
    }
    void saveProgress() {
        std::ofstream outFile("save.txt");
        if (outFile.is_open()) {
            outFile << All_Money << std::endl;
            outFile << All_Food << std::endl;
            outFile << foundLoot.size() << std::endl;
            for (const auto loot : foundLoot) {
                outFile << loot.name << " " << loot.quantity << " " << loot.price << std::endl; // Сохраняем каждый предмет
            }
            outFile.close();
            std::cout << "Прогресс сохранен!" << std::endl;
        }
        else {
            std::cerr << "Не удается открыть файл для сохранения!" << std::endl;
        }
    }
    void loadProgress() {
        std::ifstream inFile("save.txt");
        if (inFile.is_open()) {
            inFile >> All_Money;
            inFile >> All_Food;
            int lootCount;
            inFile >> lootCount;
            foundLoot.clear(); // Очищаем то, что было
            inFile.ignore(); // Игнорируем оставшийся символ новой строки после считывания количества лута
            for (int i = 0; i < lootCount; ++i) {
                std::string line;
                std::getline(inFile, line);
                std::istringstream iss(line); // создание потока(как cin) для разделения(пробелы - разделители) считанной строки
                std::string name;
                int quantity, price;
                iss >> name >> quantity >> price;

                // Проверим, считываем ли корректные значения или выдаем ошибку, если есть лишние строки
                if (iss && iss.good()) {
                    foundLoot.emplace_back(name, quantity, price); // использовал emplace_back, чтобы не происходило копирования объекта, а он создавался прям в векторе
                }
                else {
                    std::cerr << "Ошибка при загрузке данных о найденных вещах!" << std::endl;
                }
            }
            inFile.close();
            std::cout << "Прогресс загружен!" << std::endl;
        }
        else {
            std::cerr << "Файл сохранения не найден, запускаю новую игру!" << std::endl;
        }
    }
    void resetGame() {
        All_Money = 500;
        All_Food = 15;
        foundLoot.clear();
        std::cout << "Процесс игры сброшен!" << std::endl;
    }
    void buyFood() {
        int foodPrice = 5;
        std::cout << "Еда стоит " << foodPrice << " монет за штуку. Сколько вы хотите купить?" << std::endl;
        int quantity;
        std::cin >> quantity;

        int totalCost = quantity * foodPrice;
        if (All_Money >= totalCost) {
            All_Money -= totalCost;
            All_Food += quantity;
            std::cout << "Вы купили " << quantity << " еды." << std::endl;
        }
        else {
            std::cout << "Не хватает монет!" << std::endl;
        }
    }
    void viewLoot() {
        std::cout << "Вы нашли:" << std::endl;
        if (foundLoot.empty()) {
            std::cout << "В рюкзаке пусто." << std::endl;
        }
        else {
            for (const auto& loot : foundLoot) {
                std::cout << loot.name << ": " << loot.quantity << " (Цена: " << loot.price << " за штуку)" << std::endl;
            }
        }
    }
    void sellLoot() {
        viewLoot();
        std::cout << "Введите количество предметов, которые вы хотите продать (0 для отмены): ";
        int choice;
        std::cin >> choice;

        if (choice > 0 && choice <= foundLoot.size()) {
            Loot& lootToSell = foundLoot[choice - 1];
            int totalSalePrice = lootToSell.price * lootToSell.quantity;
            All_Money += totalSalePrice;
            std::cout << "Ты продал " << lootToSell.quantity << " " << lootToSell.name << " за " << totalSalePrice << " монет." << std::endl;
            foundLoot.erase(foundLoot.begin() + (choice - 1)); // Удаляем проданный лут
        }
        else if (choice == 0) {
            std::cout << "Распродажа отменена." << std::endl;
        }
        else {
            std::cout << "Неверный выбор!" << std::endl;
        }
    }
    void start() {
        int choice;
        int itemChoice;
        int excavationChoice;
        while (true) {
            std::cout << " " << "\n";
            std::cout << "------------------------------------------------" << std::endl;
            std::cout << "Твой рюкзак: " << All_Money << " монетки и " << All_Food << " еда." << std::endl;
            std::cout << "-------------------------Меню-------------------" << std::endl;
            std::cout << "1. Приобретение оборудования" << std::endl; //Закупка оборудования
            std::cout << "2. Отправиться в экспедицию" << std::endl;
            std::cout << "3. Просмотреть весь лут" << std::endl;
            std::cout << "4. Продать что-то" << std::endl;
            std::cout << "5. Купить еду" << std::endl;
            std::cout << "6. Сохранить прогресс" << std::endl;
            std::cout << "7. Сброс игры" << std::endl;
            std::cout << "8. Выйти из игры" << std::endl;
            std::cout << "Выбери что будешь делать: ";
            std::cin >> choice;
            std::cout << " " << "\n";
            switch (choice) {
            case 1: {
                std::cout << "Приобретение оборудования (кирки, лопаты, щетки, еда)." << std::endl;
                for (size_t i = 0; i < equipment.size(); ++i) {
                    std::cout << i + 1 << ". " << equipment[i] << " - " << equipmentPrices[i] << " монет." << std::endl;
                }
                std::cout << "Выберите товар для покупки (0 для выхода): ";
                std::cin >> itemChoice;
                if (itemChoice > 0 && itemChoice <= equipment.size()) {
                    if (All_Money >= equipmentPrices[itemChoice - 1]) {
                        All_Money -= equipmentPrices[itemChoice - 1];
                        foundLoot.push_back(Loot(equipment[itemChoice - 1], 1, 1)); // если нужно поменять цену, купленного снаряжения
                        std::cout << "Вы купили: " << equipment[itemChoice - 1] << "!" << std::endl;
                    }
                    else {
                        std::cout << "Не хватает монет!" << std::endl;
                    }
                }
                else if (itemChoice == 0) {
                    std::cout << "Вы покинули меню покупок." << std::endl;
                }
                else {
                    std::cout << "Неправильный выбор!" << std::endl;
                }
                break;
            }

            case 2: {
                std::cout << "Выберите тип земляных работ: " << std::endl;
                for (size_t i = 0; i < excavations.size(); ++i) {
                    std::cout << i + 1 << ". " << excavations[i]->getName() << std::endl;
                }
                int excavationChoice;
                std::cin >> excavationChoice;

                if (excavationChoice > 0 && excavationChoice <= excavations.size()) {
                    if (All_Money >= excavations[excavationChoice - 1]->getCost()) {
                        All_Money -= excavations[excavationChoice - 1]->getCost();
                        std::cout << "Вы ходили в " << excavations[excavationChoice - 1]->getName() << "!" << std::endl;

                        while (true) {
                            if (All_Food < excavations[excavationChoice - 1]->getFoodRequired()) {
                                std::cout << "Недостаточно еды, чтобы продолжать раскопки!" << std::endl;
                                break;
                            }

                            excavations[excavationChoice - 1]->excavate(foundLoot);
                            All_Food -= excavations[excavationChoice - 1]->getFoodRequired(); // Списание еды

                            std::cout << "Вы хотите продолжить раскопки? (1 - да, 0 - нет): ";
                            int continueExcavating;
                            std::cin >> continueExcavating;
                            if (continueExcavating == 0) {
                                std::cout << "Вы вернулись домой." << std::endl;
                                break;
                            }
                        }
                    }
                    else {
                        std::cout << "Не хватило денег на экспедицию!" << std::endl;
                    }
                }
                else {
                    std::cout << "Неправильный выбор!" << std::endl;
                }
                break;
            }
            case 3: {
                std::cout << "Что в вашем рюкзаке: " << std::endl;
                viewLoot();
                break;
            }
            case 4: {
                sellLoot();
                break;
            }
            case 5: {
                buyFood();
                break;
            }
            case 6: {
                saveProgress();
                break;
            }
            case 7: {
                resetGame();
                break;
            }
            case 8: {
                std::cout << "Закрыть игру." << std::endl;
                return;
            }
            default:
                std::cout << "Неправильный выбор! Пожалуйста, попробуйте снова:)" << std::endl;
                break;
            }
        }

    }
};
int main() {
    setlocale(LC_ALL, "Russian");
    std::cout << "Добро пожаловать в симулятор палеонтолога!" << std::endl;
    Game game;
    game.start();
    game.saveProgress();
    return 0;
}*/
// main.cpp
#include <iostream>
//#include "game.срр"
#include <vector>
#include <string>
#include <memory> // Для std::unique_ptr
#include <cstdlib>
#include <ctime>
#include <fstream>
#include <sstream>
#include "locations.cpp"
//#include "inventory.срр"

/*class GameManager {
    std::vector<Location*> locations;
    Location* currentLocation;

public:
    GameManager() {
        std::srand(std::time(NULL));
        locations.push_back(new class MarketLocation());
        currentLocation = locations[0];
    }

    void startGame() {
        while (true) {
            std::cout << "\nВы в: " << currentLocation->getName() << "\n";
            currentLocation->interact();
            handleNavigation();
        }
    }

private:
    void handleNavigation() {
        int choice;
        std::cout << "Выбор (0-назад): ";
        std::cin >> choice;

        if (choice == 0) {
            currentLocation = locations[0];
            return;
        }

        if (choice > 0 && choice <= currentLocation->getSubLocations().size()) {
            currentLocation = currentLocation->getSubLocations()[choice - 1];
        }
    }
};*/

void showMainMenu() {
    std::cout << "\n=== ГЛАВНОЕ МЕНЮ ==="
        << "\n1. Бой"
        << "\n2. Локации"
        << "\n3. Инвентарь"
        << "\n0. Выход\n"
        << "Сделайте выбор: ";
}

int main() {
    GameManager game;
  //  InventoryManager inventory;

    int choice;
    do {
        showMainMenu();
        std::cin >> choice;

        switch (choice) {
        //case 1:
         //   startBattle(); // Функция из battle.cpp
           // break;
        case 2:
            game.startGame(); // Реализация из locations.cpp
            break;
       // case 3:
         //   showInventory(inventory); // Функция из inventory.cpp
           // break;
        case 0:
            std::cout << "Выход из игры...\n";
            break;
        default:
            std::cout << "Неверный выбор!\n";
        }
    } while (choice != 0);

    return 0;
}

// Запуск программы: CTRL+F5 или меню "Отладка" > "Запуск без отладки"
// Отладка программы: F5 или меню "Отладка" > "Запустить отладку"

// Советы по началу работы 
//   1. В окне обозревателя решений можно добавлять файлы и управлять ими.
//   2. В окне Team Explorer можно подключиться к системе управления версиями.
//   3. В окне "Выходные данные" можно просматривать выходные данные сборки и другие сообщения.
//   4. В окне "Список ошибок" можно просматривать ошибки.
//   5. Последовательно выберите пункты меню "Проект" > "Добавить новый элемент", чтобы создать файлы кода, или "Проект" > "Добавить существующий элемент", чтобы добавить в проект существующие файлы кода.
//   6. Чтобы снова открыть этот проект позже, выберите пункты меню "Файл" > "Открыть" > "Проект" и выберите SLN-файл.
