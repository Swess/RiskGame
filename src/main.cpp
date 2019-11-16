// TESTS INCLUDES
#include "Map/MapDriver.h"
#include "Dice/DiceDriver.h"
#include "MapLoader/MapLoaderDriver.h"
#include "GameEngine/GameEngineDriver.h"
#include "Cards/CardsDriver.h"
#include "Player/PlayerDriver.h"
#include "Player/PlayerStrategiesDriver.h"
#include "GameObservers/PlayerObserverDriver.h"
#include "Terminal/Terminal.h"
// RUN INCLUDES
#include "GameEngine/GameEngine.h"

using namespace std;

int main(int argc, const char*argv[]) {

    Terminal::print("Domination Game - Team 12 - COMP 345");

    bool test = false;

    for (int i = 0; i < argc; i++)
    {
        string string_test = "test";
        if (string_test == argv[i] ) test = true;
    }


    // Terminal::debug_mode_on();
    if (test) {
        Terminal::test_mode_on();
        Terminal::run_test("MapLoader", MapLoader::Driver::run);
        Terminal::run_test("Map", Board::Driver::run);
        Terminal::run_test("Dice", Dice::Driver::run);
        Terminal::run_test("Cards", Cards::Driver::run);
        Terminal::run_test("Player", Player::Driver::run);
        Terminal::run_test("GameEngine", GameEngine::Driver::run);
        Terminal::run_test("PlayerStrategies", PlayerStrategies::Driver::run);
        Terminal::test_mode_off();
    } else {
        auto gameInstance = GameEngine::GameEngine::instance();
        int debug_mode = Terminal::print_select("Do you want debug mode on?");
        if (debug_mode) Terminal::debug_mode_on();
        int automated_start = Terminal::print_select("Do you want an automated start set-up?");
        if (automated_start){
            int players = Terminal::print_select(2,6, "How many players?");
            Terminal::test_mode_on();
            gameInstance->start_test(7 , players);
            gameInstance->startup_phase();
            Terminal::test_mode_off();
        } else {
            gameInstance->start();
            gameInstance->startup_phase();
        }
        gameInstance->game_loop();
    }

    return 0;
}