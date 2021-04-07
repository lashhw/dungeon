#ifndef DUNGEON_NPC_H
#define DUNGEON_NPC_H

#include "GameCharactor.h"

class NPC : public GameCharactor {
public:
    NPC(string name, string script, const json &commoditiesJson);
    virtual bool triggerEvent(Object &object) override;
    virtual string getInteractMessage() const override;
    virtual string getTag() const override;
    virtual json getJson() const override;

private:
    string script;
    vector<pair<int, Item>> commodities; // price, Item
};

#endif //DUNGEON_NPC_H
