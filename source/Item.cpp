#include "Item.h"
#include "Player.h"
#include <iostream>

using nlohmann::json;

Item::Item(const string &name, const json &properties):Object(name) {
    ATK = DEF = HP = 0;
    type = properties["type"];
    for (auto &property : properties.items()) {
        if (property.key() == "ATK") ATK = property.value();
        else if (property.key() == "DEF") DEF = property.value();
        else if (property.key() == "HP") HP = property.value();
    }
}

bool Item::triggerEvent(Object &object, Room &currRoom) {
    Player& player = dynamic_cast<Player&>(object);
    player.addToInventory(*this);
    cout << "Item [" << name << "] has added into your inventory." << endl;
    return true;
}

string Item::getInteractMessage() const {
    return "Item: pick up " + getInfo();
}

ostream &operator<<(ostream &out, Item item) {
    out << item.getInfo();
    return out;
}

int Item::getHP() const {
    return HP;
}

int Item::getATK() const {
    return ATK;
}

int Item::getDEF() const {
    return DEF;
}

string Item::getInfo(bool withType) const {
    string info;
    info += "[" + name + "]";
    vector<pair<string, string>> propertiesInfo;
    if (HP != 0) {
        propertiesInfo.push_back(make_pair("HP", to_string(HP)));
    }
    if (ATK != 0) {
        propertiesInfo.push_back(make_pair("ATK", to_string(ATK)));
    }
    if (DEF != 0) {
        propertiesInfo.push_back(make_pair("DEF", to_string(DEF)));
    }
    if (withType) {
        propertiesInfo.push_back(make_pair("Type", type));
    }
    info += " {";
    for (auto i = 0; i < propertiesInfo.size(); i++) {
        if (i != 0) info += ", ";
        info += propertiesInfo[i].first + ": " + propertiesInfo[i].second;
    }
    info += "}";
    return info;
}

string Item::getTag() const {
    return "Item";
}

string Item::getType() const {
    return type;
}

json Item::getJson() const {
    json j = Object::getJson();
    j["properties"]["type"] = type;
    j["properties"]["HP"] = HP;
    j["properties"]["ATK"] = ATK;
    j["properties"]["DEF"] = DEF;
    return j;
}
