//
//  main.cpp
//  DiceRoller
//
//  Created by Francesco Benzi on 2019-09-20.
//

#include <iostream>
#include <cstdlib>
#include <vector>
#include <time.h>
#include "Dice.hpp"

using namespace std;

void printRollSorter(vector<int> &vec){
    
    for(unsigned int i=0; i<vec.size(); ++i){
        
        cout << vec.at(i) << "  ";
    }
    cout<< endl;
}

namespace DiceDriver {

    int run () {

        srand(time(0)); // Setting a unique pattern for the rand() function

        /*
           From the outline:

           Driver that test:
           1) one can request from 1 to 3 dice being rolled,
           2) that the container returns the right number of values in a sorted container,
           3) that no value outside of the 1-6 range is ever returned,
           4) that there is an equal share of 1-6 values returned and
           5) that every dice rolling facility object maintains a percentage of all individual values rolled up to now.

        */

        //Test Die
        Dice object_1;
        vector<int> rollSorter_1;

        /*
         * The variable 'iterations' is used to select how many iterations to test (at least 3 in the assignment) but for instance 10
         * to 500 will show that the % for each dice face value will be approximately ~16% = 1/6 = fair dice / equal share
         */
        int iterations = 100;

        //This variable defines the number of Dice to roll (from 1 to 3 in the Risk Game)
        int number_of_dice_rolled = 3;


        cout << "\nWelcome to DICE tester" << endl;

        //This do-while cycle tests the requested amount of iterations of the roll() function
        do {

            rollSorter_1 = object_1.roll(number_of_dice_rolled);    //creates a rollsorter vector with the Dice value provided
            cout << "\nrollSorter values: ";
            printRollSorter(rollSorter_1);                                   //prints the values generated in rollSorter_1
            cout << "\nRolling percentages: " << endl;
            object_1.printRollPercentageArray();

            iterations--;

        }while(iterations > 0);

        return 0;
    }

}




