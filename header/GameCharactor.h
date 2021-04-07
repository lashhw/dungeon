#ifndef DUNGEON_GAMECHARACTOR_H
#define DUNGEON_GAMECHARACTOR_H

#include "Object.h"
#include "Item.h"
#include <vector>
#include <unordered_map>

class GameCharactor : public Object {
public:
    static const vector<string> EQUIP_TYPES;
    static int getCalculatedDamage(int ATK, int DEF);
    static string getBar(int num, int maxNum);
    static int getLVRequirement(int lv);
    int getHP() const;
    int getLV() const;
    int getMoney() const;
    string getBriefStats() const;
    void addDamageTaken(int damage);
    void setDamageTaken(int damage);
    void addEXP(int exp);
    void setMoney(int newMoney);
    void addToInventory(const Item &newItem);
    virtual int getMaxHP() const;
    virtual int getATK() const;
    virtual int getDEF() const;
    virtual json getJson() const override;
    friend ostream& operator<<(ostream& out, GameCharactor &gameCharactor);
protected:
    GameCharactor(string name, const json& statsJson);
    GameCharactor(string name);

    int baseHP;
    int baseATK;
    int baseDEF;
    int damageTaken;
    int money;
    int EXP;

    unordered_map<string, Item> equipped; // typeName -> item
    vector<Item> unequipped;
};

#endif //DUNGEON_GAMECHARACTOR_H
