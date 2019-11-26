/**
Created by ker10 on 2019-11-12.

*/

#pragma once

#include "../Cards/Cards.h"
#include "../Map/Map.h"

namespace Player {
    /*
     * PlayerStrategies uses the Strategy design pattern, and is the Strategy (super class) class where other
     * Concrete Strategy will extends. It needs a reference of the player object it lives in.
     */
    class PlayerStrategies {
    private:
        virtual int battle_and_get_last_roll_amount(Board::Country *source, Board::Country *target) const = 0;
    protected:
        Player * player;
        int get_attacker_amount_of_dice(Board::Country *source) const;
    public:
        PlayerStrategies();
        PlayerStrategies(Player *received_player);
        virtual ~PlayerStrategies();
        /*
         * @return true if the attacker gain atleast one country (useful for deck handling)
         */
        virtual bool attack() = 0;
        /*
         * return a vector containing either 0 or 2 countries.
         * if empty it means that the player will not fortify,
         * if 2 countries, the first one will be the source country and the second one is the target.
         */
        virtual vector<Board::Country *> fortify() = 0;
        /*
         * Takes a number of army the player can reinforce and put them into own's country
         */
        virtual void reinforce(int i) = 0;
        /*
         * set a player reference to the strategy. Must only be used by the player constructor.
         */
        void setPlayer(Player *d_player);
    };

    /*
     * Human player strategy is a concrete player strategy
     * It is controlled by a human.
     */
    class HumanPlayerStrategy : public PlayerStrategies{
    public:
        HumanPlayerStrategy() = default;
        explicit HumanPlayerStrategy(Player * player);
        ~HumanPlayerStrategy() override;

    private:

        bool attack() override;
        //Used by attack
        int battle_and_get_last_roll_amount(Board::Country *source, Board::Country *target) const override;

        vector<Board::Country *>  fortify() override;

        void reinforce(int i) override;
        //Used by reinforce
        int update_army_by_exchange() const;
        void reinforce_country(int new_army);
    };

    /*
     * Aggressive player strategy is a concrete player strategy
     * It an aggressive computer player that focuses on attack (reinforces its strongest
     * country, then always attack with it until it cannot attack anymore,
     * then fortifies in order to maximize aggregation of forces in one country),
     */
    class AggressivePlayerStrategy : public PlayerStrategies {
    public:
        AggressivePlayerStrategy() = default;
        explicit AggressivePlayerStrategy(Player * player);
        ~AggressivePlayerStrategy() override;
    private:
    public:
        bool attack() override;

        vector<Board::Country *> fortify() override;

        void reinforce(int i) override;

    private:
        int battle_and_get_last_roll_amount(Board::Country *source, Board::Country *target) const override;

    };

    /* Benevolent player strategy is a concrete player strategy
     * A benevolent computer player that focuses on protecting its weak countries (reinforces
     * its weakest countries, never attacks, then fortifies in order to move armies to weaker countries)
     */
    class BenevolentPlayerStrategy : public PlayerStrategies {
    public:
        BenevolentPlayerStrategy() = default;
        explicit BenevolentPlayerStrategy(Player * player);
        ~BenevolentPlayerStrategy() override;
    private:
        int battle_and_get_last_roll_amount(Board::Country *source, Board::Country *target) const override {
            return 0;
        }

    public:
        bool attack() override;

        vector<Board::Country *> fortify() override;

        void reinforce(int i) override;
    };

    /* Random player strategy is a concrete player strategy
     * A random computer player that reinforces random a random country, attacks a random number
     * of times a random country, and fortifies a random country.
     */
    class RandomPlayerStrategy : public PlayerStrategies {
    public:
        RandomPlayerStrategy() = default;
        explicit RandomPlayerStrategy(Player * player);
        ~RandomPlayerStrategy() override;
        bool attack() override;
        vector<Board::Country *> fortify() override;
        void reinforce(int i) override;
    private:
        int battle_and_get_last_roll_amount(Board::Country *source, Board::Country *target) const override;
    };

    /*
     * A Cheater Computer Player Strategy whose reinforce() method doubles the number of armies on all
        its countries, whose attack() method automatically conquers all the neighbors of all its countries, and
        whose fortify() method doubles the number of armies on its countries that have neighbors that belong
        to other players.
     */

    class CheaterPlayerStrategy : public PlayerStrategies {
    public:
        CheaterPlayerStrategy() = default;
        explicit CheaterPlayerStrategy(Player * player);
        ~CheaterPlayerStrategy() override;
        bool attack() override;
        vector<Board::Country *> fortify() override;
        void reinforce(int i) override;

    private:
        int battle_and_get_last_roll_amount(Board::Country *source, Board::Country *target) const override;
    };

}