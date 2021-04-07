#include "Dungeon.h"
#include "Monster.h"
#include "Chest.h"
#include "NPC.h"
#include "trim.hpp"
#include <fstream>
#include <iostream>
#include <iomanip>

using namespace std;
using json = nlohmann::json;

const string Dungeon::CHOICE_START = " - ";
const json Dungeon::GAME_DATA = R"(
{"player":{"name":"unnamed player","stats":{"baseHP":100,"baseATK":10,"baseDEF":10,"money":0,"EXP":0},"equipped":[],"unequipped":[],"currRoom":0,"prevRoom":0},"exitRoom":16,"rooms":[{"index":0,"linkedRoom":{"right":1},"objects":[{"tag":"npc","name":"Voice-over","script":"Welcome to dungeon. After the war, the world has become so chaotic. Your mission is to lead the humanity to find a way out of the cellar.","commodities":[]}]},{"index":1,"linkedRoom":{"left":0,"right":7,"down":2},"objects":[{"tag":"chest","name":"Broken Chest","contents":[{"name":"Leather Tunic","properties":{"type":"chestplate","HP":10,"DEF":10}},{"name":"Worn Crossbow","properties":{"type":"weapon","ATK":5}}]}]},{"index":2,"linkedRoom":{"up":1,"down":3},"objects":[{"tag":"npc","name":"Weapon Merchant","script":"I sell only quality weapons.","commodities":[{"price":50,"item":{"name":"Elven Bow","properties":{"type":"weapon","ATK":33}}}]}]},{"index":3,"linkedRoom":{"up":2,"right":4},"objects":[{"tag":"monster","name":"Slime","stats":{"baseHP":15,"baseATK":3,"baseDEF":0,"money":7,"EXP":33}},{"tag":"monster","name":"Slime","stats":{"baseHP":15,"baseATK":3,"baseDEF":0,"money":7,"EXP":33}},{"tag":"monster","name":"Slime","stats":{"baseHP":15,"baseATK":3,"baseDEF":0,"money":7,"EXP":33}}]},{"index":4,"linkedRoom":{"left":3,"up":5,"right":6},"objects":[{"tag":"item","name":"Priest Wand","properties":{"type":"weapon","ATK":12}}]},{"index":5,"linkedRoom":{"down":4},"objects":[{"tag":"monster","name":"Wolf","stats":{"baseATK":13,"baseDEF":3,"baseHP":20,"money":20,"EXP":77}}]},{"index":6,"linkedRoom":{"left":4},"objects":[{"tag":"npc","name":"unnamed_npc","script":"Hello World.","commodities":[]}]},{"index":7,"linkedRoom":{"left":1,"right":8},"objects":[{"tag":"monster","name":"Fire Dragon","stats":{"baseATK":33,"baseDEF":20,"baseHP":100,"money":97,"EXP":120},"equipped":[{"name":"Iron Chestplate","properties":{"type":"chestplate","baseDEF":17,"baseHP":33}}]}]},{"index":8,"linkedRoom":{"up":12,"right":9,"left":7},"objects":[{"tag":"npc","name":"a young boy","script":"I am so afraid... Can you lead me out of here?","commodities":[]},{"tag":"chest","name":"Chest","contents":[{"name":"Spiked Shield","properties":{"type":"shield","DEF":36,"ATK":15}},{"name":"Mortorcycle Helmet","properties":{"type":"helmet","DEF":23}}]}]},{"index":9,"linkedRoom":{"left":8,"down":10},"objects":[{"tag":"npc","name":"Merchant","script":"","commodities":[{"price":150,"item":{"name":"pointer","properties":{"type":"weapon","ATK":52}}},{"price":50,"item":{"name":"Medical Kit","properties":{"type":"usable","HP":100}}}]}]},{"index":10,"linkedRoom":{"up":9,"down":11},"objects":[{"tag":"monster","name":"virtual function","stats":{"baseATK":1,"baseDEF":0,"baseHP":128,"EXP":2048}}]},{"index":11,"linkedRoom":{"up":10},"objects":[{"tag":"chest","name":"Luxurious Chest","contents":[{"name":"Diamond leggings","properties":{"type":"leggings","HP":62,"DEF":26}},{"name":"Archer's Boots","properties":{"type":"boots","HP":21,"DEF":6}}]}]},{"index":12,"linkedRoom":{"left":13,"down":8},"objects":[{"tag":"monster","name":"d/dx","stats":{"baseATK":27,"baseHP":111,"baseDEF":12,"money":500,"EXP":987},"equipped":[{"name":"Chain Helmet","properties":{"type":"helmet","DEF":20,"HP":67}}]}]},{"index":13,"linkedRoom":{"right":12,"left":14},"objects":[{"tag":"monster","name":"Abstract class","stats":{"baseATK":0,"baseHP":1,"baseDEF":0,"money":1,"EXP":1}}]},{"index":14,"linkedRoom":{"right":13,"up":15},"objects":[]},{"index":15,"linkedRoom":{"left":16,"down":14},"objects":[{"tag":"monster","name":"BOSS: OOP assignment","stats":{"baseATK":123,"baseHP":1000,"baseDEF":37,"EXP":1000,"money":1000}}]},{"index":16,"linkedRoom":{"right":15},"objects":[]}]}
)"_json;

