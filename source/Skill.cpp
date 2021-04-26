#include "Skill.h"
#include <vector>
#include <sstream>

Skill::Skill(string name, int lvRequirement, int CD, int PATK, int MATK, int HP)
    : name(name), lvRequirement(lvRequirement), CD(CD), PATK(PATK), MATK(MATK), HP(HP), CDCounter(0) {}

void Skill::decreaseCD() {
    CDCounter = max(0, CDCounter - 1);
}

void Skill::resetToMaxCD() {
    CDCounter = CD;
}

string Skill::getInfo() const {
    string info;
    info += "[" + name + "]";
    vector<pair<string, string>> propertiesInfo;
    if (PATK != 0) {
        propertiesInfo.push_back(make_pair("PATK", to_string(PATK)));
    }
    if (MATK != 0) {
        propertiesInfo.push_back(make_pair("MATK", to_string(MATK)));
    }
    if (HP != 0) {
        propertiesInfo.push_back(make_pair("HP", to_string(HP)));
    }
    info += " {";
    for (auto i = 0; i < propertiesInfo.size(); i++) {
        if (i != 0) info += ", ";
        info += propertiesInfo[i].first + ": " + propertiesInfo[i].second;
    }
    info += "}";
    return info;
}

bool operator==(const Skill &s1, const Skill &s2) {
    return s1.name == s2.name;
}

string Skill::getBriefInfo() const {
    stringstream ss;
    if (CDCounter == 0) {
        ss << "[READY]";
    } else {
        ss << "[CD: " << CDCounter << "]";
    }
    ss << " " << name;
    return ss.str();
}
