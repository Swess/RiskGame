#include <iostream>
#include "Map/MapDriver.h"
#include "MapLoader/MapLoaderDriver.h"
#include "Cards/CardsDriver.h"

using namespace std;


int main() {
    cout << "Domination Game - Team 12 - COMP 345" << endl;

    MapLoader::Driver::run();
    Map::Driver::run();
    Cards::Driver::run();

    return 0;
}