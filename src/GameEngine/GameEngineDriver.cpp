//
// Created by ker10 on 2019-10-23.
//
#include <vector>
#include <string>
#include <cassert>
#include "GameEngineDriver.h"
#include "GameEngine.h"
#include "../Terminal/Terminal.h"

using namespace std;

namespace GameEngine {
    namespace Driver {

        bool run(){
            bool passed =   map_loaded() &&
                            right_amount_of_player() &&
                            right_amount_of_card_in_deck();

            return passed;
        }

        bool map_loaded() {
            GameEngine::instance()->start_test(1,3);
            auto map = GameEngine::instance()->get_map();
            assert(map->is_connected());
            assert(map->get_countries().at(0)->get_name() == "Portugal");
            assert(map->get_countries().size() == 52);
            GameEngine::instance()->reset_test();
            return true;
        }

        bool right_amount_of_player() {
            int nb_of_players = 3;
            GameEngine::instance()->start_test(0, nb_of_players);
            auto loaded_players = GameEngine::instance()->get_players()->size();
            assert(loaded_players == nb_of_players);
            GameEngine::instance()->reset_test();
            return true;
        }

        bool right_amount_of_card_in_deck() {
            GameEngine::instance()->start_test(0, 3);
            auto deck_size = GameEngine::instance()->get_deck()->get_size();
            auto country_size = GameEngine::instance()->get_map()->get_countries().size();
            assert(country_size == deck_size);
            GameEngine::instance()->reset_test();
            return true;
        }
    }
};
