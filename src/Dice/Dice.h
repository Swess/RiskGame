//
//  Dice.hpp
//  Dice
//
//  Created by Francesco Benzi on 2019-09-20.
//

#ifndef Dice_hpp
#define Dice_hpp
#pragma once

#include <vector>

namespace Dice {
    class Dice {

    public:

        Dice();

        /*
         * roll() rolls one to three dice, registers the new %
         * and returns a sorted container which holds the
         * values of the dice.
         */
        virtual std::vector<int> roll(int numDice);

        /*
         * printRollPercentageArray() is a function that prints the current
         * rolling % for all dice values (from 1 to 6).
         */

        void printRollPercentageArray();

        /*
         * getRollPercentage() is a function that gathers the %
         * value for a specific dice value and returns it as a double.
         */

        double getRollPercentage(int roll);

        /*
         * getTotalRolls() is a function that returns the total number
         * of dice that has been rolled
         */
        int getTotalRolls();

        virtual ~Dice();

    private:

        //Pointer Variables
        int *totalRolls;
        double *rollPercentageArray; //stores a value for each dice face [6]

        //Methods
        void initRollPercentArray();

        void addToRollPercentArray(int val);


    };

    /**
     * Used to substitute the values returned by a Dice to always return the subtituted value
     */
    class TestDice : public Dice {
        int* subtitute_val;
    public:
        explicit TestDice(int value);

        virtual ~TestDice() override;

        std::vector<int> roll(int numDice) override;
    };
}
#endif /* Dice_hpp */
