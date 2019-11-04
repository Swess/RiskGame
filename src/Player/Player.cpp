//
// Created by ker10 on 2019-09-26.
//

#include <iostream>
#include <vector>
#include <string>
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

        Terminal::debug("Reinforcement phase");

        /*
            By definition, Player gets number of armies A, where A depends on the following:
               - number of countries owned on the map, divided by 3 (rounded down), with a min of 3.
               - continent-control value of all continents totally control value of all continents totally controlled by that player
               - armies resulting in card exchange, if possible. if a player owns more than 5 cards, must exchange cards (done in exchange method)
         */
        //Armies declaration
        int army_country=0, army_continent=0, army_exchange=0, army_total=0;

        /*
         * First, we check if the player in question actually has any countries to reinforce
         */
            //Gather countries owned
            vector<Country *> countries_owned = this->get_countries();

            //In case the player owns countries, to calculate the number of armies A we first consider the number of countries owned on the map
            //divided by 3 (rounded down), with a minimum of 3.
            if ((countries_owned.size() / 3) <= 3) {
                army_country = 3;
            } else {
                army_country = (countries_owned.size()) / 3; //rounded-down
            }

        /*
         * Second, we need to consider the owned continents by the current player
         */

            //Gather continents and countries from the GameEngine
            vector<Continent *> continents_in_map = GameEngine::GameEngine::instance()->get_map()->get_continents();
            vector<Country *> countries_in_map = GameEngine::GameEngine::instance()->get_map()->get_countries();
            //Declaring vectors of integers that act as counters
            vector<int> continent_index_map (continents_in_map.size(), 0);
            vector<int> continent_index_owned (continents_in_map.size(), 0);
            int continents_owned = 0;

            //Counting how many countries are found in each continent of the map
            for(int a=0;  a<countries_in_map.size(); a++){
                continent_index_map.at(countries_in_map.at(a)->get_continent_index())++;
            }
            //Counting how many player-owned countries are found in each continent of the map
            for(int b=0;  b<countries_owned.size(); b++){
                continent_index_owned.at(countries_owned.at(b)->get_continent_index())++;
            }
            //Therefore, if continent_index_map equals continent_index_owned at a certain index, it means that
            //Player owns all countries in that specific continent (since a country can only be defined in a single continent).
            for(int c=0; c<continents_in_map.size(); c++){
                if(continent_index_map.at(c) == continent_index_owned.at(c)){
                    //Found a continent that is owned by player
                    continents_owned++;
                    //Update army_continent
                    army_continent += continents_in_map.at(c)->get_bonus();
                }
            }

        /*
         * Third, we consider the armies resulting from a card exchange (if applicable).
         * If player owns more than 5 cards, then it must exchange cards
         */
        bool exchange_desired;
        vector<string> print_exchange_required;
        print_exchange_required.emplace_back("Player [" + this->get_color() + "] has more than 5 cards in their hand! Exchange required. \n Following are the cards available in their hand: ");
        vector<string> print_hand;
        int answer_cards_indices[3];

            if(this->hand->size() >= 5) {
                //Displaying warning message
                Terminal::print(print_exchange_required);

                //Displaying available cards to Player
                for (int i = 0; i < this->hand->size(); i++) {
                    print_hand.emplace_back(this->hand->to_string());
                }
                Terminal::print(print_exchange_required);

                //Prompt the user to choose three cards
                Terminal::print("Please input your desired [3] cards: [1" + std::to_string(this->hand->size()) + "].");
                answer_cards_indices[0] = Terminal::print_select(this->hand->to_string()) -1;
                answer_cards_indices[1] = Terminal::print_select(this->hand->to_string()) -1;
                answer_cards_indices[2] = Terminal::print_select(this->hand->to_string()) -1;


                //Checking if cards are valid
                if(this->hand->cardsValidForExchange(answer_cards_indices)){
                    exchange_desired = Terminal::print_select("Player [Color] has cards that are valid for exchange, would you like to proceed now?");
                    if(exchange_desired){
                        army_exchange += this->hand->exchange(answer_cards_indices);
                    }
                }

                //Exchanging cards
                while(this->hand->size() >= 5){
                    army_exchange += this->hand->exchange(answer_cards_indices);
                }
            }

        //Therefore, the total armies available (A) to Player are
        army_total = army_country + army_continent + army_exchange;

        /*
         * Then player places all armies on some of the countries it owns as they see fit.
         */

        //Looping around the number of armies available to place
        do{
            //Display available options (armies available + countries for positioning)
            Terminal::print("You are now able to place your reinforcement armies. The total armies and countries available for positioning are the following: ");
                vector<string> armies_print;
                armies_print.emplace_back(army_total + " total armies available");
                Terminal::print(armies_print);
            /*INDEX = */

                //Gathering countries owned by player and in map
                    //vector<Countries *> countries_owned ~ already declared above
                    //vector<Countries *> countries_in_map ~ already declared above
                //Printing Utility
                vector<string> options;
                for(auto c : countries_owned) {
                    options.emplace_back(c->get_name() + " (which has" + to_string(c->get_armies()) + " armies present).");
                }
                //Asking player to input data
                options.emplace_back("\nPlease enter the country [#] to which you would like to place armies: ");
                int answer_country = Terminal::print_select(options);

                vector<string> options_number;
                options_number.emplace_back("Enter how many armies you would like to place on your chosen country: ");
                int answer_number = Terminal::print_select(options_number);

                //Adding 'answer_number' armies to 'answer_country'
                countries_in_map.at(answer_country)->set_armies(countries_in_map.at(answer_country)->get_armies() + answer_number);

            //Reducing the # of armies available
            army_total -= answer_number;
        }while(army_total !=0);

        //Printing success message
        vector<string> print_reinforcement_success;
        print_reinforcement_success.emplace_back("Armies have been positioned successfully! Your countries are now equipped with the following number of armies: ");
        for(auto c : countries_owned){
            print_reinforcement_success.emplace_back(c->get_name() + " has" + to_string(c->get_armies()) + " armies present.");
        }
        Terminal::print(print_reinforcement_success);
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