json Dungeon::getJsonFromFile(string fileName) {
    ifstream file(fileName);
    if(file.fail()) {
        cerr << R"(Unable to read ")" << fileName << R"(". Exiting...)" << endl;
        exit(EXIT_FAILURE);
    }
    json js;
    file >> js;
    file.close();
    return js;
}

string Dungeon::getResponce(const vector<string> &choices) {
    for (;;) {
        cout << "(";
        for (int i = 0; i < choices.size(); i++) {
            if (i != 0) cout << "/";
            cout << choices[i];
        }
        cout << "): ";

        string input;
        getline(cin, input);
        trim(input);
        if (find(choices.begin(), choices.end(), input) != choices.end())
            return input;

        cout << "Wrong Input! ";
    }
}

string Dungeon::getResponce() {
    string input;
    getline(cin, input);
    trim(input);
    return input;
}

void Dungeon::startGame() {
    cout << R"( oooooooooo.                                                                      )" << endl;
    cout << R"( `888'   `Y8b                                                                     )" << endl;
    cout << R"(  888      888 oooo  oooo  ooo. .oo.    .oooooooo  .ooooo.   .ooooo.  ooo. .oo.   )" << endl;
    cout << R"(  888      888 `888  `888  `888P"Y88b  888' `88b  d88' `88b d88' `88b `888P"Y88b  )" << endl;
    cout << R"(  888      888  888   888   888   888  888   888  888ooo888 888   888  888   888  )" << endl;
    cout << R"(  888     d88'  888   888   888   888  `88bod8P'  888    .o 888   888  888   888  )" << endl;
    cout << R"( o888bood8P'    `V88V"V8P' o888o o888o `8oooooo.  `Y8bod8P' `Y8bod8P' o888o o888o )" << endl;
    cout << R"(                                       d"     YD                                  )" << endl;
    cout << R"(                                       "Y88888P'                                  )" << endl;
    cout << R"( ============================== Welcome To Dungeon! ==============================)" << endl;

    cout << "Load previous data? ";
    string input = getResponce({"y", "n"});
    if (input == "y") {
        cout << "Enter filename: (default: saved.json): ";
        string fileName;
        fileName = getResponce();
        if (fileName == "") fileName = "saved.json";
        if (ifstream(fileName).good()) {
            loadFile(fileName);
            cout << R"(")" << fileName << R"(" loaded.)" << endl;
        } else {
            cout << R"(Failed to open ")" << fileName << R"(")" << endl;
            createNewGame();
        }
    } else if (input == "n") {
        createNewGame();
    }

    clearConsole();
    cout << "Welcome [" << player.getName() << "]!" << endl;
    while (checkGameLogic() == indeterminate) {
        player.getCurrRoomPtr()->printRoom(*player.getPrevRoomPtr());
        auto choices = getChoices();
        cout << choices.first;
        cout << "What do you want to do? ";
        input = getResponce(choices.second);
        clearConsole();
        handleAction(input);
    }
    if (checkGameLogic() == win) {
        cout << R"( ____   ____.__        __                        )" << endl;
        cout << R"( \   \ /   /|__| _____/  |_  ___________ ___.__. )" << endl;
        cout << R"(  \   Y   / |  |/ ___\   __\/  _ \_  __ <   |  | )" << endl;
        cout << R"(   \     /  |  \  \___|  | (  <_> )  | \/\___  | )" << endl;
        cout << R"(    \___/   |__|\___  >__|  \____/|__|   / ____| )" << endl;
        cout << R"(                    \/                   \/      )" << endl;
    } else if (checkGameLogic() == lose) {
        cout << R"(             /`._      ,                         )" << endl;
        cout << R"(            /     \   / \                        )" << endl;
        cout << R"(            ) ,-==-> /\/ \                       )" << endl;
        cout << R"(             )__\\/ // \  |                      )" << endl;
        cout << R"(            /  /' \//   | |                      )" << endl;
        cout << R"(           /  (  /|/    | /                      )" << endl;
        cout << R"(          /     //|    /,'                       )" << endl;
        cout << R"(         // /  (( )    '                         )" << endl;
        cout << R"(        //     // \    |                         )" << endl;
        cout << R"(       //     (#) |                              )" << endl;
        cout << R"(      /        )\/ \   '       ____              )" << endl;
        cout << R"(     /        /#/   )         /,.__\__,,--=_,    )" << endl;
        cout << R"(    /         \#\  /)      __/ + \____,--==<     )" << endl;
        cout << R"(    //gnv_____/#/_/'      (\_\__+/_,   ---<^     )" << endl;
        cout << R"(                                   '==--=='      )" << endl;
        cout << R"(                 You Died.                       )" << endl;
    }
}

