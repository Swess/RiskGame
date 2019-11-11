#pragma once

#include <vector>
#include "../Map/Map.h"
#include "../Cards/Cards.h"
#include "../Dice/Dice.h"
#include "../GameObservers/GameObservers.h"

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
        vector<Country *> *countries;
        void fortify();
        void reinforce();
        bool attack();
        bool is_able_to_attack();
        player_color * color;
        static int player_count;
        vector<Country *> get_countries_attack_source();
        int battle_and_get_last_roll_amount(Country *source, Country *target) const;
        int get_attacker_amount_of_dice(Country *source) const;
        bool player_can_fortify() const;
        void reinforce_country(int new_army);
        int get_army_by_continent_owned();
        int update_army_by_exchange(int new_army) const;
        void clear_state();

        /**
         * Current state of player, this is what the observer is watching
         */
        phase * current_phase;
        Country * source_country;
        Country * target_country;
        int * number_of_armies;
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
        static void reset_player_count();

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

        string get_phase() override;
        Country *get_source_country() override;
        Country *get_target_country() override;
        int get_number_armies_used() override;
    };
}