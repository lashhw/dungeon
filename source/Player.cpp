#include "Player.h"
#include "Dungeon.h"

#include <sstream>

using namespace std;

const vector<Skill> Player::ALL_SKILLS = {
    //    name      LV CD PATK MATK HP
    Skill("Skill1", 2, 5, 30,  0,   0),
    Skill("Skill2", 3, 4, 20,  20,  0),
    Skill("Heal"  , 5, 3, 0,   0,   100)
};

int Player::getMaxHP() const {
    int LV = getLV();
    int HP = (int)(baseHP*(1+0.05*(LV-1)*(LV-1)));
    for (auto &item : equipped) HP += item.second.getHP();
    return HP;
}

int Player::getPATK() const {
    int LV = getLV();
    int PATK = (int)(basePATK*(1+0.05*(LV-1)*(LV-1)));
    for (auto &item : equipped) PATK += item.second.getPATK();
    return PATK;
}

int Player::getMATK() const {
    int LV = getLV();
    int MATK = (int)(baseMATK*(1+0.05*(LV-1)*(LV-1)));
    for (auto &item : equipped) MATK += item.second.getMATK();
    return MATK;
}

int Player::getPDEF() const {
    int LV = getLV();
    int PDEF = (int)(basePDEF*(1+0.05*(LV-1)*(LV-1)));
    for (auto &item : equipped) PDEF += item.second.getPDEF();
    return PDEF;
}

int Player::getMDEF() const {
    int LV = getLV();
    int MDEF = (int)(baseMDEF*(1+0.05*(LV-1)*(LV-1)));
    for (auto &item : equipped) MDEF += item.second.getMDEF();
    return MDEF;
}

void Player::printStats() const {
    stringstream unformattedSS;
    int LV = getLV();
    unformattedSS << "Name: " << name << endl
                  << "LV: " << LV << " " << getBar(EXP-getLVRequirement(LV), getLVRequirement(LV+1)-getLVRequirement(LV)) << endl
                  << "HP: " << getBar(getHP(), getMaxHP()) << endl
                  << "PATK: " << getPATK() << endl
                  << "MATK: " << getMATK() << endl
                  << "PDEF: " << getPDEF() << endl
                  << "MDEF: " << getMDEF() << endl
                  << "Money: $" << money << endl
                  << "Skills:";
    for (const auto &s : skills) {
        unformattedSS << endl;
        unformattedSS << string(4, ' ') << s.getInfo();
    }

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
                cout << "You healed yourself by " << damageToDecrease << " HP." << endl;
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

void Player::addEXP(int exp) {
    int oldLV = getLV();
    GameCharactor::addEXP(exp);
    int newLV = getLV();
    if (newLV >= oldLV) {
        cout << "You levelled up! Your HP is full now." << endl;
        decreaseDamageTaken(getDamageTaken());
        updateSkills(true);
    }
}

void Player::updateSkills(bool verbose) {
    for (const auto &s : ALL_SKILLS) {
        if (s.lvRequirement <= getLV() && find(skills.begin(), skills.end(), s) == skills.end()) {
            if (verbose) cout << "You unlocked a new skill [" << s.name << "]." << endl;
            skills.push_back(s);
        }
    }
}
