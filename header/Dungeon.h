#ifndef DUNGEON_DUNGEON_H
#define DUNGEON_DUNGEON_H

#include "Room.h"
#include "Player.h"
#include "json.hpp"
#include <string>
#include <vector>
#include <unordered_map>
using namespace std;
using nlohmann::json;

enum gameState {win, lose, indeterminate};

class Dungeon {
public:
    static json getJsonFromFile(string fileName);
    static string getResponce(const vector<string> &choices);
    static string getResponce();
    static string getBoxedString(string unformatted, int width, int marginLeft, bool upBorder, bool bottomBorder);
    static void clearConsole();
    static const string CHOICE_START;
    static const json GAME_DATA;
    void startGame();
private:
    pair<string, vector<string>> getChoices() const;
    bool saveGame(string fileName) const;
    void quitGame() const;
    enum gameState checkGameLogic() const;
    void createNewGame();
    void createPlayer();
    void loadFile(string fileName);
    void loadJson(json gameDataJson);
    void handleAction(string actionKey);

    unordered_map<int, Room> rooms;
    Player player;
    int exitRoom;
};

#endif //DUNGEON_DUNGEON_H
