//
//  Dice.cpp
//  DiceRoller
//
//  Created by Francesco Benzi on 2019-09-20.
//  Copyright © 2019 Francesco Benzi. All rights reserved.
//

#include "Dice.hpp"
#include <iostream>
#include <vector>

using namespace std;

Dice::Dice(){
    
    totalRolls = 0;
    double rollPercentageArray[6];
    initRollPercentArray();
}

vector<int> Dice::diceRoller(int diceNumber){
    
    int rollValue;
    vector <int> rollSorter;
    
    /*
     * Gathers values of the dice and records them in a
     * sorted vector while also updating the % array
     */
    for(int i=0; i<diceNumber; i++){
        //update total counter
        totalRolls++;
        
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

void Dice::printRollPercentageArray(){
    
    for(int i=0; i < 6; i++){
        cout << "Number '" << i + 1 << "' has been rolled " << rollPercentageArray[i] << " % of the time." << endl;
    }
}

/*
 * getRollPercentage() is a function that gathers the %
 * value for a specific dice value.
 */

void Dice::getRollPercentage(int rollValue){
    //Error case: print a message in case rollValue is out of bounds
    if(rollValue < 1 || rollValue > 6) {
        cout << "Error: 'rollValue' must be between 1 and 6 included." << endl;
        exit(1);
    }
    //otherwise print % of a rollValue
    cout << "The value '" << rollValue << "' was rolled " << rollPercentageArray[rollValue-1] << " % of the time." << endl;
}

/*
 * getTotalRolls() is a function that returns the total number
 * of dice that has been rolled
 */
int Dice::getTotalRolls(){
    
    return totalRolls;
    
}
/*
 * initRollPercentArray() is a void function that initializes the % array
 */
void Dice::initRollPercentArray(){
    
    for(int i=0; i < 6; i++){
        rollPercentageArray[i] = 0.0;
    }
    
}


/*
 * addToRollPercentArray() is a void function that updates the % array
 */
void Dice::addToRollPercentArray(int val){
    
    //Transform % to Double value
    if(getTotalRolls() > 1){
        
        for(int i=0; i<6; i++){
            rollPercentageArray[i] = rollPercentageArray[i] / 100 * (getTotalRolls() - 1);
        }
    }
    
    //Updates rollValue and reverts it back to % value
    rollPercentageArray[val - 1]++;
    for(int i=0; i<6; ++i){
        rollPercentageArray[i] = rollPercentageArray[i] / getTotalRolls() * 100;
    }
    
}

