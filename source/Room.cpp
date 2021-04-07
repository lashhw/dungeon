#include "Room.h"
#include "Monster.h"

#include <iostream>
#include <string>

Room::Room(int index, int upRoom, int downRoom, int leftRoom, int rightRoom)
    :index(index), upRoom(upRoom), downRoom(downRoom), leftRoom(leftRoom), rightRoom(rightRoom) {}

int Room::getUpRoom() const {
    return upRoom;
}

int Room::getDownRoom() const {
    return downRoom;
}

int Room::getLeftRoom() const {
    return leftRoom;
}

int Room::getRightRoom() const {
    return rightRoom;
}

const vector<shared_ptr<Object>> & Room::getObjectPtrs() const {
    return objectPtrs;
}

void Room::addObjectPtr(shared_ptr<Object> newObjectPtr) {
    objectPtrs.push_back(newObjectPtr);
}

void Room::removeObjectPtrByIdx(int objectPtrIdx) {
    objectPtrs.erase(objectPtrs.begin()+objectPtrIdx);
}

bool Room::hasMonster() const {
    for (auto &objectPtr : objectPtrs) {
        if (dynamic_pointer_cast<Monster>(objectPtr) != nullptr) return true;
    }
    return false;
}

void Room::printRoom(const Room& prevRoom) const {
    string roomStr[12];
    roomStr[0]  = R"~( +-----------------------------------+ )~";
    roomStr[1]  = R"~( |                                   | )~";
    roomStr[2]  = R"~( |                                   | )~";
    roomStr[3]  = R"~( |                                   | )~";
    roomStr[4]  = R"~( |               .--,                | )~";
    roomStr[5]  = R"~( |              ((''))               | )~";
    roomStr[6]  = R"~( |               /  \/               | )~";
    roomStr[7]  = R"~( |              /____\               | )~";
    roomStr[8]  = R"~( |                | \                | )~";
    roomStr[9]  = R"~( |               (You)               | )~";
    roomStr[10] = R"~( |                                   | )~";
    roomStr[11] = R"~( +-----------------------------------+ )~";
    if (upRoom != -1 && (!hasMonster() || prevRoom.getIndex() == upRoom)) {
        roomStr[0][17] = '\'';
        roomStr[0][18] = ' ';
        roomStr[0][19] = ' ';
        roomStr[0][20] = '\'';
    }
    if (downRoom != -1 && (!hasMonster() || prevRoom.getIndex() == downRoom)) {
        roomStr[11][17] = ',';
        roomStr[11][18] = ' ';
        roomStr[11][19] = ' ';
        roomStr[11][20] = ',';
    }
    if (leftRoom != -1 && (!hasMonster() || prevRoom.getIndex() == leftRoom)) {
        roomStr[4][0] = '_';
        roomStr[5][1] = ' ';
        roomStr[6][1] = ' ';
        roomStr[6][0] = '_';
    }
    if (rightRoom != -1 && (!hasMonster() || prevRoom.getIndex() == rightRoom)) {
        roomStr[4][38] = '_';
        roomStr[5][37] = ' ';
        roomStr[6][37] = ' ';
        roomStr[6][38] = '_';
    }
    for (auto i = 0; i < objectPtrs.size(); i++) {
        string tag = "(" + objectPtrs[i]->getTag() + ")";
        roomStr[2+i].replace(25, tag.size(), tag);
    }
    for (const auto &str : roomStr) {
        cout << str << endl;
    }
}

json Room::getJson() const {
    json j;
    j["index"] = index;
    j["linkedRoom"] = json::object();
    if (upRoom != -1) j["linkedRoom"]["up"] = upRoom;
    if (downRoom != -1) j["linkedRoom"]["down"] = downRoom;
    if (leftRoom != -1) j["linkedRoom"]["left"] = leftRoom;
    if (rightRoom != -1) j["linkedRoom"]["right"] = rightRoom;
    j["objects"] = json::array();
    for (auto &objectPtr : objectPtrs) {
        j["objects"].push_back(objectPtr->getJson());
    }
    return j;
}

int Room::getIndex() const {
    return index;
}
