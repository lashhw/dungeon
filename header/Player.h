#ifndef DUNGEON_PLAYER_H
#define DUNGEON_PLAYER_H

#include "GameCharactor.h"
#include "Room.h"
#include "Skill.h"

#include <iostream>

class Player : public GameCharactor {
public:
    static const vector<Skill> ALL_SKILLS;
    Player(string name, const json& statsJson, Room *currRoomPtr, Room *prevRoomPtr);
    void printStats() const;
    const Room * getCurrRoomPtr() const;
    const Room * getPrevRoomPtr() const;
    void setName(string name);
    void changeRoom(Room *newRoomPtr);
    void changeToPrevRoom();
    void addObjectPtrInRoom(shared_ptr<Object> newObjectPtr);
    void removeObjectInRoomByIdx(int objectIdxToDelete);
    void updateSkills(bool verbose = false);
    virtual void addEXP(int exp);
    virtual int getMaxHP() const override;
    virtual int getPATK() const override;
    virtual int getMATK() const override;
    virtual int getPDEF() const override;
    virtual int getMDEF() const override;
    virtual bool triggerEvent(Object &object) override;
    virtual string getInteractMessage() const override;
    virtual string getTag() const override;
    virtual json getJson() const override;
    vector<Skill> skills;

private:
    Room *currRoomPtr;
    Room *prevRoomPtr;
};

#endif //DUNGEON_PLAYER_H
