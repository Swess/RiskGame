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

            //Creating 2 players
            auto * playerBlue = new Player();
            auto * playerGreen = new Player();

            //Country Assignment Sample
            // Assigning playerGreen 3 countries, country 1 is in between the other two
            vector<Country * > countries = testMap->get_countries();
            playerGreen->gain_control(countries.at(0));
            playerGreen->gain_control(countries.at(1));
            playerGreen->gain_control(countries.at(2));

            // Assigning playerBlue two countries
            playerBlue->gain_control(countries.at(3));
            playerBlue->gain_control(countries.at(4));

            //test 1)


            //test 2)

            return true;
        }

        bool test_fortify(){
            Player p1;
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
            bool passed = test_attack() &&
                          test_fortify() &&
                          test_reinforce() &&
                          test_Dice();

            return passed;
        }
    }
}