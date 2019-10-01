//
//  Dice.hpp
//  DiceRoller
//
//  Created by Francesco Benzi on 2019-09-20.
//  Copyright © 2019 Francesco Benzi. All rights reserved.
//

#ifndef Dice_hpp
#define Dice_hpp

#pragma once
#include <vector>
#include <stdio.h>

class Dice
{
    
public:
    
    Dice();
    
    /*
     * diceRoller() rolls one to three dice, registers the new %
     * and returns a sorted container which holds the
     * values of the dice.
     */
    std::vector<int> diceRoller(int numDice);
    
    /*
     * printRollPercentageArray() is a function that prints the current
     * rolling % for all dice values (from 1 to 6).
     */
    
    void printRollPercentageArray();
    
    /*
     * getRollPercentage() is a function that gathers the %
     * value for a specific dice value.
     */
    
    void getRollPercentage(int roll);
    
    /*
     * getTotalRolls() is a function that returns the total number
     * of dice that has been rolled
     */
    int getTotalRolls();
    
private:
    
    //Variables
    int totalRolls;
    double rollPercentageArray[6]; //stores a value for each dice face
    
    //Methods
    void initRollPercentArray();
    void addToRollPercentArray(int val);
    
    
};

#endif /* Dice_hpp */
