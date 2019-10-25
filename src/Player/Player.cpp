//
// Created by ker10 on 2019-09-26.
//

#include <iostream>
#include <vector>
#include "Player.h"
#include "../Cards/Cards.h"
#include "../Dice/Dice.h"
#include "../Terminal/Terminal.h"

using namespace std;

namespace Player {
    Player::Player() {
        hand = new Cards::Hand();
        dice = new Dice::Dice();
    };

    Player::~Player() {
        delete hand;
        delete dice;
        hand = nullptr;
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
