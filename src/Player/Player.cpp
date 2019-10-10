//
// Created by ker10 on 2019-09-26.
//

#include <iostream>
#include <vector>
#include "Player.h"
#include "../Map/Map.h"
#include "../Cards/Cards.h"
#include "../Dice/Dice.h"

using namespace std;

namespace Player{
Player::Player() {
    hand = new Cards::Hand();
    countries = new vector<Map::Country*>;
    dice = new Dice::Dice();
}

Player::~Player(){
    delete hand;
    delete countries;
    delete dice;
    hand = nullptr;
    countries = nullptr;
    dice = nullptr;
}

string Player::fortify() {
    return "fortify";
}

string Player::reinforce() {
    return "reinforce";
}

string Player::attack() {
    return "attack";
}
}