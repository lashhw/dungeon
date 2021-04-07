#ifndef DUNGEON_ITEM_H
#define DUNGEON_ITEM_H

#include "Object.h"

class Item : public Object {
public:
    Item(const string &name, const json &properties);
    int getHP() const;
    int getATK() const;
    int getDEF() const;
    string getType() const;
    string getInfo(bool withType = false) const;
    virtual bool triggerEvent(Object &object) override;
    virtual string getInteractMessage() const override;
    virtual string getTag() const override;
    virtual json getJson() const override;
    friend ostream& operator<<(ostream& out, Item item);

private:
    string type;
    int HP;
    int ATK;
    int DEF;
};

#endif //DUNGEON_ITEM_H