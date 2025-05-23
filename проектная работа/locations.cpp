// ЛОКАЦИИ
//-рынок Ковент-Гарден (здесь можно купиить еды (чтобы восстановить силы после сражения или долгих обследований локаций))
//-хэмпстед-хит: кофейня, театр, британский музей, парк

#include <iostream>
#include <vector>
#include <string>
#include <memory> 
#include <cstdlib>
#include <ctime>
#include <fstream>
#include <sstream>
#include <map>


enum LootType {
    NOTHING,
    COFFEE,
    TICKET,
    ARTIFACT,
    FLOWER,
    FOOD,

    // Снаряжение
    WEAPON_START,
    WOODEN_SWORD,
    STEEL_SWORD,
    IRON_AXE,
    WEAPON_END,

    ARMOR_START,
    LEATHER_ARMOR,
    CHAINMAIL,
    PLATE_ARMOR,
    ARMOR_END
};

struct LootData {
    std::string description;
    int price;
    int energy;
    int strength;
    std::string text;
};

class Location {
protected:
    std::string name;
    std::vector<Location*> subLocations;
    std::map<LootType, LootData> lootData;

public:
    Location(const std::string& name) : name(name) {
        lootData = {
            {NOTHING,      {"", 0, 0, 0, ""}},
            {COFFEE,       {"Кофе", 15, 10, 0, ""}},
            {TICKET,       {"Билет", 0, 0, 0, ""}},
            {ARTIFACT,     {"Артефакт", 0, 0, 0, ""}},
            {FLOWER,      {"Цветы", 0, 5, 0, ""}},
            {FOOD,        {"Еда", 20, 15, 0, ""}},
            {WOODEN_SWORD, {"Деревянный меч", 50, 0, 3, ""}},
            {STEEL_SWORD,  {"Стальной меч", 150, 0, 7, ""}},
            {IRON_AXE,     {"Железный топор", 120, 0, 5, ""}},
            {LEATHER_ARMOR, {"Кожаная броня", 80, 0, 2, ""}},
            {CHAINMAIL,     {"Кольчуга", 200, 0, 5, ""}},
            {PLATE_ARMOR,   {"Латная броня", 300, 0, 8, ""}}
        };
    }

    virtual void interact() = 0;

    virtual void showSubLocations() {
        if (subLocations.empty()) {
            std::cout << "Здесь больше нет подлокаций\n";
            return;
        }

        std::cout << "Доступные подлокации:\n";
        for (std::size_t i = 0; i < subLocations.size(); ++i) {
            std::cout << i + 1 << ". " << subLocations[i]->name << std::endl;
        }
    }

    std::vector<Location*>& getSubLocations() { return subLocations; }
    std::string getName() const { return name; }
    const std::map<LootType, LootData>& getLootData() const { return lootData; }
};

class MarketLocation : public Location {
    std::map<std::string, int> goods;

public:
    MarketLocation() : Location("Рынок Ковент-Гарден") {
        goods = {
            {"Фрукты", 10},
            {"Овощи", 8},
            {"Рыба", 15}
        };

        for (int i = 0; i < 3; ++i) {
            subLocations.push_back(new FoodCart(i + 1));
        }
    }

    void interact() override {
        std::cout << "Добро пожаловать на рынок! Выберите тележку:\n";
        showSubLocations();
    }
};

class FoodCart : public Location {
    int cartNumber;
public:
    FoodCart(int num) : Location("Тележка " + std::to_string(num)), cartNumber(num) {}

    void interact() override {
        std::cout << "Вы покупаете свежие продукты в тележке " << cartNumber << std::endl;
    }
};

class HampsteadLocation : public Location {
public:
    HampsteadLocation() : Location("Хэмпстед-Хит") {
        subLocations.push_back(new CoffeeHouse());
        subLocations.push_back(new Theatre());
        subLocations.push_back(new Museum());
        subLocations.push_back(new Park());
    }

    void interact() override {
        std::cout << "Вы в Хэмпстед-Хите. Исследуйте достопримечательности:\n";
        showSubLocations();
    }
};

class CoffeeHouse : public Location {
public:
    CoffeeHouse() : Location("Кофейня") {}

    void interact() override {
        std::cout << "Аромат свежесваренного кофе...\n";
        if (std::rand() % 100 > 70) {
            std::cout << lootDescriptions[COFFEE] << std::endl;
        }
        else {
            std::cout << lootDescriptions[NOTHING] << std::endl;
        }
    }
};

class Theatre : public Location {
public:
    Theatre() : Location("Театр") {}

