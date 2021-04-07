#include "NPC.h"
#include "Player.h"
#include "Dungeon.h"

#include <iostream>

bool NPC::triggerEvent(Object &object) {
    cout << "[" << name << "]: " <<  script << endl;

    vector<string> choices;
    if (!commodities.empty()) {
        int count = 0;
        for (auto &commodity : commodities) {
            count++;
            cout << Dungeon::CHOICE_START << commodity.second << " $" << commodity.first
                 << " (" << count << ")" << endl;
            choices.push_back(to_string(count));
        }
        cout << Dungeon::CHOICE_START << "I'll think about it. (x)" << endl;
        choices.push_back("x");

        Player &player = dynamic_cast<Player&>(object);
        cout << "What do you want to buy? ";
        string input = Dungeon::getResponce(choices);
        Dungeon::clearConsole();
        if (input != "x") {
            int commodityIdx = stoi(input) - 1;
            auto commodity = commodities[commodityIdx];
            if (player.getMoney() >= commodity.first) {
                player.addToInventory(commodity.second);
                player.setMoney(player.getMoney() - commodity.first);
                commodities.erase(commodities.begin() + commodityIdx);
                cout << "You bought [" << commodity.second.getName() << "]. "
                     << "You have $" << player.getMoney() << " left." << endl;
            } else {
                cout << "You don't have enough money!" << endl;
            }
        }
    }

    return false;
}

string NPC::getInteractMessage() const {
    return "NPC: talk to [" + name + "]";
}

NPC::NPC(string name, string script, const json &commoditiesJson):GameCharactor(name), script(script) {
    for (auto& commodityJson : commoditiesJson) {
        int price = commodityJson["price"];
        Item item(commodityJson["item"]["name"], commodityJson["item"]["properties"]);
        pair<int, Item> commodity = make_pair(price, item);
        commodities.push_back(commodity);
    }
}

string NPC::getTag() const {
    return "NPC";
}

json NPC::getJson() const {
    json j = GameCharactor::getJson();
    j["script"] = script;
    j["commodities"] = json::array();
    for (auto &commodity : commodities) {
        j["commodities"].push_back({{"price", commodity.first}, {"item", commodity.second.getJson()}});
    }
    return j;
}
