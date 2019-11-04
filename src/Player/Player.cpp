//
// Created by ker10 on 2019-09-26.
//

#include <iostream>
#include <vector>
#include <string>
#include <sstream>
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
        int armies_assigned; //used for testing of reinforce()
    };

    Player::~Player() {
        delete hand;
        delete dice;
        delete countries;
        countries = nullptr;
        hand = nullptr;
        dice = nullptr;
    }

    vector<string> Player::player_to_string() {

        vector<string> p;
        p.emplace_back("Player " + this->get_color() + ": \n -Countries:\n ");
        if(this->get_countries().size() == 0){
            p.emplace_back("Player" + this->get_countries() + " has no countries!");
            Terminal::print(p);
        }else{
            for(int c=0; c<this->get_countries().size(); c++){
                p.emplace_back(this->get_countries().at(c)->get_name() +"\n");
            }
            p.emplace_back("- Hand: \n" + this->hand->to_string());
        }

        return p;
    }
    void Player::set_armies_assigned(int a){
        armies_assigned = a;
    }
    int Player::get_armies_assigned(){
        return armies_assigned;
    }

    void Player::fortify() {
        Terminal::debug("Player fortify");
        Terminal::print("Fortification phase");
        vector<string> selections;
        bool validChoice = false;
        int source_country_index;
        Country * source_country;

        bool can_fortify = false;
        // confirm fortification is possible
        for (auto & country : *countries) {
            for (auto & neighbor : *country->get_neighbors()) {
                if (this == neighbor->get_owner() && country->get_armies() > 1) {
                    can_fortify = true;
                    break;
                }
            }
            if (can_fortify) {
                break;
            }
        }

        if (!can_fortify) {
            Terminal::print("Fortification is not possible, you do not own two valid neighboring countries");
            return;
        }
        Terminal::print("Fortification is possible, would you like to fortify?");
        vector<string> fortify;
        fortify.emplace_back("Yes");
        fortify.emplace_back("No");
        int response = Terminal::print_select(fortify);
        if (response == 1) {
            return;
        }


        Terminal::print("Select source country");
        for (auto & country : *countries) {
            selections.push_back(country->to_string_with_neighbors());
        }
        while (!validChoice) {
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
        Terminal::print("Select target country");
        for (Country * neighbor: *source_country_neighbors) {
            selections.push_back(neighbor->to_string_with_neighbors());
        }
        while (!validChoice) {
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
        vector<string> num_of_armies(source_country->get_armies() - 1);
        for (int i = 0; i < (source_country->get_armies() - 1); i++) {
            ostringstream os;
            if (i == 0) {
                os << "1 army";
            } else {
                os << i + 1 << " armies";
            }
            num_of_armies.at(i) = os.str();
        }
        num_of_armies.shrink_to_fit();
        int selected_num_armies = Terminal::print_select(num_of_armies);
        selected_num_armies++;

        ostringstream os;
        os << selected_num_armies << " armies have been transferred from " << source_country->get_name() << " to " << target_country->get_name();
        Terminal::print(os.str());

        source_country->set_armies(source_country->get_armies() - selected_num_armies);
        target_country->set_armies(target_country->get_armies() + selected_num_armies);
        os.str("");
        os.clear();
        os << "countries following fortification: \n";
        os << "source country: " << source_country->to_string() << "\n";
        os << "target country: " << target_country->to_string();
        Terminal::print(os.str());
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
                Terminal::print_select(1, this->hand->size(), "Please input your desired [3] cards: ");
                answer_cards_indices[0] = Terminal::print_select(this->hand->to_string());
                answer_cards_indices[1] = Terminal::print_select(this->hand->to_string());
                answer_cards_indices[2] = Terminal::print_select(this->hand->to_string());

                this->set_card_indices_for_exchange(answer_cards_indices);

                //Checking if cards are valid
                if(this->hand->cardsValidForExchange(answer_cards_indices)){
                    exchange_desired = Terminal::print_select("Player [Color] has cards that are valid for exchange, would you like to proceed now?");
                    if(exchange_desired){
                        army_exchange += this->hand->exchange(answer_cards_indices);
                    }
                }else{
                    /* TO DO */
                }

                //Exchanging cards
                while(this->hand->size() >= 5){
                    army_exchange += this->hand->exchange(answer_cards_indices);
                }
            }

        //Therefore, the total armies available (A) to Player are
        army_total = army_country + army_continent + army_exchange;

        //Assigning the value to player to use it for testing
        this->set_armies_assigned(army_total);

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

                //Gathering countries owned by player and in map
                    //vector<Countries *> countries_owned ~ already declared above
                    //vector<Countries *> countries_in_map ~ already declared above
                //Printing Utility
                vector<string> options;
                for(auto c : countries_owned) {
                    options.emplace_back(c->get_name() + " (which has" + to_string(c->get_armies()) + " armies present).");
                }
                //Asking player to input data
                Terminal::print("Please enter the country [#] to which you would like to place armies: ");
                int answer_country = Terminal::print_select(options);

                int answer_number = Terminal::print_select(1, army_total, "Enter how many armies you would like to place on your chosen country: ");

                //Adding 'answer_number' armies to 'answer_country'
                countries_in_map.at(answer_country)->set_armies(countries_in_map.at(answer_country)->get_armies() + answer_number);

            //Reducing the # of armies available
            army_total -= answer_number;
        }while(army_total <=0);

        //Printing success message
        vector<string> print_reinforcement_success;
        print_reinforcement_success.emplace_back("Armies have been positioned successfully! Your countries are now equipped with the following number of armies: ");
        for(auto c : countries_owned){
            print_reinforcement_success.emplace_back(c->get_name() + " has" + to_string(c->get_armies()) + " armies present.");
        }
        Terminal::print(print_reinforcement_success);
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
