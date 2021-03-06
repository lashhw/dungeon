#include "GameCharactor.h"

#include <iostream>
#include <random>

const vector<string> GameCharactor::EQUIP_TYPES = {"helmet", "chestplate", "leggings", "boots", "shield", "weapon"};

int GameCharactor::getMaxHP() const {
    int HP = baseHP;
    for (auto &item : equipped) HP += item.second.getHP();
    return HP;
}

int GameCharactor::getPATK() const {
    int PATK = basePATK;
    for (auto &item : equipped) PATK += item.second.getPATK();
    return PATK;
}

int GameCharactor::getMATK() const {
    int MATK = baseMATK;
    for (auto &item : equipped) MATK += item.second.getMATK();
    return MATK;
}

int GameCharactor::getPDEF() const {
    int PDEF = basePDEF;
    for (auto &item : equipped) PDEF += item.second.getPDEF();
    return PDEF;
}

int GameCharactor::getMDEF() const {
    int MDEF = baseMDEF;
    for (auto &item : equipped) MDEF += item.second.getMDEF();
    return MDEF;
}

void GameCharactor::addToInventory(const Item &newItem) {
    if (find(EQUIP_TYPES.begin(), EQUIP_TYPES.end(), newItem.getType()) != EQUIP_TYPES.end() &&
        equipped.find(newItem.getType()) == equipped.end()) {
        equipped.insert(pair<string, Item>(newItem.getType(), newItem));
    } else {
        unequipped.push_back(newItem);
    }
}

int GameCharactor::getCalculatedDamage(int PATK, int MATK, int PDEF, int MDEF) {
    static random_device rd;
    static mt19937 gen(rd());
    static normal_distribution<double> d(1, 0.1);
    return (int)round(PATK*(1.0/(1.0+PDEF/100.0))*d(gen)+MATK*(1.0/(1.0+MDEF/100.0))*d(gen));
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
    baseHP = basePATK = baseMATK = basePDEF = baseMDEF = damageTaken = money = EXP = 0;
    for (auto &stat : statsJson.items()) {
        if (stat.key() == "baseHP") baseHP = stat.value();
        else if (stat.key() == "basePATK") basePATK = stat.value();
        else if (stat.key() == "baseMATK") baseMATK = stat.value();
        else if (stat.key() == "basePDEF") basePDEF = stat.value();
        else if (stat.key() == "baseMDEF") baseMDEF = stat.value();
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
            unequipped.emplace_back(itemJson["name"], itemJson["properties"]);
        }
    }
}

GameCharactor::GameCharactor(string name) : Object(name) {
    baseHP = basePATK = baseMATK = basePDEF = baseMDEF = damageTaken = money = EXP = 0;
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
    size_t fill, left;
    fill = (size_t)(10.0*num/maxNum);
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
    j["stats"]["basePATK"] = basePATK;
    j["stats"]["baseMATK"] = baseMATK;
    j["stats"]["basePDEF"] = basePDEF;
    j["stats"]["baseMDEF"] = baseMDEF;
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

int GameCharactor::getDamageTaken() const {
    return damageTaken;
}

int GameCharactor::decreaseDamageTaken(int damage) {
    int oldDamageTaken = damageTaken;
    damageTaken = max(0, damageTaken - damage);
    return oldDamageTaken - damageTaken;
}
