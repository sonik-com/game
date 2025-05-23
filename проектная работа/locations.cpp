// �������
//-����� ������-������ (����� ����� ������� ��� (����� ������������ ���� ����� �������� ��� ������ ������������ �������))
//-��������-���: �������, �����, ���������� �����, ����

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

    // ����������
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
            {COFFEE,       {"����", 15, 10, 0, ""}},
            {TICKET,       {"�����", 0, 0, 0, ""}},
            {ARTIFACT,     {"��������", 0, 0, 0, ""}},
            {FLOWER,      {"�����", 0, 5, 0, ""}},
            {FOOD,        {"���", 20, 15, 0, ""}},
            {WOODEN_SWORD, {"���������� ���", 50, 0, 3, ""}},
            {STEEL_SWORD,  {"�������� ���", 150, 0, 7, ""}},
            {IRON_AXE,     {"�������� �����", 120, 0, 5, ""}},
            {LEATHER_ARMOR, {"������� �����", 80, 0, 2, ""}},
            {CHAINMAIL,     {"��������", 200, 0, 5, ""}},
            {PLATE_ARMOR,   {"������ �����", 300, 0, 8, ""}}
        };
    }

    virtual void interact() = 0;

    virtual void showSubLocations() {
        if (subLocations.empty()) {
            std::cout << "����� ������ ��� ����������\n";
            return;
        }

        std::cout << "��������� ����������:\n";
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
    MarketLocation() : Location("����� ������-������") {
        goods = {
            {"������", 10},
            {"�����", 8},
            {"����", 15}
        };

        for (int i = 0; i < 3; ++i) {
            subLocations.push_back(new FoodCart(i + 1));
        }
    }

    void interact() override {
        std::cout << "����� ���������� �� �����! �������� �������:\n";
        showSubLocations();
    }
};

class FoodCart : public Location {
    int cartNumber;
public:
    FoodCart(int num) : Location("������� " + std::to_string(num)), cartNumber(num) {}

    void interact() override {
        std::cout << "�� ��������� ������ �������� � ������� " << cartNumber << std::endl;
    }
};

class HampsteadLocation : public Location {
public:
    HampsteadLocation() : Location("��������-���") {
        subLocations.push_back(new CoffeeHouse());
        subLocations.push_back(new Theatre());
        subLocations.push_back(new Museum());
        subLocations.push_back(new Park());
    }

    void interact() override {
        std::cout << "�� � ��������-����. ���������� ���������������������:\n";
        showSubLocations();
    }
};

class CoffeeHouse : public Location {
public:
    CoffeeHouse() : Location("�������") {}

    void interact() override {
        std::cout << "������ ��������������� ����...\n";
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
    Theatre() : Location("�����") {}

    void interact() override {
        std::cout << "����� ������ ��������...\n";
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
    Museum() : Location("���������� �����") {}

    void interact() override {
        std::cout << "���� ������� ����������...\n";
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
    Park() : Location("����") {}

    void interact() override {
        std::cout << "�������� ��������� �������...\n";
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
        std::cout << "\n=== ���������� ==="
            << "\n������: " << currentLocation->getLootData().at(currentWeapon).description
            << " (+" << currentLocation->getLootData().at(currentWeapon).strength << ")"
            << "\n�����: " << currentLocation->getLootData().at(currentArmor).description
            << " (+" << currentLocation->getLootData().at(currentArmor).strength << ")"
            << "\n����� ����: " << getTotalStrength() << "\n";
    }

    void startGame() {
        while (true) {
            std::cout << "\n�� ���������� �: " << currentLocation->getName() << std::endl;
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
        std::cout << "�������� ������� (0 - ��������� �����): ";
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
            std::cout << "�������� �����!\n";
        }
    }

    void returnToMain() {
        if (dynamic_cast<MarketLocation*>(currentLocation) ||
            dynamic_cast<HampsteadLocation*>(currentLocation)) {
            std::cout << "�� ��� � �������� �������\n";
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
        // ��������� ������ ��� ����������
        else if (dynamic_cast<Museum*>(currentLocation) && chance > 80) {
            foundItem = (std::rand() % 2) ? CHAINMAIL : ARTIFACT;
        }

        if (foundItem != NOTHING) {
            std::cout << "\n�������: " << currentLocation->getLootData().at(foundItem).description << "\n";
            inventory[foundItem]++;
            tryEquip(foundItem);
        }
    }

    void tryEquip(LootType item) {
        const auto& data = currentLocation->getLootData().at(item);
        if (item > WEAPON_START && item < WEAPON_END && data.strength > currentLocation->getLootData().at(currentWeapon).strength) {
            currentWeapon = item;
            std::cout << "����������� ����� ������!\n";
        }
        else if (item > ARMOR_START && item < ARMOR_END && data.strength > currentLocation->getLootData().at(currentArmor).strength) {
            currentArmor = item;
            std::cout << "����������� ����� �����!\n";
        }
    }
};


