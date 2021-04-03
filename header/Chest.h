#ifndef DUNGEON_CHEST_H
#define DUNGEON_CHEST_H

#include "Object.h"
#include "Room.h"
#include "Item.h"
#include <vector>

class Chest : public Object {
public:
    Chest(string name, const json &contentsJson);
    virtual bool triggerEvent(Object &object, Room &currRoom) override;
    virtual string getInteractMessage() const override;
    virtual string getTag() const override;
    virtual json getJson() const override;
private:
    vector<Item> contents;
};

#endif //DUNGEON_CHEST_H
