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
#include "../GameEngine/GameEngine.h"

using namespace std;
using namespace Board;

namespace Player {

    enum class player_color;
    int Player::player_count = 0;

    Player::Player() {
        color = reinterpret_cast<player_color *>(new int(player_count++));
        hand = new Cards::Hand();
        dice = new Dice::Dice();
        countries = new vector<Country *>();
        playerStrategies = new HumanPlayerStrategy(this);
    };

    Player::Player(PlayerStrategies * playerStrategies1) : Player() {
       delete playerStrategies1;
        playerStrategies = playerStrategies1;
        playerStrategies1->setPlayer(this);
    }

    Player::~Player() {
        delete hand;
        delete dice;
        delete countries;
        delete color;
        delete playerStrategies;
        playerStrategies = nullptr;
        color = nullptr;
        countries = nullptr;
        hand = nullptr;
        dice = nullptr;
    }

    void Player::fortify() {
        Terminal::debug("Player fortify");
        Terminal::print("Fortification phase");
        vector<string> selections;

        bool can_fortify = player_can_fortify();

        if (!can_fortify) {
            Terminal::print("Fortification is not possible, you do not own two valid neighboring countries");
            return;
        }

        vector<Board::Country *> forfity_country = playerStrategies->fortify();
        if (forfity_country.empty()) return;

        Board::Country * source_country = forfity_country.at(0);
        Board::Country * target_country = forfity_country.at(1);
        ostringstream os;
        os.str("");
        os.clear();
        os << "countries following fortification: \n";
        os << "source country: " << source_country->to_string() << "\n";
        os << "target country: " << target_country->to_string();
        Terminal::print(os.str());
    }

    bool Player::player_can_fortify() const {
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
        return can_fortify;
    }

    void Player::reinforce() {

        int new_army = (int) this->get_countries().size() / 3;
        if (new_army < 3) new_army = 3;

        new_army += get_army_by_continent_owned();

        string message = "Player " + this->get_color() + ":You have " + std::to_string(new_army) + " units to place.";
        Terminal::print(message);

        playerStrategies->reinforce(new_army);

        Terminal::print("You don't have any units to place anymore.");

    }

    int Player::get_army_by_continent_owned() {
        int new_army = 0;
        GameEngine::GameEngine * instance = GameEngine::GameEngine::instance();
        auto continents = instance->get_map()->get_continents();

        for (auto continent: continents){
            int continent_size = continent->get_size();
            int owned_country_of_this_continent = 0;
            for (auto owned_country : get_countries()){
                if ( owned_country->get_continent_index() == continent->get_index()) {
                    owned_country_of_this_continent++;
                }
                if ( owned_country_of_this_continent == continent_size){
                    new_army += continent->get_bonus();
                    break;
                }
            }
        }
        return new_army;
    }

    bool Player::attack() {
        return playerStrategies->attack();
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

        reinforce();
        attack();
        fortify();

        Terminal::debug("Player has ended their turn");
    }

    void Player::gain_control(vector<Country *> f_countries) {
        for (auto & country : f_countries) {
            gain_control(country);
        }
    }

    string Player::get_color() {
        switch (*color) {
            case player_color::RED: return "Red";
            case player_color::BLUE: return "Blue";
            case player_color::GREEN: return "Green";
            case player_color::BLACK: return "Black";
            case player_color::GRAY: return "Gray";
            case player_color::WHITE: return "White";
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

    void Player::reset_player_count() {
        player_count = 0;
    }

    void Player::setPlayerStrategy(PlayerStrategies *playerStrategies1) {
        delete playerStrategies;
        playerStrategies = playerStrategies1;
    }


}
