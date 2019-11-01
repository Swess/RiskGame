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
        string exchange_desired;

            if(this->hand->size() >= 5 ){
                Terminal::print("Player [Color] has more than 5 cards in their hand! Exchange required.");

                while(this->hand->size() >= 5){
                    army_exchange += this->hand->exchange( /*  INDICES */ );
                }
            }
            if(this->hand->cardsValidForExchange(/* IS VALID FOR EXCHANGE */)){
                Terminal::print("Player [Color] has cards that are valid for exchange, would you like to proceed now? (y/n)");
                /* INPUT >>*/ exchange_desired;
                if(exchange_desired.compare("y")==0 || exchange_desired.compare("Y")==0){
                    army_exchange += this->hand->exchange(/*  INDICES */ );
                }
            }

        //Therefore, the total armies available (A) to Player are
        army_total = army_country + army_continent + army_exchange;

        /*
         * Then player places all armies on some of the countries it owns as they see fit.
         */
        string desired_index;
        int desired_armies_to_place;
        do{

            //Display available options
            Terminal::print("The armies available for positioning are: ");
            Terminal::print(army_total);
            /*INDEX = */

            /* PRINTOUT LIST OF COUNTRIES & ARMIES
             *
                     Terminal::print("Choose your source country");
                    vector<string> selections_source;
                 //FILTER NOT NEEDED just do get_countries  vector<Country* > countries_source = get_countries_attack_source();
                    selections_source.reserve(countries_source.size());
                    for (auto country : countries_source){
                        selections_source.emplace_back(country->to_string());
                    }
                  //NO  selections_source.emplace_back("Restart the process.");
                    int answer_source = Terminal::print_select(selections_source);
                  //NO  if (answer_source == selections_source.size()-1) { continue; }
             *
             * */
            Terminal::print("Here is the list of countries available: ");
           // vector<string> selection_source;
           // vector<Country *> countries_source =

            Terminal::print("Please specify the index of the country where you desire to position armies: ");
            do{
                /*INPUT */ desired_index;
              //ALREADY DONE IN TERMINAL - REDUNDANT  if(desired_index > /*MAP INDEX*/ || desired_index < 1){
                    Terminal::error("Index must be greater tan zero and less than " /*VALUE */ "Please enter another value: ");
                }
            } while(desired_index > /*MAP_INDEX*/ || desired_index < 1);

            /*MAPINDEX=0*/

            /*

              static bool print_select(const string& s);        (YES OR NO)
                static int print_select(std::vector<string> s); (INDEX returned) like country selection

                int Terminal::print_select(const int &minimum, const int &maximum, const string &s) //useful when user selects a country

             */

            //Then, Player decides how many armies they want to place

            /*

             while(Terminal::print_select("Player " + get_color() + ": Do you want to attack this round?")){
            if (!is_able_to_attack()) {
                Terminal::print("Player " + get_color() + " don't have any country he can attack from." );
                return false;
            }

             */

            do{
                /*INPUT */ desired_armies_to_place;
                if(desired_armies_to_place < 1 || desired_armies_to_place > army_total){
                    Terminal::error("The number of armies to place must be greater than zero and less than " /*total armies + 1*/ "Please enter another value: ");
                }
            }while(desired_armies_to_place < 1 || desired_armies_to_place > army_total);

            //Finally, we can add the armies
            for(/*ITERATE COUNTRY*/){
                if((desired_index-1) == /* INDEX */){
                    GameEngine::GameEngine::get_map()->get_countries().at(/*iterate*/).set_armies(desired_armies_to_place);
                    break;
                }
                /*INDEX++*/
            }

            army_total -= desired_armies_to_place;
        }while(army_total !=0);

        //Display Updated Countries & Armies??
        Terminal::print("Armies have been positioned successfully!");

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
