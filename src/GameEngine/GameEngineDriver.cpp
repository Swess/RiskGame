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

        bool run() {
            bool passed = (
                    map_loaded() &&
                    right_amount_of_player() &&
                    right_amount_of_card_in_deck() &&
                    mutex_country_to_players() &&
                    correct_country_players_assignation_count() &&
                    test_game_done() &&
                    test_game_loop() &&
                    test_tournament()
            );

            return passed;
        }

        bool map_loaded() {
            string map_name = "eurasien.map";
            GameEngine::instance()->start_test(map_name, 3);
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

        bool mutex_country_to_players() {
            string map_name = "eurasien.map";
            int nb_of_players = 4;
            GameEngine::instance()->start_test(map_name, nb_of_players);
            auto map = GameEngine::instance()->get_map();
            vector<Player::Player *> *loaded_players = GameEngine::instance()->get_players();

            // Simulate startup phase
            GameEngine::instance()->assign_player_order_randomly();
            GameEngine::instance()->assign_country_to_player();

            Terminal::set_input(1);
            GameEngine::instance()->assign_armies_into_country();

            // Simulate some exchange from player to player
            vector<Country *> countries = map->get_countries();
            loaded_players->at(1)->gain_control(countries.at(4));
            loaded_players->at(1)->gain_control(countries.at(20));
            loaded_players->at(1)->gain_control(countries.at(31));
            loaded_players->at(1)->gain_control(countries.at(40));
            loaded_players->at(1)->gain_control(countries.at(38));

            loaded_players->at(2)->gain_control(countries.at(4));
            loaded_players->at(2)->gain_control(countries.at(20));
            loaded_players->at(2)->gain_control(countries.at(38));
            loaded_players->at(2)->gain_control(countries.at(50));
            loaded_players->at(2)->gain_control(countries.at(1));

            loaded_players->at(3)->gain_control(countries.at(10));
            loaded_players->at(3)->gain_control(countries.at(11));
            loaded_players->at(3)->gain_control(countries.at(12));
            loaded_players->at(3)->gain_control(countries.at(13));
            loaded_players->at(3)->gain_control(countries.at(14));

            // Check mutex
            // Simple simple would test if it's working since each player has their list of countries
            int assigned_count = 0;
            for (auto & loaded_player : *loaded_players) {
                vector<Country *> player_countries = loaded_player->get_countries();
                assigned_count += player_countries.size();
            }

            assert(assigned_count == map->get_countries().size());

            GameEngine::instance()->reset_test();
            return true;
        }


        bool correct_country_players_assignation_count() {
            int nb_of_players = 4;
            GameEngine::instance()->start_test(1, nb_of_players);
            auto map = GameEngine::instance()->get_map();
            vector<Player::Player *> *loaded_players = GameEngine::instance()->get_players();


            // Simulate startup phase
            GameEngine::instance()->assign_player_order_randomly();
            GameEngine::instance()->assign_country_to_player();

            Terminal::set_input(3);
            GameEngine::instance()->assign_armies_into_country();

            for (auto & loaded_player : *loaded_players) {
                vector<Country *> countries = loaded_player->get_countries();
                int armies_count = 0;
                for (auto & country : countries)
                    armies_count += country->get_armies();

                assert(armies_count == 30);
            }

            GameEngine::instance()->reset_test();
            return true;
        }

        bool test_game_done() {
            GameEngine::instance()->start();
            GameEngine::instance()->startup_phase();
            vector<Country *> countries = GameEngine::instance()->get_map()->get_countries();
            GameEngine::instance()->get_players()->at(0)->gain_control(countries);
            GameEngine::instance()->game_loop();
            assert( GameEngine::instance()->is_game_done());
            GameEngine::instance()->reset_test();
            return true;
        }

        bool test_game_loop() {
            // TODO ?
            return true;
        }

        bool test_tournament(){
            Terminal::test_mode_on();
            auto *tournament_1 = new Tournament();
            vector<int> input_vector = {2, 2, 7, 12, 2, 2, 2, 12}; //testing with 2 maps, 2 games, 2 players with random behavior and 12 max turns
            Terminal::set_input_vector(input_vector);

            // (2) when the tournament mode is chosen, the user is asked to select
            tournament_1->prepareTournament(); // asks for num_maps, num_players, num_games, num_turns
            Terminal::clear_terminal_input_counter();
            // (3) after being started, the tournament runs without any user interaction
            tournament_1->start();
            // (4) upon completion, the results of the tournament are displayed as depicted above
            tournament_1->displayResults();

            //assert( = );

            //When finished, deleting everything

            GameEngine::instance()->game_state->clear();
            GameEngine::instance()->reset_test();
            return true;
        }
    }
};
