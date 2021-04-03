#include "Object.h"

Object::Object(string name):name(name) {}

string Object::getName() const {
    return name;
}

json Object::getJson() const {
    json j;
    string tag = this->getTag();
    transform(tag.begin(), tag.end(), tag.begin(), [](unsigned char c) { return tolower(c); });
    j["tag"] = tag;
    j["name"] = name;
    return j;
}

