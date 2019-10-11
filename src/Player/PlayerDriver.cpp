//
// Created by ker10 on 2019-10-10.
//

#include <cassert>
#include <iostream>
#include "PlayerDriver.h"
#include "Player.h"

namespace Player{
    namespace Driver {
        bool test_attack(){
            Player p1;
            assert( p1.attack() == "attack");
            return true;
        }
        bool test_reinforce(){
            Player p1;
            assert(p1.reinforce() == "reinforce");
            return true;
        }
        bool test_fortify(){
            Player p1;
            assert (p1.fortify() == "fortify");
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

        bool test_countries(){
            Player p1;
            assert(p1.countries->size() == 0 );
            return true;
        }

        bool test_hand(){
            Player p1;
            assert(p1.hand->size() == 0 );
            return true;
        }

        bool run (){
            cout << "[TEST] -----" << endl;
            cout << "[TEST] - Running Player component tests." << endl;

            bool passed = test_attack() &&
                          test_fortify() &&
                          test_reinforce() &&
                          test_Dice() &&
                          test_countries();

            cout << "[TEST] - End of Player component testing." << endl;
            cout << "[TEST] -----" << endl;

            return passed;
        }
    }
}