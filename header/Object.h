#ifndef DUNGEON_OBJECT_H
#define DUNGEON_OBJECT_H

#include "json.hpp"
#include <string>

using namespace std;
using nlohmann::json;

class Object {
public:
    Object(string name);
    string getName() const;
    virtual bool triggerEvent(Object &object) = 0; // if return true, then you have to manually destroy the object
    virtual string getInteractMessage() const = 0;
    virtual string getTag() const = 0;
    virtual json getJson() const;
    virtual ~Object() = default;
protected:
    string name;
};

#endif //DUNGEON_OBJECT_H
