#include "GameCharactor.h"
#include <iostream>
#include <random>

const vector<string> GameCharactor::EQUIP_TYPES = {"helmet", "chestplate", "leggings", "boots", "shield", "weapon"};

int GameCharactor::getMaxHP() const {
    int HP = baseHP;
    for (auto &item : equipped) HP += item.second.getHP();
    return HP;
}

int GameCharactor::getATK() const {
    int ATK = baseATK;
    for (auto &item : equipped) ATK += item.second.getATK();
    return ATK;
}

int GameCharactor::getDEF() const {
    int DEF = baseDEF;
    for (auto &item : equipped) DEF += item.second.getDEF();
    return DEF;
}

void GameCharactor::addToInventory(Item newItem) {
    if (find(EQUIP_TYPES.begin(), EQUIP_TYPES.end(), newItem.getType()) != EQUIP_TYPES.end() &&
        equipped.find(newItem.getType()) == equipped.end()) {
        equipped.insert(pair<string, Item>(newItem.getType(), newItem));
    } else {
        unequipped.push_back(newItem);
    }
}

int GameCharactor::getCalculatedDamage(int ATK, int DEF) {
    static random_device rd;
    static mt19937 gen(rd());
    normal_distribution<double> d(1, 0.1);
    return round(ATK*(1.0/(1.0+DEF/100.0))*d(gen));
}

void GameCharactor::addDamageTaken(int damage) {
    damageTaken += damage;
}

int GameCharactor::getMoney() const{
    return money;
}

void GameCharactor::setMoney(int newMoney) {
    money = newMoney;
}

GameCharactor::GameCharactor(string name, const json& statsJson) : Object(name) {
    baseHP = baseATK = baseDEF = damageTaken = money = EXP = 0;
    for (auto &stat : statsJson.items()) {
        if (stat.key() == "baseHP") baseHP = stat.value();
        else if (stat.key() == "baseATK") baseATK = stat.value();
        else if (stat.key() == "baseDEF") baseDEF = stat.value();
        else if (stat.key() == "damageTaken") damageTaken = stat.value();
        else if (stat.key() == "money") money = stat.value();
        else if (stat.key() == "EXP") EXP = stat.value();
    }

    if (statsJson.find("equipped") != statsJson.end()) {
        for (auto &itemJson : statsJson["equipped"]) {
            Item item = Item(itemJson["name"], itemJson["properties"]);
            string itemType = item.getType();
            if (equipped.find(itemType) != equipped.end()) {
                cerr << "Equip " << itemType << "twice" << endl;
                exit(EXIT_FAILURE);
            }
            equipped.insert(pair<string, Item>(itemType, item));
        }
    }

    if (statsJson.find("unequipped") != statsJson.end()) {
        for (auto &itemJson : statsJson["unequipped"]) {
            unequipped.push_back(Item(itemJson["name"], itemJson["properties"]));
        }
    }
}

GameCharactor::GameCharactor(string name) : Object(name) {
    baseHP = baseATK = baseDEF = damageTaken = money = EXP = 0;
}

int GameCharactor::getHP() const {
    return max(0, getMaxHP() - damageTaken);
}

ostream &operator<<(ostream &out, GameCharactor &gameCharactor) {
    out << gameCharactor.getBriefStats();
    return out;
}

string GameCharactor::getBar(int num, int maxNum) {
    string bar;
    int fill, left;
    fill = 10.0*num/maxNum;
    left = 10 - fill;

    bar += '|';
    bar += string(fill, '-');
    bar += string(left, ' ');
    bar += '|';
    bar += "(" + to_string(num) + "/" + to_string(maxNum) + ")";
    return bar;
}

string GameCharactor::getBriefStats() const {
    int HP = getHP();
    int maxHP = getMaxHP();
    string briefStats;
    briefStats = "[" + name + "]" + " HP:" +
                 GameCharactor::getBar(HP, maxHP);
    return briefStats;
}

json GameCharactor::getJson() const {
    json j = Object::getJson();
    j["stats"]["baseHP"] = baseHP;
    j["stats"]["baseATK"] = baseATK;
    j["stats"]["baseDEF"] = baseDEF;
    j["stats"]["damageTaken"] = damageTaken;
    j["stats"]["money"] = money;
    j["stats"]["EXP"] = EXP;

    j["stats"]["equipped"] = json::array();
    for (auto &item : equipped) {
        j["stats"]["equipped"].push_back(item.second.getJson());
    }

    j["stats"]["unequipped"] = json::array();
    for (auto &item : unequipped) {
        j["stats"]["unequipped"].push_back(item.getJson());
    }
    return j;
}

int GameCharactor::getLV() const {
    for (int i = 2; i <= 1000; i++) {
        if (EXP < getLVRequirement(i)) return i-1;
    }
    return 1000;
}

int GameCharactor::getLVRequirement(int lv) {
    return 30*(lv-1)*(lv-1);
}

void GameCharactor::addEXP(int exp) {
    EXP += exp;
}

void GameCharactor::setDamageTaken(int damage) {
    this->damageTaken = damage;
}
