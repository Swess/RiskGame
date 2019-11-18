// TESTS INCLUDES
#include <sstream>
#include "Map/MapDriver.h"
#include "Dice/DiceDriver.h"
#include "MapLoader/MapLoaderDriver.h"
#include "GameEngine/GameEngineDriver.h"
#include "Cards/CardsDriver.h"
#include "Player/PlayerDriver.h"
#include "Player/PlayerStrategiesDriver.h"
#include "GameObservers/PlayerObserverDriver.h"
#include "GameObservers/GameStatisticsObserverDriver.h"

#include "Terminal/Terminal.h"
// RUN INCLUDES
#include "GameEngine/GameEngine.h"

using namespace std;

void game_state_callback(Observer::GameStateSubject *game_state);

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
        Terminal::run_test("PlayerObservers", PlayerObserver::Driver::run);
        Terminal::run_test("GameStatisticsObserver", GameStatisticsObserver::Driver::run);

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
        auto *game_state_observer = new Observer::GameStateObserver(gameInstance->game_state, game_state_callback);
        gameInstance->game_loop();
    }

    return 0;
}

void game_state_callback(Observer::GameStateSubject *game_state) {
//    vector<Player::Player *> *players = GameEngine::instance()->get_players();
    ostringstream os;
    os << "////////////////////////////////////////////////" << endl;
    os << "players world domination view" << endl;
    os << endl;
    if (!game_state->is_game_over()) {
        for (auto player : *game_state->get_players_in_game()) {
            if (!player->is_player_dead()) {
                os << player->get_color() << " player owns " << player->get_countries().size() << " countries" << endl;
            }
        }
    } else {

        os << "Congratulations " << game_state->get_winner() << ", you win!" << endl;
    }
    os << "////////////////////////////////////////////////" << endl;
    Terminal::print(os.str());
}