void Dungeon::createNewGame() {
    cout << "Starting a new game..." << endl;
    loadJson(GAME_DATA);
    createPlayer();
}

void Dungeon::createPlayer() {
    cout << "Enter your name: ";
    string playerName = getResponce();
    player.setName(playerName);
}

enum gameState Dungeon::checkGameLogic() const {
    if (player.getCurrRoomPtr()->getIndex() == exitRoom) return win;
    if (player.getHP() <= 0) return lose;
    else return indeterminate;
}

pair<string, vector<string>> Dungeon::getChoices() const {
    const Room *currRoomPtr = player.getCurrRoomPtr();
    int upRoom = currRoomPtr->getUpRoom();
    int downRoom = currRoomPtr->getDownRoom();
    int leftRoom = currRoomPtr->getLeftRoom();
    int rightRoom = currRoomPtr->getRightRoom();
    auto objectPtrs = currRoomPtr->getObjectPtrs();
    bool hasMonster = currRoomPtr->hasMonster();

    string message;

    vector<string> choices;
    if (!hasMonster) {
        if (upRoom != -1) {
            choices.push_back("w");
            message += CHOICE_START + "Move up (w)\n";
        }
        if (downRoom != -1) {
            choices.push_back("s");
            message += CHOICE_START + "Move down (s)\n";
        }
        if (leftRoom != -1) {
            choices.push_back("a");
            message += CHOICE_START + "Move left (a)\n";
        }
        if (rightRoom != -1) {
            choices.push_back("d");
            message += CHOICE_START + "Move right (d)\n";
        }
    } else {
        choices.push_back("q");
        message += CHOICE_START + "Retreat (q)\n";
    }

    choices.push_back("e");
    message += CHOICE_START + "Print player's stats / Change equipment (e)\n";

    choices.push_back("Q");
    message += CHOICE_START + "Quit game (Q)\n";

    int choiceCount = 0;
    for (auto objectPtr : objectPtrs) {
        choiceCount++;
        string choiceKey = to_string(choiceCount);
        choices.push_back(choiceKey);
        message += CHOICE_START + objectPtr->getInteractMessage() + " ";
        message += "(" + choiceKey + ")\n";
    }
    return make_pair(message, choices);
}

void Dungeon::handleAction(string actionKey) {
    const Room* currRoomPtr = player.getCurrRoomPtr();
    if (actionKey == "w") {
        player.changeRoom(&rooms.find(currRoomPtr->getUpRoom())->second);
    } else if (actionKey == "s") {
        player.changeRoom(&rooms.find(currRoomPtr->getDownRoom())->second);
    } else if (actionKey == "a") {
        player.changeRoom(&rooms.find(currRoomPtr->getLeftRoom())->second);
    } else if (actionKey == "d") {
        player.changeRoom(&rooms.find(currRoomPtr->getRightRoom())->second);
    } else if (actionKey == "q") {
        player.changeToPrevRoom();
    } else if (actionKey == "e") {
        player.triggerEvent(player);
    } else if(actionKey == "Q") {
        quitGame();
    } else {
        int objectIdx = stoi(actionKey)-1;
        shared_ptr<Object> objectPtr = currRoomPtr->getObjectPtrs()[objectIdx];
        bool removeObject = objectPtr->triggerEvent(player);
        if (removeObject) player.removeObjectInRoomByIdx(objectIdx);
    }
}