    void interact() override {
        std::cout << "Здесь ставят Шекспира...\n";
        if (std::rand() % 100 > 80) {
            std::cout << lootDescriptions[TICKET] << std::endl;
        }
        else {
            std::cout << lootDescriptions[NOTHING] << std::endl;
        }
    }
};

class Museum : public Location {
public:
    Museum() : Location("Британский музей") {}

    void interact() override {
        std::cout << "Залы древних экспонатов...\n";
        if (std::rand() % 100 > 90) {
            std::cout << lootDescriptions[ARTIFACT] << std::endl;
        }
        else {
            std::cout << lootDescriptions[NOTHING] << std::endl;
        }
    }
};

class Park : public Location {
public:
    Park() : Location("Парк") {}

    void interact() override {
        std::cout << "Красивые природные пейзажи...\n";
        if (std::rand() % 100 > 60) {
            std::cout << lootDescriptions[FLOWER] << std::endl;
        }
        else {
            std::cout << lootDescriptions[NOTHING] << std::endl;
        }
    }
};


class GameManager {
    std::vector<Location*> mainLocations;
    Location* currentLocation;

    int baseStrength = 5;
    LootType currentWeapon = WOODEN_SWORD;
    LootType currentArmor = LEATHER_ARMOR;
    std::map<LootType, int> inventory;

public:
    GameManager() {
        std::srand(std::time(NULL));
        mainLocations.push_back(new MarketLocation());
        mainLocations.push_back(new HampsteadLocation());
        currentLocation = mainLocations[0];
    }

    int getTotalStrength() const {
        return baseStrength +
            currentLocation->getLootData().at(currentWeapon).strength +
            currentLocation->getLootData().at(currentArmor).strength;
    }

    void showEquipment() const {
        std::cout << "\n=== СНАРЯЖЕНИЕ ==="
            << "\nОружие: " << currentLocation->getLootData().at(currentWeapon).description
            << " (+" << currentLocation->getLootData().at(currentWeapon).strength << ")"
            << "\nБроня: " << currentLocation->getLootData().at(currentArmor).description
            << " (+" << currentLocation->getLootData().at(currentArmor).strength << ")"
            << "\nОбщая сила: " << getTotalStrength() << "\n";
    }

    void startGame() {
        while (true) {
            std::cout << "\nВы находитесь в: " << currentLocation->getName() << std::endl;
            currentLocation->interact();

            if (!currentLocation->getSubLocations().empty()) {
                handleNavigation();
            }
            else {
                handleLoot();
                returnToMain();
            }
        }
    }

private:
    void handleNavigation() {
        int choice;
        std::cout << "Выберите локацию (0 - вернуться назад): ";
        std::cin >> choice;

        if (choice == 0) {
            returnToMain();
            return;
        }

        auto& subs = currentLocation->getSubLocations();
        if (choice > 0 && choice <= static_cast<int>(subs.size())) {
            currentLocation = subs[choice - 1];
        }
        else {
            std::cout << "Неверный выбор!\n";
        }
    }

    void returnToMain() {
        if (dynamic_cast<MarketLocation*>(currentLocation) ||
            dynamic_cast<HampsteadLocation*>(currentLocation)) {
            std::cout << "Вы уже в основной локации\n";
            return;
        }
        currentLocation = mainLocations[0];
    }
    void handleLoot() {
        int chance = std::rand() % 100;
        LootType foundItem = NOTHING;

        if (dynamic_cast<CoffeeHouse*>(currentLocation) && chance > 30) {
            foundItem = COFFEE;
        }
        else if (dynamic_cast<Theatre*>(currentLocation) && chance > 30) {
            foundItem = TICKET;
        }
        // Добавлена логика для снаряжения
        else if (dynamic_cast<Museum*>(currentLocation) && chance > 80) {
            foundItem = (std::rand() % 2) ? CHAINMAIL : ARTIFACT;
        }

        if (foundItem != NOTHING) {
            std::cout << "\nНайдено: " << currentLocation->getLootData().at(foundItem).description << "\n";
            inventory[foundItem]++;
            tryEquip(foundItem);
        }
    }

    void tryEquip(LootType item) {
        const auto& data = currentLocation->getLootData().at(item);
        if (item > WEAPON_START && item < WEAPON_END && data.strength > currentLocation->getLootData().at(currentWeapon).strength) {
            currentWeapon = item;
            std::cout << "Экипировано новое оружие!\n";
        }
        else if (item > ARMOR_START && item < ARMOR_END && data.strength > currentLocation->getLootData().at(currentArmor).strength) {
            currentArmor = item;
            std::cout << "Экипирована новая броня!\n";
        }
    }
};


