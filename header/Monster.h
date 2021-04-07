#ifndef DUNGEON_MONSTER_H
#define DUNGEON_MONSTER_H

#include "GameCharactor.h"

class Monster : public GameCharactor {
public:
    Monster(string name, const json &stats);
    virtual bool triggerEvent(Object &object) override;
    virtual string getInteractMessage() const override;
    virtual string getTag() const override;
};

#endif //DUNGEON_MONSTER_H
