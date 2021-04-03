#ifndef DUNGEON_PLAYER_H
#define DUNGEON_PLAYER_H

#include "GameCharactor.h"
#include "Room.h"
#include "json.hpp"
#include <iostream>
using nlohmann::json;

class Player : public GameCharactor {
public:
    Player(string name, const json& statsJson, int currRoomIdx, int prevRoomIdx);
    Player();
    int getCurrRoomIdx() const;
    int getPrevRoomIdx() const;
    void printStats() const;
    void setName(string name);
    void changeRoom(int newRoomIdx);
    virtual int getMaxHP() const override;
    virtual int getATK() const override;
    virtual int getDEF() const override;
    virtual bool triggerEvent(Object &object, Room &currRoom) override;
    virtual string getInteractMessage() const override;
    virtual string getTag() const override;
    virtual json getJson() const override;

private:
    int currRoomIdx;
    int prevRoomIdx;
};

#endif //DUNGEON_PLAYER_H
