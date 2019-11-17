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

        current_phase = new phase;
        source_country = nullptr;
        target_country = nullptr;
        number_of_armies = new int(-1);
        battle_number = new int(-1);
        number_armies_gained_from_continent = new int(-1);
        number_armies_gained_from_countries = new int(-1);
        number_armies_gained_from_exchange = new int(-1);
        reinforce_pair_vector = new vector<pair<int, Country*>*>;
        success = nullptr;
    }

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

        // phase variables
        delete current_phase;
        delete number_of_armies;
        delete battle_number;
        delete number_armies_gained_from_continent;
        delete number_armies_gained_from_countries;
        delete number_armies_gained_from_exchange;

        current_phase = nullptr;
        number_of_armies = nullptr;
        battle_number = nullptr;
        number_armies_gained_from_countries = nullptr;
        number_armies_gained_from_countries = nullptr;
        number_armies_gained_from_exchange = nullptr;

    }

    void Player::fortify() {
        Terminal::debug("Player fortify");
        Terminal::print("Fortification phase");
        clear_phase_state();
        *current_phase = phase::FORTIFICATION;
        notify();
        vector<string> selections;
        bool validChoice = false;
        int source_country_index;

        bool can_fortify = player_can_fortify();

        if (!can_fortify) {
            Terminal::print("Fortification is not possible, you do not own two valid neighboring countries");
            return;
        }

        vector<Board::Country *> fortify_country = playerStrategies->fortify();
        if (fortify_country.empty()) return;

        source_country = fortify_country.at(0);
        target_country = fortify_country.at(1);
        notify();
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

        clear_phase_state();
        *current_phase = phase::REINFORCEMENT;
        *number_armies_gained_from_countries = (int) this->get_countries().size() / 3;
        if (*number_armies_gained_from_countries < 3) *number_armies_gained_from_countries = 3;
        *number_armies_gained_from_continent = get_army_by_continent_owned();
        notify();
        int new_army = *number_armies_gained_from_continent + *number_armies_gained_from_countries;
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
        clear_phase_state();
        *current_phase = phase::ATTACK;
        notify();
        bool attack = playerStrategies->attack();
        if (attack && GameEngine::GameEngine::instance()->game_state) {
            GameEngine::GameEngine::instance()->game_state->notify();
        }
        return attack;
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

    bool Player::turn() {
        Terminal::debug("Player has started their turn");

        reinforce();
        bool needs_update = attack();
        fortify();

        Terminal::debug("Player has ended their turn");
        return needs_update;
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

    string Player::get_phase_string() {
        switch(*current_phase) {
            case phase::ATTACK:
                return "Attack";
            case phase::REINFORCEMENT:
                return "Reinforcement";
            case phase::FORTIFICATION:
                return "Fortification";
            default:
                return "ERROR";
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

    Country *Player::get_source_country() {
        return source_country;
    }

    Country *Player::get_target_country() {
        return target_country;
    }

    int Player::get_number_armies_used() {
        return *number_of_armies;
    }

    void Player::clear_phase_state() {
        source_country = nullptr;
        target_country = nullptr;
        *number_of_armies = -1;
        *battle_number = -1;
        *number_armies_gained_from_continent = -1;
        *number_armies_gained_from_countries = -1;
        *number_armies_gained_from_exchange = -1;
        reinforce_pair_vector->clear();

        delete success;
        success = nullptr;
    }

    int Player::get_battle_number() {
        return *battle_number;
    }

    bool *Player::get_success() {
        return success;
    }

    int Player::get_armies_gained_by_continent_owned() {
        return *number_armies_gained_from_continent;
    }

    int Player::get_armies_gained_by_country_owned() {
        return *number_armies_gained_from_countries;
    }

    int Player::get_armies_gained_by_exchange() {
        return *number_armies_gained_from_exchange;
    }


    void Player::setPlayerStrategy(PlayerStrategies *playerStrategies1) {
        delete playerStrategies;
        playerStrategies = playerStrategies1;
    }

    void Player::set_source_country(Country *country) {
        source_country = country;
        notify();
    }

    void Player::set_target_country(Country *country) {
        target_country = country;
        notify();
    }

    void Player::set_battle_number(int battle_number) {
        *this->battle_number = battle_number;
        notify();
    }

    void Player::set_success(bool success) {
        this->success = new bool(success);
        notify();
    }

    void Player::set_number_armies(int armies) {
        *this->number_of_armies = armies;
        notify();
    }

    void Player::set_armies_gained_by_exchange(int gained) {
        *this->number_armies_gained_from_exchange = gained;
        notify();
    }

    void Player::update_reinforce_pair_vector(pair<int, Country *> *int_country) {
        this->reinforce_pair_vector->emplace_back(int_country);
        notify();
    }

    void Player::set_autonomous_fortification_phase_state(Country *source, Country *target, int armies_moved) {
        source_country = source;
        target_country = target;
        *this->number_of_armies = armies_moved;
        notify();
    }

    vector<pair<int, Country*>*> *Player::get_reinforcement_vector() {
        return reinforce_pair_vector;
    }

}


