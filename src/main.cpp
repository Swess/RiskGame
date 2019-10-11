#include <iostream>
#include "Map/MapDriver.h"
#include "Dice/DiceDriver.h"
#include "MapLoader/MapLoaderDriver.h"
#include "Cards/CardsDriver.h"
#include "Player/PlayerDriver.h"

using namespace std;


int main() {
    cout << "Domination Game - Team 12 - COMP 345" << endl;

    MapLoader::Driver::run();
    Map::Driver::run();
    Dice::Driver::run();
    Cards::Driver::run();
    Player::Driver::run();

    return 0;
}