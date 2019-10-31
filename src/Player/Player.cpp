//
// Created by ker10 on 2019-09-26.
//

#include <iostream>
#include <vector>
#include "Player.h"
#include "../Cards/Cards.h"
#include "../Dice/Dice.h"
#include "../Terminal/Terminal.h"

using namespace std;
using namespace Board;

namespace Player {

    int Player::player_count = 0;

    Player::Player() {
        color = static_cast<player_color > (player_count++) ;
        hand = new Cards::Hand();
        dice = new Dice::Dice();
        countries = new vector<Country *>();
    };

    Player::~Player() {
        delete hand;
        delete dice;
        delete countries;
        countries = nullptr;
        hand = nullptr;
        dice = nullptr;
    }

    void Player::fortify() {
        Terminal::debug("Player fortify");
    }

    void Player::reinforce() {
        //TODO
    }

    bool Player::attack() {
        bool got_a_country = false;
        while(Terminal::print_select("Player " + get_color() + ": Do you want to attack this round?")){
            if (!is_able_to_attack()) {
                Terminal::print("Player " + get_color() + " don't have any country he can attack from." );
                return false;
            }

            Terminal::print("Choose your source country");
            vector<string> selections_source;
            vector<Country* > countries_source = get_countries_attack_source();
            selections_source.reserve(countries_source.size());
            for (auto country : countries_source){
                selections_source.emplace_back(country->to_string());
            }
            selections_source.emplace_back("Restart the process.");
            int answer_source = Terminal::print_select(selections_source);
            if (answer_source == selections_source.size()-1) { continue; }

            Terminal::print("Choose your target country");
            vector<string> selections_target;
            for (auto country : * countries_source.at(answer_source)->get_neighbors()){
                if (country->get_owner() == this) continue;
                selections_target.emplace_back(country->to_string());
            }
            selections_target.emplace_back("Restart the process.");
            int answer_target = Terminal::print_select(selections_target);
            if (answer_target == selections_target.size()-1) { continue; }

            Country * source = countries_source.at(answer_source);
            Country * target = source->get_neighbors()->at(answer_target);

            // At the end of an attack, if it's a win a attacker has to move in at
            // least as many armies as the number of dice you rolled in its last battle
            int last_roll_attacker = 0;

            while (source->get_armies() > 1 && target->get_armies() > 0 ) {
                Terminal::print("The country you started the attack from is");
                Terminal::print(source->to_string());
                Terminal::print("The country you are attacking is");
                Terminal::print(target->to_string());

                // If you attack with 2 army, can only throw 1 dice
                int available_dice_attacker = source->get_armies() - 1;
                // Clamp the amount of attacker's dice to 3
                if (available_dice_attacker > 3) available_dice_attacker = 3;
                string attacker_question = "How many dice you want to attack with? (Choose 0 to stop the battle.)";
                int nb_of_dice_attacker = Terminal::print_select(0, available_dice_attacker, attacker_question);
                if (nb_of_dice_attacker == 0) { break; }

                int available_dice_defender = target->get_armies();
                // Clamp the amount of defender's dice to 2
                if (available_dice_defender > 2) available_dice_defender = 2;
                int nb_of_dice_defender = 0;
                if (available_dice_defender == 1) {
                    Terminal::print("Player " + target->get_owner()->get_color() + " can only throw a die.");
                    nb_of_dice_defender = 1;
                } else {
                    string defender_question = "How many dice Player " + target->get_owner()->get_color() + " want to defend with?";
                    nb_of_dice_defender = Terminal::print_select(1, available_dice_defender, defender_question);
                }
                vector<int> roll_attacker = source->get_owner()->dice->roll(nb_of_dice_attacker);
                vector<int> roll_defender = target->get_owner()->dice->roll(nb_of_dice_defender);

                Terminal::print("Player " + source->get_owner()->get_color() + " rolled: ");
                Terminal::print_on_same_line(roll_attacker);
                Terminal::print("Player " + target->get_owner()->get_color() + " rolled: ");
                Terminal::print_on_same_line(roll_defender);

                int how_many_unit_will_die = available_dice_defender < available_dice_attacker ? available_dice_defender
                                                                                               : available_dice_attacker;

                for (int i = 0; i < how_many_unit_will_die; i++) {
                    roll_attacker[i] > roll_defender[i] ? target->decrement_army() : source->decrement_army();
                }

                last_roll_attacker = nb_of_dice_attacker;

            } // End of a battle

            if (target->get_armies() == 0 ) {
                got_a_country = true;
                Terminal::print("Congratulations! You decimated your opponents.");
                gain_control(target);
                string after_attack_question = "How many units do you want to move in your new country?";
                int moving_units = Terminal::print_select(last_roll_attacker, source->get_armies()-1, after_attack_question);
                source->set_armies(source->get_armies() - moving_units);
                target->set_armies(moving_units);
                Terminal::print("The country you started the attack from now is");
                Terminal::print(source->to_string());
                Terminal::print("Your new country now is");
                Terminal::print(target->to_string());
            } else if (source->get_armies() == 1){
                Terminal::print("Whoops! You can't attack from this country anymore. Better luck next time.");
            }
        } // End of attack round
        return got_a_country;
    }

    void Player::gain_control(Country *country) {
        Player* other = country->get_owner();
        // Remove from the other player's countries
        if(other != nullptr){
            vector<Country *>::iterator it;
            for(it = other->countries->begin(); it < other->countries->end(); it++){
                if((*it) == country)
                    other->countries->erase(it);
            }
        }

        countries->push_back(country);
        country->set_owner(this);
    }

    vector<Country *> Player::get_countries() {
        return *countries;
    }

    void Player::turn() {
        Terminal::debug("Player has started their turn");

        this->fortify();
        this->attack();
        this->reinforce();

        Terminal::debug("Player has ended their turn");
    }

    void Player::gain_control(vector<Country *> f_countries) {
        for (auto & country : f_countries) {
            gain_control(country);
        }
    }

    string Player::get_color() {
        switch (color) {
            case RED: return "Red";
            case BLUE: return "Blue";
            case GREEN: return "Green";
            case BLACK: return "Black";
            case GRAY: return "Gray";
            case WHITE: return "White";
            default: return "ERROR";
        }
    }

    bool Player::is_player_dead() {
        return get_countries().empty();
    }

    bool Player::is_able_to_attack() {
        bool can_attack = false;
        for (auto country : *countries){
            if (country->get_armies() > 1){
                for (auto neighbor : * country->get_neighbors()){
                    if (neighbor->get_owner() != this) {
                        can_attack = true;
                    }
                }
            }
        }
        return can_attack;
    }

    vector<Country *> Player::get_countries_attack_source(){
        vector<Country *> avail_countries;
        for (auto country : *countries){
            if (country->get_armies() > 1){
                for (auto neighbor : * country->get_neighbors()){
                    if (this != neighbor->get_owner()) {
                        avail_countries.emplace_back(country);
                        break;
                    }
                }
            }
        }
        return avail_countries;
    }
}
