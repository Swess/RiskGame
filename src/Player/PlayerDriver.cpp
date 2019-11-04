//
// Created by ker10 on 2019-10-10.
//

#include <cassert>
#include "PlayerDriver.h"
#include "Player.h"
#include "../MapLoader/MapLoader.h"
#include "../Terminal/Terminal.h"
#include "../exceptions.h"

namespace Player{
    namespace Driver {
        bool test_attack(){
            Player p1;
            return true;
        }
        bool test_reinforce(){
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
            Player * player_red = new Player();
            auto * player_green = new Player();
            auto * player_blue = new Player();
            auto * player_black = new Player();
            auto * player_white = new Player();


            /*
            //Country Assignment Sample
            // Assigning player countries
            player->gain_control(countries.at(0));
            player->gain_control(countries.at(1));
            player->gain_control(countries.at(2));
            */

            vector<Country * > countries = testMap->get_countries();

            //TEST (1): check if player receives the right number of armies during reinforcement

                //Case 1: Player_red with no countries nor cards
                vector<string> p_red = player_red->player_to_string();
                Terminal::print(p_red);
                player_red->reinforce();

                //Case 2: Player_green with 3 countries and no cards
                vector<string> p_green = player_blue->player_to_string();
                Terminal::print(p_green);

                //Case 3: Player_blue with 4 countries and 4 cards
                vector<string> p_blue = player_blue->player_to_string();
                Terminal::print(p_blue);

                //Case 4: Player_black with 1 continent and 4 cards
                vector<string> p_black = player_black->player_to_string();
                Terminal::print(p_black);

                //Case 5: Player_white with 2 continents and 7 cards
                vector<string> p_white = player_white->player_to_string();
                Terminal::print(p_white);



            //TEST (2): check that a player has effectively placed this exact number of new armies somewhere on the map by the end of the phase



            return true;
        }

        bool test_fortify(){

            // Prepare map
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

            // create two players
            auto * playerBlue = new Player();
            auto * playerGreen = new Player();

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
            vector<Country * > countries = testMap->get_countries();
            playerGreen->gain_control(countries.at(0));
            playerGreen->gain_control(countries.at(1));
            playerGreen->gain_control(countries.at(2));

            // Assign playerBlue two countries
            playerBlue->gain_control(countries.at(3));
            playerBlue->gain_control(countries.at(4));


            // First test: ensure that if no valid countries, then return
            playerGreen->turn();


            // Assign armies to each country
            countries.at(0)->set_armies(2);
            countries.at(1)->set_armies(5);
            countries.at(2)->set_armies(1);
            countries.at(3)->set_armies(3);
            countries.at(4)->set_armies(1);

            // Set inputs (fortify: yes, choose country 2 as source, choose neighbor 3 of country 2 as target, transfer 2 armies)
            int input_array[] = {0, 1, 2, 1};
            vector<int> input_vector;
            input_vector.assign(input_array, input_array+4);
            Terminal::set_input_vector(input_vector);
            playerGreen->turn();

            Terminal::clear_terminal_input_counter();
            // assert armies have been moved
            assert(playerGreen->get_countries().at(1)->get_armies() == 3);
            assert(playerGreen->get_countries().at(2)->get_armies() == 3);


            return true;
        }
        bool test_Dice(){
            Player p1;
            auto l = p1.dice->roll(2);
            int firstDice = l.at(0);
            int secondDice = l.at(1);
            assert( firstDice == 1 || firstDice == 2 || firstDice == 3 || firstDice == 4 || firstDice == 5 || firstDice == 6  );
            assert( secondDice == 1 || secondDice == 2 || secondDice == 3 || secondDice == 4 || secondDice == 5 || secondDice == 6  );
            return true;
        }

//        bool test_countries(){
//            Player p1;
//            assert(p1.countries->size() == 0 );
//            return true;
//        }

        bool test_hand(){
            Player p1;
            assert(p1.hand->size() == 0 );
            return true;
        }

        bool run (){
            bool passed = test_fortify();
//
//                    test_attack() &&
//                    test_fortify() &&
//                    test_reinforce() &&
//                    test_Dice();

            return passed;
        }
    }
}