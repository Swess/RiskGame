//
// Created by ker10 on 2019-10-10.
//

#include <cassert>

#define private public

#include "PlayerDriver.h"
#include "Player.h"
#include "../MapLoader/MapLoader.h"
#include "../Terminal/Terminal.h"
#include "../exceptions.h"
#include "../GameEngine/GameEngine.h"

namespace Player {
    namespace Driver {

        bool test_attack() {

            return test_attacking_and_attacked_country_validity() &&
                   test_amount_of_dice() &&
                   test_deducted_armies() &&
                   test_multiple_attacks();

        }

        bool test_amount_of_dice() {
            // 2) only valid number of dice can be chosen by the attacker/defender;
            auto gameInstance = GameEngine::GameEngine::instance();
            gameInstance->start_test(1, 3);
            gameInstance->startup_phase();

            auto player = gameInstance->get_players()->at(0);
            auto countries = player->get_countries_attack_source();

            Country *source = countries.at(0);

            int amount_of_dice = player->get_attacker_amount_of_dice(source);

            if (source->get_armies() > 3) {
                assert(amount_of_dice == 3);
            } else {
                assert(amount_of_dice - 1 == source->get_armies());
            }


            gameInstance->reset_test();
            return true;
        }

        bool test_attacking_and_attacked_country_validity() {
            auto gameInstance = GameEngine::GameEngine::instance();
            //1) only valid attacks can be declared (i.e. valid attacker/attacked country);
            gameInstance->start_test(1, 3);
            gameInstance->startup_phase();

            auto player = gameInstance->get_players()->at(0);
            auto countries = player->get_countries_attack_source();

            // verify that my country are all owned by me, and has more than one army
            for (auto country : countries) {
                assert(country->get_owner() == player);
                assert(country->get_armies() > 1);
            }

            // verify that the country has enemy neighbor
            for (auto country : *countries.at(0)->get_neighbors()) {
                if (country->get_owner() == player) continue;
                assert(country->get_owner() != player);
            }


            gameInstance->reset_test();
            return true;
        }

        bool test_deducted_armies() {
            // 3) given known dice values, that the right number of armies are deducted on the attacker/defender;
            auto gameInstance = GameEngine::GameEngine::instance();

            gameInstance->start_test(1, 3);
            gameInstance->startup_phase();

            auto player = gameInstance->get_players()->at(0);
            auto opponent = gameInstance->get_players()->at(1);
            auto countries = player->get_countries_attack_source();

            // Assign country in a testable manner
            Country *source = countries.at(0);
            Country *target_country = source->get_neighbors()->at(0);

            // Set combat setup
            source->set_armies(10);
            target_country->set_armies(10);
            opponent->gain_control(target_country);

            // Substitute player's dice to control return values
            player->dice = new Dice::TestDice(4);
            opponent->dice = new Dice::TestDice(2);

            // Substitute inputs and test
            vector<int> input_vector = {3, 2, 0};
            Terminal::set_input_vector(input_vector);
            player->battle_and_get_last_roll_amount(source, target_country);
            Terminal::clear_terminal_input_counter();

            assert(source->get_armies() == 10);
            assert(target_country->get_armies() == 8);

            // .. With 2 dices
            input_vector = {2, 2, 0};
            Terminal::set_input_vector(input_vector);
            player->battle_and_get_last_roll_amount(source, target_country);
            Terminal::clear_terminal_input_counter();

            assert(source->get_armies() == 10);
            assert(target_country->get_armies() == 6);

            // With 1 dice attack
            input_vector = {1, 2, 0};
            Terminal::set_input_vector(input_vector);
            player->battle_and_get_last_roll_amount(source, target_country);
            Terminal::clear_terminal_input_counter();

            assert(source->get_armies() == 10);
            assert(target_country->get_armies() == 5);

            gameInstance->reset_test();
            return true;
        }

