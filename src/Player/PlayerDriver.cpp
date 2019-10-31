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
            Player p1;
            return true;
        }
        bool test_fortify(){
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

            //TODO 2: create two players
            auto * playerRed = new Player();
            auto * playerBlue = new Player();

            // some adjacent countries:
            /**
             * First 5 borders
             * 1 39 2 27
             * 2 1 27 3
             * 3 2 4
             * 4 3 5
             * 5 4 6 7
             */
            //TODO 3: give player one 5 countries, 3 adjacent, 1 adjacent to one, and one alone
            vector<Country * > countries = testMap->get_countries();
            countries.at(0)->set_owner(playerRed);
            countries.at(1)->set_owner(playerRed);
            countries.at(2)->set_owner(playerRed);
            //TODO 4: give player 2 some countries beside player one
            countries.at(3)->set_owner(playerBlue);
            countries.at(4)->set_owner(playerBlue);

            //TODO 5: give players armies, and distribute, ensure C3 get's only one army
            countries.at(0)->set_armies(2);
            countries.at(1)->set_armies(5);
            countries.at(2)->set_armies(1);
            countries.at(3)->set_armies(3);
            countries.at(4)->set_armies(1);

            //TODO 6: try to fortify from C1, to C2

            //TODO 7: try to fortify from C3 to C2
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