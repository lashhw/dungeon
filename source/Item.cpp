#include "Item.h"
#include "Player.h"

using nlohmann::json;

Item::Item(const string &name, const json &properties):Object(name) {
    PATK = MATK = PDEF = MDEF = HP = 0;
    type = properties["type"];
    for (auto &property : properties.items()) {
        if (property.key() == "PATK") PATK = property.value();
        else if (property.key() == "MATK") MATK = property.value();
        else if (property.key() == "PDEF") PDEF = property.value();
        else if (property.key() == "MDEF") MDEF = property.value();
        else if (property.key() == "HP") HP = property.value();
    }
}

bool Item::triggerEvent(Object &object) {
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

int Item::getPATK() const {
    return PATK;
}

int Item::getMATK() const {
    return MATK;
}

int Item::getPDEF() const {
    return PDEF;
}

int Item::getMDEF() const {
    return MDEF;
}

string Item::getInfo(bool withType) const {
    string info;
    info += "[" + name + "]";
    vector<pair<string, string>> propertiesInfo;
    if (HP != 0) {
        propertiesInfo.push_back(make_pair("HP", to_string(HP)));
    }
    if (PATK != 0) {
        propertiesInfo.push_back(make_pair("PATK", to_string(PATK)));
    }
    if (MATK != 0) {
        propertiesInfo.push_back(make_pair("MATK", to_string(MATK)));
    }
    if (PDEF != 0) {
        propertiesInfo.push_back(make_pair("PDEF", to_string(PDEF)));
    }
    if (MDEF != 0) {
        propertiesInfo.push_back(make_pair("MDEF", to_string(MDEF)));
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
    j["properties"]["PATK"] = PATK;
    j["properties"]["MATK"] = MATK;
    j["properties"]["PDEF"] = PDEF;
    j["properties"]["MDEF"] = MDEF;
    return j;
}