string Dungeon::getBoxedString(string unformatted, int width, int marginLeft, bool upBorder, bool bottomBorder) {
    stringstream unformattedSS(unformatted);
    stringstream formattedSS;
    if (upBorder) formattedSS << '+' << string(width, '-') << '+' << endl;
    string token;
    while (getline(unformattedSS, token)) {
        formattedSS << '|' << string(marginLeft, ' ');
        formattedSS << setw(width-marginLeft) << left << token;
        formattedSS << '|' << endl;
    }
    if (bottomBorder) formattedSS << '+' << string(width, '-') << '+';

    return formattedSS.str();
}

bool Dungeon::saveGame(string fileName) const {
    json savedJson;
    savedJson["player"] = player.getJson();
    savedJson["exitRoom"] = exitRoom;
    for (auto &room : rooms) {
        savedJson["rooms"].push_back(room.second.getJson());
    }

    ofstream file(fileName);
    if (!file.good()) return false;
    file << savedJson.dump(4);
    file.close();
    return true;
}

void Dungeon::quitGame() const {
    cout << CHOICE_START << "Save game (s)" << endl;
    cout << CHOICE_START << "Quit without saving (Q)" << endl;
    cout << "What do you want to do? ";
    string input = getResponce({"s", "Q"});
    if (input == "s") {
        while(1) {
            cout << "Enter filename to save (default: saved.json): ";
            string fileName;
            fileName = getResponce();
            if (fileName == "") fileName = "saved.json";
            cout << "Saving... ";
            if(saveGame(fileName)) {
                cout << "Finished!" << endl;
                cout << R"(File saved as ")" << fileName << R"(")";
                break;
            } else {
                cout << "Failed." << endl;
            }
        }
        exit(EXIT_SUCCESS);
    } else if (input == "Q") {
        exit(EXIT_SUCCESS);
    }
}

void Dungeon::loadJson(json gameDataJson) {
    for (const auto &roomJson : gameDataJson["rooms"]) {
        int upRoom, downRoom, leftRoom, rightRoom;
        upRoom = downRoom = leftRoom = rightRoom = -1;
        int roomIdx = roomJson["index"];

        json linkedRoom = roomJson["linkedRoom"];
        if (linkedRoom.find("up") != linkedRoom.end())
            upRoom = linkedRoom["up"];
        if (linkedRoom.find("down") != linkedRoom.end())
            downRoom = linkedRoom["down"];
        if (linkedRoom.find("left") != linkedRoom.end())
            leftRoom = linkedRoom["left"];
        if (linkedRoom.find("right") != linkedRoom.end())
            rightRoom = linkedRoom["right"];
        rooms.insert(pair<int, Room>(roomIdx, Room(roomIdx, upRoom, downRoom, leftRoom, rightRoom)));

        for (const auto& objectJson : roomJson["objects"]) {
            string objectTag = objectJson["tag"];
            if (objectTag == "chest") {
                shared_ptr<Chest> chestPtr = make_shared<Chest>(objectJson["name"], objectJson["contents"]);
                rooms.find(roomIdx)->second.addObjectPtr(chestPtr);
            } else if (objectTag == "item") {
                shared_ptr<Item> itemPtr = make_shared<Item>(objectJson["name"], objectJson["properties"]);
                rooms.find(roomIdx)->second.addObjectPtr(itemPtr);
            } else if (objectTag == "monster") {
                shared_ptr<Monster> monsterPtr = make_shared<Monster>(objectJson["name"], objectJson["stats"]);
                rooms.find(roomIdx)->second.addObjectPtr(monsterPtr);
            } else if (objectTag == "npc") {
                shared_ptr<NPC> NPCPtr = make_shared<NPC>(objectJson["name"], objectJson["script"],
                                                          objectJson["commodities"]);
                rooms.find(roomIdx)->second.addObjectPtr(NPCPtr);
            }
        }
    }

    json playerJson = gameDataJson["player"];
    player = Player(playerJson["name"], playerJson["stats"],
                    &rooms.find(playerJson["currRoom"])->second, &rooms.find(playerJson["prevRoom"])->second);

    exitRoom = gameDataJson["exitRoom"];
}

void Dungeon::loadFile(string fileName) {
    json gameDataJson = getJsonFromFile(fileName);
    loadJson(gameDataJson);
}

void Dungeon::clearConsole() {
    cout << string(50, '\n') << flush;
}

Dungeon::Dungeon():
    player("null", {}, nullptr, nullptr) {
    rooms.reserve(1000); // maximum 1000 rooms
}
