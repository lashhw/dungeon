#include "Chest.h"
#include "Player.h"

Chest::Chest(string name, const json &contentsJson):Object(name) {
    for (auto& itemJson :contentsJson) {
        Item item(itemJson["name"], itemJson["properties"]);
        contents.push_back(item);
    }
}

bool Chest::triggerEvent(Object &object, Room &currRoom) {
    cout << "You opened [" << name << "]." << endl;
    for (auto &item : contents) {
        currRoom.addObjectPtr(make_shared<Item>(item));
    }
    return true;
}

string Chest::getInteractMessage() const {
    return "Chest: open [" + name + "]";
}

string Chest::getTag() const {
    return "Chest";
}

json Chest::getJson() const {
    json j = Object::getJson();
    j["contents"] = json::array();
    for (auto &item : contents) {
        j["contents"].push_back(item.getJson());
    }
    return j;
}

