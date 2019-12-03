#pragma once

#include <vector>
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


        /**
         * Current state of player, this is what the observer is watching
         */
        phase * current_phase;
        Country * source_country;
        Country * target_country;
        int * number_of_armies;
        int * battle_number;
        bool * success;
        int * number_armies_gained_from_countries;
        int * number_armies_gained_from_continent;
        int * number_armies_gained_from_exchange;
        vector<pair<int, Country*>*> *reinforce_pair_vector;
        ///////////////////////////////////
    public:
        // TODO: These need to be private
        Dice::Dice *dice;
        Cards::Hand *hand;

        Player();
        explicit Player(PlayerStrategies * playerStrategies1);
        virtual ~Player();
        void select_computer_strategy();

        /**
         * Player is about to be used in another game, clear all previous data except strategy and colour
         */
        void clearPlayerForNewGame();
        /**
         * change player strategy at runtime
         */
        void setPlayerStrategy(PlayerStrategies * playerStrategies1);

        PlayerStrategies *getPlayerStrategy();
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
        bool turn();

        /////////////////////////
        void set_source_country(Country *country);
        void set_target_country(Country *country);
        void set_battle_number(int battle_number);
        void set_success(bool success);
        void set_number_armies(int armies);
        void set_armies_gained_by_exchange(int gained);
        void set_autonomous_fortification_phase_state(Country *source, Country *target, int armies_moved);
        void update_reinforce_pair_vector(pair<int, Country*> *int_country);



        string get_phase_string() override;
        Country *get_source_country() override;
        Country *get_target_country() override;
        int get_number_armies_used() override;
        int get_battle_number() override;
        bool *get_success() override;
        int get_armies_gained_by_continent_owned() override;
        int get_armies_gained_by_country_owned() override;
        int get_armies_gained_by_exchange() override;
        vector<pair<int, Country*>*> *get_reinforcement_vector() override;
        void clear_phase_state();
    };
}