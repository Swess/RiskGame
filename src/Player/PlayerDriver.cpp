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

            Terminal::set_input_vector(battle_actions);
            player->attack();
            Terminal::clear_terminal_input_counter();

            Country *source = player->get_countries_attack_source().at(0);
            Country *target_country = source->get_neighbors()->at(0);

            // Check that battles are successfully executed and won
            assert(source->get_armies() == 100);
            assert(target_country->get_armies() == 84);

            gameInstance->reset_test();
            return true;
        }


        bool test_reinforce() {
            //The test_reinforce() method demonstrates that
            //1) if a player receives the right number of armies in the reinforcement phase (different cases)
            //2) the player has effectively placed this exact number of new armies somewhere on the map by the end of the reinforcement phase

            //Preparing test map
            Player p1;
            auto * mapLoader = new MapLoader::MapLoader();
            string s = "RiskEurope.map";
            Board::Map * testMap = nullptr;
            try {
                testMap = mapLoader->load(s)->build();
            } catch (IOException &e) {
                string error = e.what(s);
                Terminal::error(error);
                exit(1);
            }

            //Creating players
            auto * player_red = new Player();
            auto * player_green = new Player();
            auto * player_blue = new Player();
            auto * player_black = new Player();
            auto * player_white = new Player();

            vector<Country *> map_countries = testMap->get_countries();
            Cards::Deck * game_deck = GameEngine::GameEngine::instance()->get_deck();

            //TEST (1): check if player receives the right number of armies during reinforcement

                //Case 1: Player_red with no countries nor cards
                    vector<string> p_red = player_red->player_to_string();
                    int armies_red_init = 0;
                    for(auto c : player_red->get_countries())
                        armies_red_init += c->get_armies();


                    Terminal::print(p_red);

                    player_red->reinforce();

                    //This player should only get 3 armies by the rules.
                    int rules_armies_red = 3;
                    //Hence
                    assert(player_red->get_armies_assigned() == rules_armies_red);

                    int armies_red_final = 0;
                    for(auto c : player_red->get_countries())
                        armies_red_final += c->get_armies();

                    delete player_red; //to avoid conflicts

                //Case 2: Player_green with 3 countries and no cards
                    player_green->gain_control(map_countries.at(1)); //Malta
                    player_green->gain_control(map_countries.at(2)); //Tunisia
                    player_green->gain_control(map_countries.at(3)); //Algeria

                    int armies_green_init = 0;
                    for(auto c : player_green->get_countries())
                        armies_green_init += c->get_armies();

                    vector<string> p_green = player_blue->player_to_string();
                    Terminal::print(p_green);

                    player_green->reinforce();

                    //This player should only get 3 armies by the rules.
                    int rules_armies_green = 3;
                    //Hence
                    assert(player_green->get_armies_assigned() == rules_armies_green);

                    int armies_green_final = 0;
                    for(auto c : player_green->get_countries())
                        armies_green_final += c->get_armies();

                    delete player_green; //to avoid conflicts

                //Case 3: Player_blue with 4 countries and 4 cards
                    player_blue->gain_control(map_countries.at(4)); //Morocco
                    player_blue->gain_control(map_countries.at(5)); //Spain
                    player_blue->gain_control(map_countries.at(6)); //Portugal
                    player_blue->gain_control(map_countries.at(7)); //France

                    int armies_blue_init = 0;
                    for(auto c : player_blue->get_countries())
                        armies_blue_init += c->get_armies();

                    player_blue->hand->insertCard(game_deck->draw());
                    player_blue->hand->insertCard(game_deck->draw());
                    player_blue->hand->insertCard(game_deck->draw());
                    player_blue->hand->insertCard(game_deck->draw());

                    vector<string> p_blue = player_blue->player_to_string();
                    Terminal::print(p_blue);

                    player_blue->reinforce();

                    //This player should only get 3 armies by the rules.
                    int rules_armies_blue = 3;
                    //Hence
                    assert(player_blue->get_armies_assigned() == rules_armies_blue);

                    int armies_blue_final = 0;
                    for(auto c : player_blue->get_countries())
                        armies_blue_final += c->get_armies();

                    delete player_blue; //to avoid conflicts

                //Case 4: Player_black with 1 continent and 4 cards
                    //Continent = Great Britain
                    player_black->gain_control(map_countries.at(11)); //England
                    player_black->gain_control(map_countries.at(12)); //Wales
                    player_black->gain_control(map_countries.at(13)); //Scotland
                    player_black->gain_control(map_countries.at(14)); //Northern Ireland
                    player_black->gain_control(map_countries.at(15)); //Ireland

                    int armies_black_init = 0;
                    for(auto c : player_black->get_countries())
                        armies_black_init += c->get_armies();

                    player_black->hand->insertCard(game_deck->draw());
                    player_black->hand->insertCard(game_deck->draw());
                    player_black->hand->insertCard(game_deck->draw());
                    player_black->hand->insertCard(game_deck->draw());

                    vector<string> p_black = player_black->player_to_string();
                    Terminal::print(p_black);

                    player_black->reinforce();

                    //This player should get 3 armies for the countries owned and 3 for the continent owned (Great Britain)
                    int rules_armies_black = 6;
                    //Hence
                    assert(player_black->get_armies_assigned() == rules_armies_black);

                    int armies_black_final = 0;
                    for(auto c : player_black->get_countries())
                        armies_black_final += c->get_armies();

                      delete player_black; //to avoid conflicts

                //Case 5: Player_white with 2 continents and 7 cards
                    //First Continent = Scandinavia
                    player_white->gain_control(map_countries.at(16)); // Finland
                    player_white->gain_control(map_countries.at(17)); // Sweden
                    player_white->gain_control(map_countries.at(18)); // Norway
                    player_white->gain_control(map_countries.at(19)); // Denmark
                    player_white->gain_control(map_countries.at(20)); // Iceland

                    //Second Continent = Iberia
                    player_white->gain_control(map_countries.at(5)); // Spain
                    player_white->gain_control(map_countries.at(6)); // Portugal

                    int armies_white_init = 0;
                    for(auto c : player_white->get_countries())
                        armies_white_init += c->get_armies();

                    player_white->hand->insertCard(game_deck->draw());
                    player_white->hand->insertCard(game_deck->draw());
                    player_white->hand->insertCard(game_deck->draw());
                    player_white->hand->insertCard(game_deck->draw());
                    player_white->hand->insertCard(game_deck->draw());
                    player_white->hand->insertCard(game_deck->draw());
                    player_white->hand->insertCard(game_deck->draw());

                    vector<string> p_white = player_white->player_to_string();
                    Terminal::print(p_white);

                    player_white->reinforce();

                    //This player should get 3 armies for the countries owned + 4 armies for the continents owned (Scandinavia = 3 + Iberia = 1) + x armies from card exchange
                    int rules_armies_white = 7; // TO FIX + player_white->hand->exchange(/* answer_cards_indices */);
                    //Hence
                    assert(player_white->get_armies_assigned() == rules_armies_white);

                    int armies_white_final = 0;
                    for(auto c : player_white->get_countries())
                        armies_white_final += c->get_armies();

                    delete player_white; // to avoid conflicts


            //TEST (2): check that a player has effectively placed this exact number of new armies somewhere on the map by the end of the phase

                assert((armies_red_init + player_red->get_armies_assigned()) == armies_red_final);
                assert((armies_green_init + player_green->get_armies_assigned()) == armies_green_final);
                assert((armies_blue_init + player_blue->get_armies_assigned()) == armies_blue_final);
                assert((armies_black_init + player_black->get_armies_assigned()) == armies_black_final);
                assert((armies_white_init + player_white->get_armies_assigned()) == armies_white_final);

            return true;
        }

        bool test_fortify(){

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
            int input_array[] = {0, 1, 2, 1};
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
            bool passed = test_attack() &&
                          test_fortify() &&
                          test_reinforce() &&
                          test_Dice();

            return passed;
        }
    }
}
