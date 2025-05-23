#include <iostream>
#include <vector>
#include <string>
#include <memory> // ��� std::unique_ptr
#include <cstdlib>
#include <ctime>
#include <fstream>
#include <sstream>
#include "locations.���"
#include "battle.���"
#include "��������� ������.cpp"

void viewLoot() {
    std::cout << "�� �����:" << std::endl;
    if (foundLoot.empty()) {
        std::cout << "� ������� �����." << std::endl;
    }
    else {
        for (const auto& loot : foundLoot) {
            std::cout << loot.name << ": " << loot.quantity << " (����: " << loot.price << " �� �����)" << std::endl;
        }
    }
}