//
//  Dice.cpp
//  Dice
//
//  Created by Francesco Benzi on 2019-09-20.
//

#include "Dice.h"
#include "../Terminal/Terminal.h"
#include <vector>
#include <algorithm>

using namespace std;

namespace Dice {
    Dice::Dice() {

        this->totalRolls = new int(0);
        this->rollPercentageArray = new double[6];
        initRollPercentArray();
    }

    Dice::~Dice() {
        delete totalRolls;
        delete rollPercentageArray;

        //Removing dangling pointers
        totalRolls = nullptr;
        rollPercentageArray = nullptr;
    }

    vector<int> Dice::roll(int diceNumber) {

        int rollValue;
        vector<int> rollSorter;

        /*
         * Gathers values of the dice and records them in a
         * sorted vector while also updating the % array
         */
        for (int i = 0; i < diceNumber; i++) {
            //update total counter
            (*totalRolls)++;

            //Calculate random die value
            rollValue = 1 + (rand() % 6);

            /*
             * Gathering the random value and sorting it
             * through the vector; also updating the %
             */
            rollSorter.push_back(rollValue);
            addToRollPercentArray(rollValue);
        }

        /*
         * Sorting the vector and returning it
         */
        std::sort(rollSorter.begin(), rollSorter.end(), greater<int>());
        return rollSorter;
    }

/*
 * printRollPercentageArray() is a function that prints the current
 * rolling % for all dice values (from 1 to 6).
 */

    void Dice::printRollPercentageArray() {
        for (int i = 0; i < 6; i++) {
            Terminal::print("Number '" + std::to_string(i + 1) + "' has been rolled " + std::to_string(rollPercentageArray[i]) + " % of the time.");
        }
    }
/*
 * getRollPercentage() is a function that gathers the %
 * value for a specific dice value.
 */

    double Dice::getRollPercentage(int rollValue) {
        //Error case: print a message in case rollValue is out of bounds
        if (rollValue < 1 || rollValue > 6) {
            Terminal::error("'rollValue' must be between 1 and 6 included.");
            exit(1);
        }

        //otherwise return the RollPercentage of the desired rollValue the desired RollPercentage value
        return rollPercentageArray[rollValue - 1];
    }

/*
 * getTotalRolls() is a function that returns the total number
 * of dice that has been rolled
 */
    int Dice::getTotalRolls() {

        return *totalRolls;

    }

/*
 * initRollPercentArray() is a void function that initializes the % array
 */
    void Dice::initRollPercentArray() {

        for (int i = 0; i < 6; i++) {
            rollPercentageArray[i] = 0.0;
        }

    }


/*
 * addToRollPercentArray() is a void function that updates the % array
 */
    void Dice::addToRollPercentArray(int val) {

        //Transform % to Double value
        if (getTotalRolls() > 1) {

            for (int i = 0; i < 6; i++) {
                rollPercentageArray[i] = rollPercentageArray[i] / 100 * (getTotalRolls() - 1);
            }
        }

        //Updates rollValue and reverts it back to % value
        rollPercentageArray[val - 1]++;
        for (int i = 0; i < 6; ++i) {
            rollPercentageArray[i] = rollPercentageArray[i] / getTotalRolls() * 100;
        }

    }

}