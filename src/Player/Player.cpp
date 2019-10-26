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
    Player::Player() {
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

    string Player::fortify() {
        vector<Country> neighbors();
        for (auto country: *countries){
            //print country

        }
        //TODO 1: List owned countries

            //TODO 1.2: List neighbors
        //TODO 2: List owned countries' neighbors
        //TODO 3: Select source country
        //TODO 4: List source country valid neighbors



        return "fortify";
    }

    string Player::reinforce() {
        return "reinforce";
    }

    string Player::attack() {
        return "attack";
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
}
