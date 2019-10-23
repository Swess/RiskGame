#include "Map/MapDriver.h"
#include "Dice/DiceDriver.h"
#include "MapLoader/MapLoaderDriver.h"
#include "Cards/CardsDriver.h"
#include "Player/PlayerDriver.h"
#include "Terminal/Terminal.h"


using namespace std;


int main() {

    Terminal::print("Domination Game - Team 12 - COMP 345");

    Terminal::test_mode_on();
    Terminal::run_test("MapLoader", MapLoader::Driver::run);
    Terminal::run_test("Map", Map::Driver::run);
    Terminal::run_test("Dice", Dice::Driver::run);
    Terminal::run_test("Cards", Cards::Driver::run);
    Terminal::run_test("Player", Player::Driver::run);



    return 0;
}