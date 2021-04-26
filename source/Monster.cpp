#include "Monster.h"
#include "Player.h"
#include "Dungeon.h"

bool Monster::triggerEvent(Object &object) {
    Player& player = dynamic_cast<Player&>(object);

    vector<string> choices;
    string input;
    cout << "Choose attack type:" << endl;
    cout << Dungeon::CHOICE_START << "Normal attack (0, default)" << endl;
    choices.push_back("0");
    for (size_t i = 0; i < player.skills.size(); i++) {
        cout << Dungeon::CHOICE_START << player.skills[i].getBriefInfo();
        if (player.skills[i].CDCounter == 0) {
            cout << " (" << i + 1 << ")";
            choices.push_back(to_string(i + 1));
        }
        cout << endl;
    }
    input = Dungeon::getResponce(choices, true);
    Dungeon::clearConsole();

    for (auto &s : player.skills) {
        s.decreaseCD();
    }

    int damage;
    if (input == "0" || input == "") {
        damage = getCalculatedDamage(player.getPATK(), player.getMATK(), this->getPDEF(), this->getMDEF());
    } else {
        int skillIdx = stoi(input) - 1;
        Skill& skillRef = player.skills[skillIdx];
        skillRef.resetToMaxCD();
        damage = getCalculatedDamage(skillRef.PATK, skillRef.MATK, this->getPDEF(), this->getMDEF());
        if (skillRef.HP != 0) {
            int damageToDecrease = player.decreaseDamageTaken(skillRef.HP);
            cout << "You healed yourself by " << damageToDecrease << " HP." << endl;
        }
    }

    damage = min(damage, this->getHP());
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
            cout << "You got " << this->EXP << " EXP from [" << this->name << "]!" << endl;
            player.addEXP(this->EXP);
        }
        return true;
    } else {
        cout << *this << endl;
        damage = getCalculatedDamage(this->getPATK(), this->getMATK(), player.getPDEF(), player.getMDEF());
        damage = min(damage, player.getHP());
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
