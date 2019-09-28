#include <iostream>
#include "Map/MapDriver.h"
//#include "PlayerDriver.cpp"
#include "src/MapLoader/MapLoaderDriver.cpp"

using namespace std;


int main() {
    cout << "Domination Game - Team 12 - COMP 345" << endl;

    MapLoaderDriver::run();
//    PlayerDriver.run();
//    cout << "Domination Game - Team 12 - COMP 345" << endl;

    Map::Driver::run();

    return 0;
}