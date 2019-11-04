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
        RED=0, BLUE=1, GREEN=2, BLACK=3, GRAY=4, WHITE=5
    };

    class Player {
    private:
        vector<Country *> *countries;
        void fortify();
        //void reinforce();
        bool attack();
        bool is_able_to_attack();
        vector<Country *> get_countries_attack_source();
        player_color color;
        static int player_count;
    public:
        // TODO: These need to be private
        Dice::Dice *dice;
        Cards::Hand *hand;

        Player();
        virtual ~Player();
        /**
         * Take control of a country and handle logic for making sure the player is the only
         * one that has control.
         */
        void gain_control(Country* country);

        /**
         * Take control of multiple country and handle logic for making sure the player is the only
         * one that has control. Used for testing mostly.
         */
        void gain_control(vector<Country *> f_countries);

        /**
         * Get a copy of the current list of country pointers
         * @return
         */
        vector<Country *> get_countries();

        bool is_player_dead();

        string get_color();
        void turn();

        void reinforce();

        vector<string> player_to_string();
    };
}