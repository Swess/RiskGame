//
//  main.cpp
//  Dice
//
//  Created by Francesco Benzi on 2019-09-20.
//  From the outline:
//
//  Driver that test:
//  1) one can request from 1 to 3 dice being rolled,
//  2) that the container returns the right number of values in a sorted container,
//  3) that no value outside of the 1-6 range is ever returned,
//  4) that there is an equal share of 1-6 values returned and
//  5) that every dice rolling facility object maintains a percentage of all individual values rolled up to now.
//

#include <iostream>
#include <cstdlib>
#include <vector>
#include <ctime>
#include <cassert>
#include "Dice.h"

using namespace std;

namespace Dice {

    void printRollSorter(vector<int> &vec) {

        for (unsigned int i = 0; i < vec.size(); ++i) {

            cout << vec.at(i) << "  ";
        }
        cout << endl;
    }

    namespace Driver {
        int run() {

            srand(time(0));

            //Test Die
            Dice object_1;
            vector<int> rollSorter_1;

            /*
             * The variable 'iterations' is used to select how many iterations to test (at least 3 in the assignment) but for instance 10
             * to 500 will show that the % for each dice face value will be approximately ~16% = 1/6 = fair dice / equal share
             */
            int iterations = 10000;
            int number_of_dice_rolled = 3;

          
            cout << "\n[TEST] -----" << endl;
            cout << "[TEST] - Running Dice component testing." << endl;

            //This do-while cycle tests the requested amount of iterations of the roll() function
            do {

                rollSorter_1 = object_1.roll(number_of_dice_rolled);

                /*
                 * Optional Console Utility that displays rollSorter values
                 * and RollPercentageArray

                     cout << "\nrollSorter values: ";
                     printRollSorter(rollSorter_1);
                     cout << "\nRolling percentages: " << endl;
                     object_1.printRollPercentageArray();
                 */

                iterations--;

            } while (iterations > 0);

            //Once all the dice have been rolled, the rollPercentage values are verified in order to check that the program
            //generated rolls within 5% tolerance of the ideal percentage
            // If a fair dice = ~16%; minValue = 16-tolerance and maxValue = 16+tolerance

            double tolerance = 5;

            //Gathering test results
            bool test_face_1 = object_1.getRollPercentage(1) > 16.00 - tolerance && object_1.getRollPercentage(1) < 16.00 + tolerance;
            bool test_face_2 = object_1.getRollPercentage(2) > 16.00 - tolerance && object_1.getRollPercentage(1) < 16.00 + tolerance;
            bool test_face_3 = object_1.getRollPercentage(3) > 16.00 - tolerance && object_1.getRollPercentage(1) < 16.00 + tolerance;
            bool test_face_4 = object_1.getRollPercentage(4) > 16.00 - tolerance && object_1.getRollPercentage(1) < 16.00 + tolerance;
            bool test_face_5 = object_1.getRollPercentage(5) > 16.00 - tolerance && object_1.getRollPercentage(1) < 16.00 + tolerance;
            bool test_face_6 = object_1.getRollPercentage(6) > 16.00 - tolerance && object_1.getRollPercentage(1) < 16.00 + tolerance;

            //Asserting test results
            assert(test_face_1 && test_face_2 && test_face_3 && test_face_4 && test_face_5 && test_face_6);

            cout << "[TEST] - End of Dice component testing." << endl;
            cout << "[TEST] -----" << endl << endl;

            return true;
        }

    }
}




