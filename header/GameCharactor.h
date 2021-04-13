#ifndef DUNGEON_GAMECHARACTOR_H
#define DUNGEON_GAMECHARACTOR_H

#include "Item.h"

#include <vector>
#include <unordered_map>

class GameCharactor : public Object {
public:
    static const vector<string> EQUIP_TYPES;
    static int getCalculatedDamage(int PATK, int MATK, int PDEF, int MDEF);
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
    virtual int getPATK() const;
    virtual int getMATK() const;
    virtual int getPDEF() const;
    virtual int getMDEF() const;
    virtual json getJson() const override;
    friend ostream& operator<<(ostream& out, GameCharactor &gameCharactor);
protected:
    GameCharactor(string name, const json& statsJson);
    GameCharactor(string name);

    int baseHP;
    int basePATK;
    int baseMATK;
    int basePDEF;
    int baseMDEF;
    int damageTaken;
    int money;
    int EXP;

    unordered_map<string, Item> equipped; // typeName -> item
    vector<Item> unequipped;
};

#endif //DUNGEON_GAMECHARACTOR_H
