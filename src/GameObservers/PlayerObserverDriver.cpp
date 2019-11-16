//
// Created by NoahHorowitz on 11/15/2019.
//
#define private public

#include <cassert>
#include "PlayerObserverDriver.h"
#include "../Terminal/Terminal.h"

namespace PlayerObserver {
    namespace Driver {
        DriverState *DriverState::driver_state = nullptr;

        DriverState::DriverState() {
            gameInstance = GameEngine::GameEngine::instance();
            p1 = nullptr;
            p2 = nullptr;
            playerObserver1 = nullptr;
            playerObserver2 = nullptr;

            current_phase = "";
            source_country = nullptr;
            target_country = nullptr;
            number_of_armies = -1;
            battle_number = nullptr;
            success = nullptr;
            number_armies_gained_from_countries = -1;
            number_armies_gained_from_continent = -1;
            number_armies_gained_from_exchange = -1;
            reinforce_pair_vector = nullptr;
        }

        DriverState *DriverState::get_instance() {
            if (driver_state == nullptr) {
                driver_state = new DriverState();
            }
            return driver_state;
        }

        void callback(Observer::PlayerSubject *player) {
            DriverState *driver_state = DriverState::get_instance();
            driver_state->current_phase = player->get_phase_string();
            driver_state->source_country = player->get_source_country();
            driver_state->target_country = player->get_target_country();
            driver_state->number_of_armies = player->get_number_armies_used();
            driver_state->success = player->get_success();
            driver_state->number_armies_gained_from_continent = player->get_armies_gained_by_continent_owned();
            driver_state->number_armies_gained_from_countries = player->get_armies_gained_by_country_owned();
            driver_state->number_armies_gained_from_exchange = player->get_armies_gained_by_exchange();
            driver_state->reinforce_pair_vector = player->get_reinforcement_vector();
        }

        void before() {
            DriverState *driver_state = DriverState::get_instance();
            driver_state->gameInstance->start_test(18, 2); // RiskEurope, 2 players
            vector<Country *> countries = driver_state->gameInstance->get_map()->get_countries();
            //give all countries 2 armies
            for (auto country : countries) {
                country->set_armies(2);
            }

            driver_state->p1 = driver_state->gameInstance->get_players()->at(0);
            driver_state->p2 = driver_state->gameInstance->get_players()->at(1);

            driver_state->playerObserver1 = new Observer::PlayerObserver(driver_state->p1, callback);
            driver_state->playerObserver2 = new Observer::PlayerObserver(driver_state->p2, callback);
            driver_state->p1->gain_control(countries.at(0));
            driver_state->p1->gain_control(countries.at(1));
            driver_state->p1->gain_control(countries.at(2));
            driver_state->p1->gain_control(countries.at(3));

            driver_state->p2->gain_control(countries.at(26));

        }

        bool change_phase(){
            //perform fortification to set phase
            DriverState *driver_state = DriverState::get_instance();
            vector<int> input_vector = {1, 0, 1, 0};
            Terminal::set_input_vector(input_vector);
            driver_state->p1->fortify();
            Terminal::clear_terminal_input_counter();

            //verify some members have been set
            assert(driver_state->source_country &&
                   driver_state->target_country &&
                   driver_state->number_of_armies > 0);

            //start next phase, and confirm everything is default, properly cleared.
            Terminal::set_input(0);
            driver_state->p1->attack();
            assert(!driver_state->source_country &&
                   !driver_state->target_country &&
                   driver_state->number_of_armies < 0);
            return true;
        }

        bool test_attack() {
            DriverState *driver_state = DriverState::get_instance();

            // prepare source country and target country
            driver_state->gameInstance->get_map()->get_countries().at(0)->set_armies(5);
            driver_state->gameInstance->get_map()->get_countries().at(26)->set_armies(1);
            driver_state->p1->dice = new Dice::TestDice(6);
            driver_state->p2->dice = new Dice::TestDice(4);

            vector<int> input_vector = {1, 0, 1, 0, 0, 0};
            Terminal::set_input_vector(input_vector);
            driver_state->p1->attack();
            Terminal::clear_terminal_input_counter();

            assert(driver_state->p1->get_source_country());
            assert( driver_state->p1->get_target_country());
            assert(driver_state->p1->get_success());
            assert(driver_state->p1->get_battle_number() > 0);
            return true;

        }

        bool test_fortify() {
            DriverState *driver_state = DriverState::get_instance();
//            Terminal::test_mode_off();
            vector<int> input_vector = {1, 1, 0, 0};
            Terminal::set_input_vector(input_vector);
            driver_state->p1->fortify();
            Terminal::clear_terminal_input_counter();

            //verify some members have been set
            assert(driver_state->source_country);
            assert(driver_state->target_country);
            assert(driver_state->number_of_armies > 0);
            return true;
        }

        bool test_reinforce() {
            DriverState *driver_state = DriverState::get_instance();
//            Terminal::test_mode_off();
            vector<int> input_vector = {0, 3, 1, 2 };
            Terminal::set_input_vector(input_vector);
            driver_state->p1->reinforce();
            Terminal::clear_terminal_input_counter();

            assert(driver_state->p1->get_armies_gained_by_country_owned() >= 0);
            assert(driver_state->p1->get_armies_gained_by_continent_owned() >= 0);
            assert(driver_state->p1->get_armies_gained_by_exchange() >= 0);
            assert(driver_state->p1->get_reinforcement_vector()->size() == 2);

            return true;
        }

        bool run() {
            before();
//            Terminal::test_mode_off();
            bool passed = (
                    test_reinforce() &&
                    change_phase() &&
                    test_fortify() &&
                    test_attack()
            );
            return passed;
        }



    }


}
