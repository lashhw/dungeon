#ifndef DUNGEON_PLAYER_H
#define DUNGEON_PLAYER_H

#include "GameCharactor.h"
#include "Room.h"

#include <iostream>

class Player : public GameCharactor {
public:
    Player(string name, const json& statsJson, Room *currRoomPtr, Room *prevRoomPtr);
    void printStats() const;
    const Room * getCurrRoomPtr() const;
    const Room * getPrevRoomPtr() const;
    void setName(string name);
    void changeRoom(Room *newRoomPtr);
    void changeToPrevRoom();
    void addObjectPtrInRoom(shared_ptr<Object> newObjectPtr);
    void removeObjectInRoomByIdx(int objectIdxToDelete);
    virtual int getMaxHP() const override;
    virtual int getATK() const override;
    virtual int getDEF() const override;
    virtual bool triggerEvent(Object &object) override;
    virtual string getInteractMessage() const override;
    virtual string getTag() const override;
    virtual json getJson() const override;

private:
    Room *currRoomPtr;
    Room *prevRoomPtr;
};

#endif //DUNGEON_PLAYER_H
