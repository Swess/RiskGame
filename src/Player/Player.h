//
// Created by ker10 on 2019-09-26.
//
#pragma once

#ifndef TEAM12_DOMINATIONGAME_PLAYER_H
#define TEAM12_DOMINATIONGAME_PLAYER_H

#include "player_color.h"

using namespace std;

class Player {

public:
    void fortify();
    void reinforce();
    void attack();
    Player(player_color playerColor);
private:
    //cards *cards
    //countries *countries
    player_color *playerColor;

};


#endif //TEAM12_DOMINATIONGAME_PLAYER_H