        bool test_multiple_attacks() {
            // 4) the attacker is allowed to initiate multiple attacks, until it declares that it does not want to attack anymore.
            auto gameInstance = GameEngine::GameEngine::instance();

            gameInstance->start_test(1, 3);
            gameInstance->startup_phase();

            auto player = gameInstance->get_players()->at(0);

            player->dice = new Dice::TestDice(5);
            gameInstance->get_players()->at(1)->dice = new Dice::TestDice(1);
            gameInstance->get_players()->at(2)->dice = new Dice::TestDice(1);

            // Buff players everywhere
            for (Country *c : gameInstance->get_map()->get_countries())
                c->set_armies(100);

            vector<int> battle_actions_pattern = {1, 0, 0, 3, 2, 3, 2, 0};   // Roll dices twice
            vector<int> battle_actions;

            // Run battle 4 times and decide to stop
            for (int i = 0; i < 4; i++)
                battle_actions.insert(battle_actions.end(), battle_actions_pattern.begin(),
                                      battle_actions_pattern.end());
            battle_actions.push_back(0);

            Country *source = player->get_countries_attack_source().at(0);
            Country *target_country = source->get_neighbors()->at(0);
            // Make sure the target is not ours
            gameInstance->get_players()->at(1)->gain_control(target_country);

            Terminal::set_input_vector(battle_actions);
            player->attack();
            Terminal::clear_terminal_input_counter();

            // Check that battles are successfully executed and won
            assert(source->get_armies() == 100);
            assert(target_country->get_armies() == 84);

            gameInstance->reset_test();
            return true;
        }

        bool test_reinforce() {
            // T 1: a player receives the right number of armies in the reinforcement phase (showing different cases)
            auto gameInstance = GameEngine::GameEngine::instance();

            gameInstance->start_test(1, 3);
            gameInstance->startup_phase();

            auto player = gameInstance->get_players()->at(0);

            for (auto card : * gameInstance->get_deck()->cards){
                player->hand->insertCard(*card);
            }

            int owned_army = 0;
            for (auto country : player->get_countries()){
                owned_army += country->get_armies();
            }

            Terminal::test_mode_off();
            player->reinforce();
            Terminal::test_mode_on();

            // T 2: the player has effectively placed this exact number of new armies somewhere on the map by the end of the reinforcement phase.
            return true;
        }

        bool test_fortify() {

            // Prepare map
            Player p1;
            auto *mapLoader = new MapLoader::MapLoader();
            string s = "RiskEurope.map";
            Board::Map *testMap = nullptr;
            try {
                testMap = mapLoader->load(s)->build();
            } catch (IOException &e) {
                string error = e.what(s);
                Terminal::error(error);
                exit(1);
            }

            // create two players
            auto *playerBlue = new Player();
            auto *playerGreen = new Player();

            // some adjacent countries:
            /**
             * First 5 borders
             * 1 39 2 27
             * 2 1 27 3
             * 3 2 4
             * 4 3 5
             * 5 4 6 7
             */
            // assign playerGreen 3 countries, country 1 is in between the other two
            vector<Country *> countries = testMap->get_countries();
            playerGreen->gain_control(countries.at(0));
            playerGreen->gain_control(countries.at(1));
            playerGreen->gain_control(countries.at(2));

            // Assign playerBlue two countries
            playerBlue->gain_control(countries.at(3));
            playerBlue->gain_control(countries.at(4));


            // First test: ensure that if no valid countries, then return
            playerGreen->fortify();


            // Assign armies to each country
            countries.at(0)->set_armies(2);
            countries.at(1)->set_armies(5);
            countries.at(2)->set_armies(1);
            countries.at(3)->set_armies(3);
            countries.at(4)->set_armies(1);

            // Set inputs (fortify: yes, choose country 2 as source, choose neighbor 3 of country 2 as target, transfer 2 armies)
            int input_array[] = {1 , 1, 2, 1};
            vector<int> input_vector;
            input_vector.assign(input_array, input_array + 4);
            Terminal::set_input_vector(input_vector);
            playerGreen->fortify();

            Terminal::clear_terminal_input_counter();
            // assert armies have been moved
            assert(playerGreen->get_countries().at(1)->get_armies() == 3);
            assert(playerGreen->get_countries().at(2)->get_armies() == 3);


            return true;
        }

        bool test_Dice() {
            Player p1;
            auto l = p1.dice->roll(2);
            int firstDice = l.at(0);
            int secondDice = l.at(1);
            assert(firstDice == 1 || firstDice == 2 || firstDice == 3 || firstDice == 4 || firstDice == 5 ||
                   firstDice == 6);
            assert(secondDice == 1 || secondDice == 2 || secondDice == 3 || secondDice == 4 || secondDice == 5 ||
                   secondDice == 6);
            return true;
        }

//        bool test_countries(){
//            Player p1;
//            assert(p1.countries->size() == 0 );
//            return true;
//        }

        bool test_hand() {
            Player p1;
            assert(p1.hand->size() == 0);
            return true;
        }

        bool run() {
            bool passed = (
                    test_attack() &&
                    test_fortify() &&
                    test_reinforce() &&
                    test_Dice()
            );
            return passed;
        }
    }
}
