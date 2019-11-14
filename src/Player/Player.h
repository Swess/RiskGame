#pragma once

#include <vector>
#include "../Map/Map.h"
#include "../Cards/Cards.h"
#include "../Dice/Dice.h"
#include "../GameObservers/GameObservers.h"
#include "./PlayerStrategies.h"

using namespace std;
using namespace Board;

namespace Player {

    enum class player_color {
        RED=0, BLUE=1, GREEN=2, BLACK=3, GRAY=4, WHITE=5
    };

    enum class phase {
        ATTACK=0, REINFORCEMENT=1, FORTIFICATION=2
    };

class Player : public Observer::PlayerSubject {
    private:
        //Data members
        vector<Country *> *countries;
        player_color * color;
        PlayerStrategies * playerStrategies;

        //Methods
        void fortify();
        void reinforce();
        bool attack();
        static int player_count;
        bool player_can_fortify() const;
        int get_army_by_continent_owned();
        ///////////////////////////////
        int update_army_by_exchange(int new_army) const;
        void clear_state();

        /**
         * Current state of player, this is what the observer is watching
         */
        phase * current_phase;
        Country * source_country;
        Country * target_country;
        int * number_of_armies;
        ///////////////////////////////////
    public:
        // TODO: These need to be private
        Dice::Dice *dice;
        Cards::Hand *hand;

        Player();
        Player(PlayerStrategies * playerStrategies1);
        virtual ~Player();

        /**
         * change player strategy at runtime
         */
        void setPlayerStrategy(PlayerStrategies * playerStrategies1);

        /**
         * Take control of a country and handle logic for making sure the player is the only
         * one that has control.
         */
        void gain_control(Country* country);
        /*
         * Reset the player count to make sure we have new colors if we reset the gameinstance.
         */
        static void reset_player_count();
        /*
         * Verify if the player is able to attack with his country as context
         */
        bool is_able_to_attack();

        /*
         * @returns list of available countries attack source
         */
        vector<Country *> get_countries_attack_source();
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

        string get_color() override;
        void turn();

        /////////////////////////
        string get_phase() override;
        Country *get_source_country() override;
        Country *get_target_country() override;
        int get_number_armies_used() override;
        /////////////////////////
    };
}