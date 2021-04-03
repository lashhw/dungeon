#include "Player.h"
#include "Dungeon.h"
#include <iostream>
#include <sstream>

using namespace std;

int Player::getMaxHP() const {
    int LV = getLV();
    int HP = baseHP*(1+0.05*(LV-1)*(LV-1));
    for (auto &item : equipped) HP += item.second.getHP();
    return HP;
}

int Player::getATK() const {
    int LV = getLV();
    int ATK = baseATK*(1+0.05*(LV-1)*(LV-1));
    for (auto &item : equipped) ATK += item.second.getATK();
    return ATK;
}

int Player::getDEF() const {
    int LV = getLV();
    int DEF = baseDEF*(1+0.05*(LV-1)*(LV-1));
    for (auto &item : equipped) DEF += item.second.getDEF();
    return DEF;
}
int Player::getCurrRoomIdx() const {
    return currRoomIdx;
}

void Player::printStats() const {
    stringstream unformattedSS;
    int LV = getLV();
    unformattedSS << "Name: " << name << endl
                  << "LV: " << LV << " " << getBar(EXP-getLVRequirement(LV), getLVRequirement(LV+1)) << endl
                  << "HP: " << getBar(getHP(), getMaxHP()) << endl
                  << "ATK: " << getATK() << endl
                  << "DEF: " << getDEF() << endl
                  << "Money: $" << money;
    string unformatted = unformattedSS.str();
    cout << Dungeon::getBoxedString(unformatted, 80, 2, true, true) << endl;

    unformattedSS.str(string());
    unformattedSS << "Inventory:" << endl;

    unformattedSS << " - Equipped";
    for (auto &type : GameCharactor::EQUIP_TYPES) {
        unformattedSS << endl;
        string typeCapitalized = type;
        typeCapitalized[0] = toupper(typeCapitalized[0]);
        unformattedSS << string(4, ' ') << typeCapitalized << ": ";
        if (equipped.find(type) != equipped.end()) {
            unformattedSS << equipped.find(type)->second;
        }
    }
    unformattedSS << endl;
    unformattedSS << " - Not equipped";
    for (const auto &item : unequipped) {
        unformattedSS << endl;
        unformattedSS << string(4, ' ') << item.getInfo(true);
    }
    unformatted = unformattedSS.str();
    cout << Dungeon::getBoxedString(unformatted, 80, 2, false, true) << endl;
}

bool Player::triggerEvent(Object &object, Room& currRoom) { // print / change equipment
    printStats();

    if (!unequipped.empty()) {
        int unequippedCount = 0;
        vector<string> choices;
        for (const auto& unequippedItem : unequipped) {
            unequippedCount++;
            cout << Dungeon::CHOICE_START << "[" << unequippedItem.getName() << "]"
                 << " (" << unequippedCount << ")" << endl;
            choices.push_back(to_string(unequippedCount));
        }
        cout << Dungeon::CHOICE_START << "Cancel (x)" << endl;
        choices.push_back("x");
        cout << "Select the item you want to equip/use: ";
        string input = Dungeon::getResponce(choices);

        if (input != "x") {
            int itemToEquipIdx = stoi(input) - 1;
            Item itemToEquip = unequipped[itemToEquipIdx];
            unequipped.erase(unequipped.begin() + itemToEquipIdx);
            if (find(EQUIP_TYPES.begin(), EQUIP_TYPES.end(), itemToEquip.getType()) != EQUIP_TYPES.end()) {
                unequipped.push_back(equipped.find(itemToEquip.getType())->second);
                equipped.find(itemToEquip.getType())->second = itemToEquip;
            } else if (itemToEquip.getType() == "usable") {
                int damageToDecrease = 0;
                damageToDecrease = min(itemToEquip.getHP(), damageTaken);
                damageTaken -= damageToDecrease;
                cout << "You healed yourself by " << damageToDecrease << " HP" << endl;
            }
        }
    }
    return false;
}

string Player::getInteractMessage() const {
    return "Player: ???";
}

void Player::changeRoom(int newRoomIdx) {
    prevRoomIdx = currRoomIdx;
    currRoomIdx = newRoomIdx;
}

Player::Player():GameCharactor("null"){
    currRoomIdx = prevRoomIdx = -1;
}

Player::Player(string name, const json &statsJson, int currRoomIdx, int prevRoomIdx)
               :GameCharactor(name, statsJson), currRoomIdx(currRoomIdx), prevRoomIdx(prevRoomIdx) {}

int Player::getPrevRoomIdx() const {
    return prevRoomIdx;
}

string Player::getTag() const {
    return "Player";
}

json Player::getJson() const {
    json j = GameCharactor::getJson();
    j["currRoom"] = currRoomIdx;
    j["prevRoom"] = prevRoomIdx;
    return j;
}

void Player::setName(string name) {
    this->name = name;
}

