//
// Created by NohaHorowitz on 11/16/2019.
//

#include "vector"
#include "../Player/Player.h"
#include "../GameEngine/GameEngine.h"


namespace GameStatisticsObserver {
    namespace Driver {
        class DriverState {
        public:
            GameEngine::GameEngine *gameInstance = GameEngine::GameEngine::instance();
            Observer::GameStateObserver *game_state_observer;

            static DriverState *get_instance();
            static DriverState *driver_state;
            vector<Player::Player*> *players_in_game;
            Player::Player *winner;
            bool *game_over;

        private:
            explicit DriverState();

        };

        bool test_country_takeover();

        bool test_elimination();

        bool test_winner();

        bool run();
    }
}
