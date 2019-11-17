//
// Created by NoahHorowitz on 11/16/2019.
//

#define private public


#include <cassert>
#include "GameStatisticsObserverDriver.h"
#include "../Terminal/Terminal.h"

namespace GameStatisticsObserver {
    namespace Driver{
        DriverState *DriverState::driver_state = nullptr;

        DriverState *GameStatisticsObserver::Driver::DriverState::get_instance() {
            if (driver_state == nullptr) {
                driver_state = new DriverState;
            }
            return driver_state;
        }

        Driver::DriverState::DriverState() {
            players_in_game = new vector<Player::Player *>;
            winner = nullptr;
            game_over = new bool(false);
            game_state_observer = nullptr;
        }

        void callback(Observer::GameStateSubject *model){
            DriverState *driver_state = DriverState::get_instance();
            driver_state->players_in_game = model->get_players_in_game();
            driver_state->winner = model->get_winner();
            *driver_state->game_over = model->is_game_over();
        }

        void before() {
            DriverState *driver_state = DriverState::get_instance();
            driver_state->gameInstance->start_test(18, 3); // RiskEurope, 2 players
            vector<Country *> countries = driver_state->gameInstance->get_map()->get_countries();
            driver_state->game_state_observer = new Observer::GameStateObserver(driver_state->gameInstance->game_state, callback);
            //reset player order
            for (auto player : *driver_state->gameInstance->get_players()) {
                Terminal::print(player->get_color() + " player:");
                for (auto country : player->get_countries()) {
                    Terminal::print("\t" + country->to_string());
                }
            }
            *driver_state->gameInstance->player_order = {0, 1, 2};
            //give all countries 3 armies
            for (auto country : countries) {
                country->set_armies(3);
            }
        }

        bool test_country_takeover() {
            DriverState *driver_state = DriverState::get_instance();
            vector<Country *> countries = driver_state->gameInstance->get_map()->get_countries();
            vector<Player::Player *> *players = driver_state->gameInstance->get_players();

            //prime observer


            //read number of countries of p1

            for (int i = 0; i < 15; i++) {
                players->at(0)->gain_control(countries.at(i));
                players->at(1)->gain_control(countries.at(i+15));
                players->at(2)->gain_control(countries.at(i+30));
            }
            driver_state->gameInstance->game_state->notify();
            // view sees player 1 with 15 countries

            for (auto player : *driver_state->players_in_game) {
                Terminal::print(player->get_color() + " player:");
                for (auto country : player->get_countries()) {
                    Terminal::print("\t" + country->to_string());
                }
            }

            assert(driver_state->players_in_game->at(0)->get_countries().size() == 15);

            countries.at(26)->set_armies(1);
            players->at(0)->dice = new Dice::TestDice(6);
            players->at(1)->dice = new Dice::TestDice(4);
            players->at(2)->dice = new Dice::TestDice(4);

            vector<int> input_vector = {1, 0, 1, 2, 2, 0};
            Terminal::set_input_vector(input_vector);
            players->at(0)->attack();
            Terminal::clear_terminal_input_counter();

            //view sees player 1 with 16 countries following successful attack
            assert(driver_state->players_in_game->at(0)->get_countries().size() == 16);

            return true;
        }

        bool test_elimination() {
            DriverState *driver_state = DriverState::get_instance();
            vector<Country *> countries = driver_state->gameInstance->get_map()->get_countries();
            vector<Player::Player *> *players = driver_state->gameInstance->get_players();

            //prime observer


            //read number of countries of p1

            for (int i = 0; i < 23; i++) {
                players->at(0)->gain_control(countries.at(i));
                players->at(1)->gain_control(countries.at(i+23));
            }
            players->at(2)->gain_control(countries.at(45));
            driver_state->gameInstance->game_state->notify();
            // view sees player 1 with 15 countries
            assert(driver_state->players_in_game->size() == 3);

            countries.at(26)->set_armies(1);
            players->at(0)->dice = new Dice::TestDice(6);
            players->at(1)->dice = new Dice::TestDice(1);
            players->at(2)->dice = new Dice::TestDice(1);

            vector<int> input_vector = {1, 8, 0, 2, 2, 2, 2, 0};
            Terminal::set_input_vector(input_vector);
            if (players->at(0)->attack()) driver_state->gameInstance->update_state();
            Terminal::clear_terminal_input_counter();

            //view sees player 1 with 16 countries following successful attack
            assert(driver_state->players_in_game->size() == 2);

            return true;
        }

        bool test_winner() {
            DriverState *driver_state = DriverState::get_instance();
            vector<Country *> countries = driver_state->gameInstance->get_map()->get_countries();
            vector<Player::Player *> *players = driver_state->gameInstance->get_players();

            //prime observer


            //read number of countries of p1

            for (int i = 0; i < 46; i++) {
                players->at(0)->gain_control(countries.at(i));
            }

            players->at(2)->gain_control(countries.at(45));
            driver_state->gameInstance->game_state->notify();
            // view sees player 1 with 15 countries
            assert(!*driver_state->game_over);

            countries.at(26)->set_armies(1);
            players->at(0)->dice = new Dice::TestDice(6);
            players->at(1)->dice = new Dice::TestDice(1);
            players->at(2)->dice = new Dice::TestDice(1);

            vector<int> input_vector = {1, 0, 0, 2, 2, 2, 2, 0};
            Terminal::set_input_vector(input_vector);
            players->at(0)->attack();
            Terminal::clear_terminal_input_counter();


            driver_state->gameInstance->is_game_done();
            //view sees player 1 with 16 countries following successful attack
            assert(*driver_state->game_over);

            return true;
        }


        bool run () {
            before();
            bool passed = (test_country_takeover() &&
                    test_elimination() &&
                    test_winner());
            DriverState::get_instance()->gameInstance->reset_test();
            return passed;
        }
    }
}