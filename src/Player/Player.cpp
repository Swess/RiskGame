//
// Created by ker10 on 2019-09-26.
//

#include <iostream>
#include "Player.h"

using namespace std;

void Player::fortify() {
    cout << this->playerColor << " fortify" << endl;
}

void Player::reinforce() {
    cout << this->playerColor << " reinforce" << endl;
}

void Player::attack() {
    cout << this->playerColor << " attack" << endl;
}

Player::Player(player_color playerColor) {
    this->playerColor = &playerColor;
}
