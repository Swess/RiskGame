//
// Created by NoahHorowitz on 11/15/2019.
//
#pragma once

#include <vector>
#include "../Map/Map.h"
#include "../GameEngine/GameEngine.h"

namespace PlayerObserver {
    namespace Driver {
        class DriverState {
        public:
            static DriverState *get_instance();


            GameEngine::GameEngine *gameInstance = GameEngine::GameEngine::instance();
            Player::Player *p1;
            Player::Player *p2;
            Observer::PlayerObserver *playerObserver1;
            Observer::PlayerObserver *playerObserver2;

            //
            string current_phase;
            Country *source_country;
            Country *target_country;
            int number_of_armies;
            int *battle_number;
            bool *success;
            int number_armies_gained_from_countries;
            int number_armies_gained_from_continent;
            int number_armies_gained_from_exchange;
            vector<pair<int, Country *> *> *reinforce_pair_vector;
            static DriverState *driver_state;


        private:
            explicit DriverState();

        };

        bool change_phase();

        bool test_attack();

        bool test_reinforce();

        bool test_fortify();

        bool run();
    }
}
