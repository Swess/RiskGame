#pragma once

#include <vector>
#include "../Cards/Cards.h"
#include "../Dice/Dice.h"

using namespace std;

namespace Player {

    // This is not used, but may be used later
    enum player_color {
        RED, BLUE, GREEN, BLACK, GRAY, WHITE
    };

    class Player {
    public:
        Dice::Dice *dice;
        Cards::Hand *hand;

        Player();

        virtual ~Player();

        string fortify();

        string reinforce();

        string attack();

    };
}