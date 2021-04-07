#include "Monster.h"
#include "Player.h"

#include <iostream>

bool Monster::triggerEvent(Object &object) {
    Player& player = dynamic_cast<Player&>(object);
    int damage = getCalculatedDamage(player.getATK(), this->getDEF());
    this->addDamageTaken(damage);
    cout << "You deal " << damage << " damage to [" << this->name << "]." << endl;
    if (this->getHP() <= 0) {
        cout << "["<< this->name << "] died." << endl;
        for (auto &item : this->equipped) {
            cout << "Item [" << item.second.getName() << "] has dropped." << endl;
            shared_ptr<Item> newItem = make_shared<Item>(item.second);
            player.addObjectPtrInRoom(newItem);
        }
        for (auto &item : this->unequipped) {
            cout << "Item [" << item.getName() << "] has dropped." << endl;
            shared_ptr<Item> newItem = make_shared<Item>(item);
            player.addObjectPtrInRoom(newItem);
        }
        if (this->money != 0) {
            cout << "You got $" << this->getMoney() << " from [" << this->name << "]!" << endl;
            player.setMoney(player.getMoney() + this->getMoney());
        }
        if (this->EXP != 0) {
            int oldLV = player.getLV();
            cout << "You got " << this->EXP << " EXP from [" << this->name << "]!" << endl;
            player.addEXP(this->EXP);
            int newLV = player.getLV();
            if (oldLV != newLV) {
                cout << "You levelled up! Your HP is full now." << endl;
                player.setDamageTaken(0);
            }
        }
        return true;
    } else {
        cout << *this << endl;
        damage = getCalculatedDamage(this->getATK(), player.getDEF());
        player.addDamageTaken(damage);
        cout << "[" << this->name << "] " << "deals " << damage << " damage to you." << endl;
        cout << player << endl;
        return false;
    }
}

string Monster::getInteractMessage() const {
    return "Monster: attack " + getBriefStats();
}

Monster::Monster(string name, const json &stats) : GameCharactor(name, stats) {}

string Monster::getTag() const {
    return "Monster";
}
