#include "Skill.h"

Skill::Skill(string name, int lvRequirement, int CD, int PATK, int MATK, int HP)
    : name(name), lvRequirement(lvRequirement), CD(CD), PATK(PATK), MATK(MATK), HP(HP), CDCounter(0) {}

bool Skill::operator==(Skill otherSkill) {
    return name == otherSkill.name;
}

void Skill::decreaseCD() {
    CDCounter = max(0, CDCounter - 1);
}

void Skill::resetToMaxCD() {
    CDCounter = CD;
}
