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
    };

    Player::~Player() {
        delete hand;
        delete dice;
        delete countries;
        delete color;
        color = nullptr;
        countries = nullptr;
        hand = nullptr;
        dice = nullptr;
    }

    void Player::fortify() {
        Terminal::debug("Player fortify");
        Terminal::print("Fortification phase");
        vector<string> selections;
        bool validChoice = false;
        int source_country_index;
        Country * source_country;

        bool can_fortify = player_can_fortify();

        if (!can_fortify) {
            Terminal::print("Fortification is not possible, you do not own two valid neighboring countries");
            return;
        }

        int user_want_fortify = Terminal::print_select("Fortification is possible, would you like to fortify?");
        if (!user_want_fortify) return;

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
        new_army = update_army_by_exchange(new_army);

        reinforce_country(new_army);

        Terminal::print("You don't have any units to place anymore.");

    }

    int Player::update_army_by_exchange(int new_army) const {
        bool trade_again = true;
        string message;
        if (hand->size() < 3) trade_again = false;
        while (trade_again) {
            bool want_to_trade = false;

            if (hand->size() > 5) {
                want_to_trade = true;
                message = "You have more than five cards, you must trade now.";
                Terminal::print(message);
            } else if (hand->size() >= 3) {
                message = "You have more than three cards, do you want to trade?";
                want_to_trade = Terminal::print_select(message);
            }

            if (want_to_trade) {
                bool exchange_valid;
                do {
                    vector<Cards::Card *> *owned_cards = hand->get_cards();
                    vector<string> selections;
                    for (auto card : *owned_cards) {
                        selections.emplace_back(card->to_string());
                    }

                    message = "Which card do you want to use? Select three.";
                    Terminal::print(message);
                    int first_card = Terminal::print_select(selections);
                    int second_card;
                    int third_card;
                    do {
                        second_card = Terminal::print_select(selections);
                        if (second_card == first_card) Terminal::print("You can't take the same card, try again.");
                    } while (second_card == first_card);
                    do {
                        third_card = Terminal::print_select(selections);
                        if (third_card == first_card || third_card == second_card)
                            Terminal::print("You can't take the same card, try again.");
                    } while (third_card == first_card || third_card == second_card);

                    int cards_to_trade[3] = {first_card, second_card, third_card};
                    int *ptr = cards_to_trade;

                    exchange_valid = hand->cardsValidForExchange(ptr);
                    if (!exchange_valid) {
                        Terminal::print("The exchange is not valid, try again.");
                        continue;
                    }

                    int exchange_card =  hand->exchange(ptr);
                    new_army += exchange_card;
                    Terminal::print("You got " + to_string(exchange_card) + " armies from the exchange.");

                } while(!exchange_valid);

                message = "After trading, you now have " + to_string(new_army) + " units to place.";
                Terminal::print(message);

                if(this->hand->size() < 3) {
                    break;
                } else if (this->hand->size() > 5 ){
                    continue;
                }
                message = "Do you want to trade again?";
                trade_again = Terminal::print_select(message);
            }
        }
        return new_army;
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

    void Player::reinforce_country(int new_army) {

        while (new_army != 0) {
            vector<Country *> owned_countries = get_countries();
            vector<string> selections;
            selections.reserve(owned_countries.size());
            for (auto country : owned_countries){
                selections.emplace_back(country->to_string());
            }
            string message = "Which country do you want to reinforce?";
            Terminal::print(message);
            int country_index = Terminal::print_select(selections);
            Country * source = owned_countries.at(country_index);
            message = "How many units?";
            int answer = Terminal::print_select(1, new_army, message);
            source->set_armies(source->get_armies()+answer);
            new_army -= answer;
        }
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

            last_roll_attacker = battle_and_get_last_roll_amount(source, target);

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

    int Player::battle_and_get_last_roll_amount(Country *source, Country *target) const {
        int last_roll_attacker = 0;
        while (source->get_armies() > 1 && target->get_armies() > 0 ) {
            Terminal::print("The country you started the attack from is");
            Terminal::print(source->to_string());
            Terminal::print("The country you are attacking is");
            Terminal::print(target->to_string());

            int available_dice_attacker = get_attacker_amount_of_dice(source);
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

            // Remove no more then lower nb of dices
            int how_many_unit_will_die = nb_of_dice_attacker < nb_of_dice_defender ? nb_of_dice_attacker
                                                                                   : nb_of_dice_defender;

            for (int i = 0; i < how_many_unit_will_die; i++) {
                roll_attacker[i] > roll_defender[i] ? target->decrement_army() : source->decrement_army();
            }

            last_roll_attacker = nb_of_dice_attacker;

        } // End of a battle

        return last_roll_attacker;
    }

    int Player::get_attacker_amount_of_dice(Country *source) const {
        int available_dice_attacker = source->get_armies() - 1;
        // Clamp the amount of attacker's dice to 3
        if (available_dice_attacker > 3) available_dice_attacker = 3;
        return available_dice_attacker;
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

        this->reinforce();
        GameEngine::GameEngine::instance()->output_state_to_file();
        this->attack();
        GameEngine::GameEngine::instance()->output_state_to_file();
        this->fortify();
        GameEngine::GameEngine::instance()->output_state_to_file();

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

}
