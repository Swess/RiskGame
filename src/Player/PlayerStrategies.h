/**
Created by ker10 on 2019-11-12.

Using the Strategy design pattern, implement different kinds of players that make different decisions during the
reinforcement, attack, and fortification phases. The kinds of players are: (1) human player that requires user
interaction to make decisions, (2) an aggressive computer player that focuses on attack (reinforces its strongest
country, then always attack with it until it cannot attack anymore, then fortifies in order to maximize aggregation of
forces in one country), (3) a benevolent computer player that focuses on protecting its weak countries (reinforces
its weakest countries, never attacks, then fortifies in order to move armies to weaker countries). You must deliver
a driver that demonstrates that (1) different players can be assigned different strategies that lead to different
behavior for the reinforcement, attack, and fortification phases using the strategy pattern; (2) the strategy adopted
by a player can be changed dynamically during play, (3) the human player makes decisions according to user
interaction, and computer players make decisions automatically, which are both implemented using the strategy
pattern.
*/

#pragma once

#include "../Cards/Cards.h"
#include "../Map/Map.h"

namespace Player {
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
        virtual bool attack() = 0;
        virtual vector<Board::Country *> fortify() = 0;
        virtual void reinforce(int i) = 0;
        void setPlayer(Player *d_player);
    };

    class HumanPlayerStrategy : public PlayerStrategies{
    public:
        HumanPlayerStrategy() = default;
        HumanPlayerStrategy(Player * player);
        ~HumanPlayerStrategy();

    private:
        bool attack() override;
        //Used by attack
        int battle_and_get_last_roll_amount(Board::Country *source, Board::Country *target) const override;

        vector<Board::Country *>  fortify() override;

        void reinforce(int i) override;
        //Used by reinforce
        int update_army_by_exchange(int new_army) const;
        void reinforce_country(int new_army);
    };

    class AggressivePlayerStrategy : public PlayerStrategies {
    public:
        AggressivePlayerStrategy() = default;
        AggressivePlayerStrategy(Player * player);
        ~AggressivePlayerStrategy();
    private:
    public:
        bool attack() override;

        vector<Board::Country *> fortify() override;

        void reinforce(int i) override;

    private:
        int battle_and_get_last_roll_amount(Board::Country *source, Board::Country *target) const override;

    };

    class BenevolentPlayerStrategy : public PlayerStrategies {
    public:
        BenevolentPlayerStrategy() = default;
        BenevolentPlayerStrategy(Player * player);
        ~BenevolentPlayerStrategy();
    private:
        int battle_and_get_last_roll_amount(Board::Country *source, Board::Country *target) const {
            return 0;
        }

    public:
        bool attack() override;

        vector<Board::Country *> fortify() override;

        void reinforce(int i) override;
    };

}