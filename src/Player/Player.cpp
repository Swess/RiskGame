//
// Created by ker10 on 2019-09-26.
//

#include <iostream>
#include <vector>
#include <sstream>
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
    string Player::fortify() {
        Terminal::print("Fortification phase");
        vector<string> selections(countries->size());
        bool validChoice = false;

        int source_country_index;
        Country * source_country;
        while (!validChoice) {
            Terminal::print("Select source country");
            for (auto & country : *countries) {
                if (country->get_armies()) {
                    selections.push_back(country->to_string_with_neighbors());
                }
            }
            source_country_index = Terminal::print_select(selections);
            source_country = countries->at(source_country_index);
            if (source_country->get_armies() > 1) {
                for (auto & country : *source_country->get_neighbors()) {
                    if (this == country->get_owner()) {
                        validChoice = true;
                        break;
                    }
                }
                if (!validChoice) {
                    Terminal::print("must choose a country with at least one neighbor which you own.");
                }
            } else {
                Terminal::print("must choose a country which has at least 2 armies.");
            }
        }


        selections.clear();
        selections.shrink_to_fit();
        vector<Country *> * source_country_neighbors = source_country->get_neighbors();
        int target_country_index;
        Country * target_country;
        validChoice = false;
        while (!validChoice) {
            Terminal::print("Select target country");
            for (Country * neighbor: *source_country_neighbors) {
                selections.push_back(neighbor->to_string_with_neighbors());
            }
            target_country_index = Terminal::print_select(selections);
            target_country = source_country_neighbors->at(target_country_index);
            if (this != target_country->get_owner()) {
                validChoice = false;
                Terminal::print("Must own selected country");
            } else {
                validChoice = true;
            }
        }

        Terminal::print("Select number of armies to move");
        vector<string> num_of_armies(source_country->get_armies());
        for (int i = 1; i < source_country->get_armies(); i++) {
            ostringstream os;
            if (i == 1) {
                os << "1 army";
            } else {
                os << i << " armies";
            }
            num_of_armies.at(i) = os.str();
        }
        int selected_num_armies = Terminal::print_select(num_of_armies);
        selected_num_armies++;

        source_country->set_armies(source_country->get_armies() - selected_num_armies);
        target_country->set_armies(target_country->get_armies() + selected_num_armies);

        return "fortify";
    }

    void Player::reinforce() {
        //TODO
    }

    void Player::attack() {
        //TODO
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
}
