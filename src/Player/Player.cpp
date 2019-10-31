//
// Created by ker10 on 2019-09-26.
//

#include <iostream>
#include <vector>
#include "Player.h"
#include "../Map/Map.h"
#include "../GameEngine/GameEngine.h"
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

        Terminal::debug(" -------------------- Reinforcement phase -------------------");
        //Terminal::debug("Reinforcement phase");

        int army_country, army_continent=0, army_exchange, army_total;

        /*
            By definition, Player gets number of armies A, where A depends on the following:
               - number of countries owned on the map, divided by 3 (rounded down), with a min of 3.
               - continent-control value of all continents totally control value of all continents totally controlled by that player
               - armies resulting in card exchange, if possible. if a player owns more than 5 cards, must exchange cards (done in exchange method)
         */

        // First, we check if the player in question actually has any countries to reinforce
            if(this->get_countries().empty()){
                Terminal::error("You currently have no countries to reinforce!");
            } else {
                //In case the player owns countries, to calculate the number of armies A we first consider the number of countries owned on the map
                //divided by 3 (rounded down), with a minimum of 3.
                if ((this->get_countries().size() / 3) <= 3) {
                    army_country = 3;
                } else {
                    army_country = (this->get_countries().size()) / 3; //conversion to int will automatically just keep the integer part effectively rounding down the calculation
                }
            }

        //Second, we need to consider the owned continents by the current player
            //Gather countries owned
            vector<Country *> countries_owned = this->get_countries();

            //Gather continents and cycling through them
            vector<Continent *> continents_in_map = GameEngine::GameEngine::instance()->get_map()->get_continents();
            vector<Country *> countries_in_map = GameEngine::GameEngine::instance()->get_map()->get_countries();

            vector<int> continent_IDs_map (continents_in_map.size(), 0);
            vector<int> continent_IDs_owned (continents_in_map.size(), 0);
            int continents_owned = 0;

            //Counting how many countries are in each continent by ID
            for(int a=0;  a<countries_in_map.size(); a++){
                continent_IDs_map.at(countries_in_map.at(a)->get_continent_index())++;
            }
            //Counting how many countries are owned by player based on their continent
            for(int b=0;  b<countries_owned.size(); b++){
                continent_IDs_owned.at(countries_owned.at(b)->get_continent_index())++;
            }
            //Therefor, if continent_IDs_map equals continent_IDs_owned at a certain index, it means that
            //Player owns all countries in that specific continent (since a country can only be defined in a single continent).
            for(int c=0; c<continents_in_map.size(); c++){
                if(continent_IDs_map.at(c) == continent_IDs_owned.at(c)){
                    continents_owned++;
                }
            }

            //Determine total control value of owned Continents
            for(int d=0; d<continents_owned; d++){
                army_continent = army_continent /* CONTINENT CONTROL VALUE ?? */;
            }

        /*
         * Then player places all armies on some of the countries it owns as it sees fit
         * (in the reinforce method).
         */

        //Terminal::print("test" army_country+army_continent);

        //Finally, driver demonstrates that
        //1) if a player receives the right number of armies in the reinforcement phase (different cases)
        //2) the player has effectively placed this exact number of new armies somewhere on the map by the end of the reinforcement phase

        //army_exchange =

        //army_total =

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
