#include <iostream>
#include "Map/MapDriver.h"
#include "DiceRoller/DiceDriver.h"
#include "MapLoader/MapLoaderDriver.h"


using namespace std;


int main() {
    cout << "Domination Game - Team 12 - COMP 345" << endl;

    MapLoader::Driver::run();
    Map::Driver::run();
    DiceDriver::run();

    return 0;
}