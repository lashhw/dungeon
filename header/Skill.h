#ifndef DUNGEON_SKILL_H
#define DUNGEON_SKILL_H

#include <string>
using namespace std;

class Skill {
public:
    Skill(string name, int lvRequirement, int CD, int PATK = 0, int MATK = 0, int HP = 0);
    bool operator ==(Skill otherSkill);
    void decreaseCD();
    void resetToMaxCD();

    string name;
    int lvRequirement;
    int CD;
    int PATK;
    int MATK;
    int HP;
    int CDCounter;
};

#endif //DUNGEON_SKILL_H
