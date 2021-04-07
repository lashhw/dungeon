#include "Player.h"
#include "Dungeon.h"

#include <sstream>

using namespace std;

int Player::getMaxHP() const {
    int LV = getLV();
    int HP = (int)(baseHP*(1+0.05*(LV-1)*(LV-1)));
    for (auto &item : equipped) HP += item.second.getHP();
    return HP;
}

int Player::getATK() const {
    int LV = getLV();
    int ATK = (int)(baseATK*(1+0.05*(LV-1)*(LV-1)));
    for (auto &item : equipped) ATK += item.second.getATK();
    return ATK;
}

int Player::getDEF() const {
    int LV = getLV();
    int DEF = (int)(baseDEF*(1+0.05*(LV-1)*(LV-1)));
    for (auto &item : equipped) DEF += item.second.getDEF();
    return DEF;
}

void Player::printStats() const {
    stringstream unformattedSS;
    int LV = getLV();
    unformattedSS << "Name: " << name << endl
                  << "LV: " << LV << " " << getBar(EXP-getLVRequirement(LV), getLVRequirement(LV+1)-getLVRequirement(LV)) << endl
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
        typeCapitalized[0] = (char)toupper(typeCapitalized[0]);
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

bool Player::triggerEvent(Object &object) { // print / change equipment
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
        Dungeon::clearConsole();

        if (input != "x") {
            int itemToEquipIdx = stoi(input) - 1;
            Item itemToEquip = unequipped[itemToEquipIdx];
            unequipped.erase(unequipped.begin() + itemToEquipIdx);
            if (find(EQUIP_TYPES.begin(), EQUIP_TYPES.end(), itemToEquip.getType()) != EQUIP_TYPES.end()) {
                unequipped.push_back(equipped.find(itemToEquip.getType())->second);
                equipped.find(itemToEquip.getType())->second = itemToEquip;
            } else if (itemToEquip.getType() == "usable") {
                int damageToDecrease = min(itemToEquip.getHP(), damageTaken);
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

void Player::changeRoom(Room *newRoomPtr) {
    prevRoomPtr = currRoomPtr;
    currRoomPtr = newRoomPtr;
}

string Player::getTag() const {
    return "Player";
}

json Player::getJson() const {
    json j = GameCharactor::getJson();
    j["currRoom"] = currRoomPtr->getIndex();
    j["prevRoom"] = prevRoomPtr->getIndex();
    return j;
}

void Player::setName(string name) {
    this->name = name;
}

const Room * Player::getCurrRoomPtr() const {
    return currRoomPtr;
}

const Room * Player::getPrevRoomPtr() const {
    return prevRoomPtr;
}

Player::Player(string name, const json &statsJson, Room *currRoomPtr, Room *prevRoomPtr)
              :GameCharactor(name, statsJson), currRoomPtr(currRoomPtr), prevRoomPtr(prevRoomPtr) {}

void Player::addObjectPtrInRoom(shared_ptr<Object> newObjectPtr) {
    currRoomPtr->addObjectPtr(newObjectPtr);
}

void Player::removeObjectInRoomByIdx(int objectIdxToDelete) {
    currRoomPtr->removeObjectPtrByIdx(objectIdxToDelete);
}

void Player::changeToPrevRoom() {
    currRoomPtr = prevRoomPtr;
}
