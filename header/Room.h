#ifndef DUNGEON_ROOM_H
#define DUNGEON_ROOM_H

#include "Object.h"

#include <vector>
#include <memory>

class Object; // forward declaration
class Room {
public:
    Room(int index, int upRoom, int downRoom, int leftRoom, int rightRoom);
    int getIndex() const;
    int getUpRoom() const;
    int getDownRoom() const;
    int getLeftRoom() const;
    int getRightRoom() const;
    const vector<shared_ptr<Object>> & getObjectPtrs() const;
    bool hasMonster() const;
    void printRoom(const Room& prevRoom) const;
    json getJson() const;
    void addObjectPtr(shared_ptr<Object> newObjectPtr);
    void removeObjectPtrByIdx(int objectPtrIdx);
private:
    int index;
    int upRoom;
    int downRoom;
    int leftRoom;
    int rightRoom;
    vector<shared_ptr<Object>> objectPtrs;
};

#endif //DUNGEON_ROOM_H
