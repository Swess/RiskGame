#pragma once

#include <vector>
#include "../Map/Map.h"
#include "../Cards/Cards.h"
#include "../Dice/Dice.h"

using namespace std;
using namespace Board;

namespace Player {

    // This is not used, but may be used later
    enum player_color {
        RED, BLUE, GREEN, BLACK, GRAY, WHITE
    };

    class Player {
        vector<Country *> *countries;
    public:
        // TODO: These need to be private
        Dice::Dice *dice;
        Cards::Hand *hand;

        Player();

        virtual ~Player();

        string fortify();

        string reinforce();

        string attack();

        /**
         * Take control of a country and handle logic for making sure the player is the only
         * one that has control.
         */
        void gain_control(Country* country);

        /**
         * Get a copy of the current list of country pointers
         * @return
         */
        vector<Country *> get_countries();
    };
}