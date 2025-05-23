#include <iostream>
#include <vector>
#include <string>
#include <memory> // Для std::unique_ptr
#include <cstdlib>
#include <ctime>
#include <fstream>
#include <sstream>
#include "locations.срр"
#include "battle.срр"
#include "проектная работа.cpp"

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