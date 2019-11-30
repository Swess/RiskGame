/**
Created by ker10 on 2019-11-12.
*/

#include "PlayerStrategies.h"
#include <string>
#include <vector>
#include <sstream>
#include <ctime>
#include <random>
#include "../Terminal/Terminal.h"
#include "../Map/Map.h"
#include "../Cards/Cards.h"
#include "../Player/Player.h"


namespace Player {

    vector<string> get_available_strategies() {
        return {"Human", "Benevolent", "Aggressive"};
    }

    PlayerStrategies::PlayerStrategies() {
        player = nullptr;
        type = nullptr;
    }

    PlayerStrategies::PlayerStrategies(Player * received_player, strategy_type received_type) {
        player = received_player;
        type = new strategy_type(received_type);
    }

    PlayerStrategies::~PlayerStrategies() {
        // do nothing
    }

    HumanPlayerStrategy::HumanPlayerStrategy(Player *player) : PlayerStrategies(player, strategy_type::HUMAN) {

    }

    bool HumanPlayerStrategy::attack() {
        bool got_a_country = false;
        int battle_number = 0;
        while(Terminal::print_select("Player " + this->player->get_color() + ": Do you want to attack this round?")){
            player->clear_phase_state();
            player->set_battle_number(++battle_number);
            if (!this->player->is_able_to_attack()) {
                Terminal::print("Player " + this->player->get_color() + " don't have any country he can attack from." );
                return false;
            }

            Terminal::print("Choose your source country");
            vector<string> selections_source;
            vector<Country* > countries_source = this->player->get_countries_attack_source();
            selections_source.reserve(countries_source.size());
            for (auto country : countries_source){
                selections_source.emplace_back(country->to_string());
            }
            selections_source.emplace_back("Restart the process.");
            int answer_source = Terminal::print_select(selections_source);
            if (answer_source == selections_source.size()-1) { continue; }
            player->set_source_country(countries_source.at(answer_source));

            Terminal::print("Choose your target country");
            vector<string> selections_target;
            vector<Country*> valid_countries;
            for (auto country : * countries_source.at(answer_source)->get_neighbors()){
                if (country->get_owner() == this->player) continue;
                selections_target.emplace_back(country->to_string());
                valid_countries.emplace_back(country);
            }
            selections_target.emplace_back("Restart the process.");
            int answer_target = Terminal::print_select(selections_target);
            if (answer_target == selections_target.size()-1) { continue; }
            Country * source = countries_source.at(answer_source);
            Country * target = valid_countries.at(answer_target);
            player->set_target_country(target);

            // At the end of an attack, if it's a win a attacker has to move in at
            // least as many armies as the number of dice you rolled in its last battle
            int last_roll_attacker = 0;

            last_roll_attacker = battle_and_get_last_roll_amount(source, target);

            if (target->get_armies() == 0 ) {
                got_a_country = true;
                Terminal::print("Congratulations! You decimated your opponents.");
                this->player->gain_control(target);
                string after_attack_question = "How many units do you want to move in your new country?";
                int moving_units = Terminal::print_select(last_roll_attacker, source->get_armies()-1, after_attack_question);
                source->set_armies(source->get_armies() - moving_units);
                target->set_armies(moving_units);
                Terminal::print("The country you started the attack from now is");
                Terminal::print(source->to_string());
                Terminal::print("Your new country now is");
                Terminal::print(target->to_string());
            } else if (source->get_armies() == 1){
                got_a_country = false;
                Terminal::print("Whoops! You can't attack from this country anymore. Better luck next time.");
            }
            player->set_success(got_a_country);
        } // End of attack round
        return got_a_country;
    }

    void HumanPlayerStrategy::reinforce(int new_army) {
        int exchange_gained_exchange = update_army_by_exchange();
        player->set_armies_gained_by_exchange(exchange_gained_exchange);

        new_army+= exchange_gained_exchange;
        reinforce_country(new_army);
    }

    int HumanPlayerStrategy::update_army_by_exchange() const {
        bool trade_again = true;
        int new_army = 0;
        string message;
        if (player->hand->size() < 3) trade_again = false;
        while (trade_again) {
            bool want_to_trade = false;

            if (player->hand->size() > 5) {
                want_to_trade = true;
                message = "You have more than five cards, you must trade now.";
                Terminal::print(message);
            } else if (player->hand->size() >= 3) {
                message = "You have more than three cards, do you want to trade?";
                want_to_trade = Terminal::print_select(message);
            }

            if (want_to_trade) {
                bool exchange_valid;
                do {
                    vector<Cards::Card *> *owned_cards = player->hand->get_cards();
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

                    exchange_valid = player->hand->cardsValidForExchange(ptr);
                    if (!exchange_valid) {
                        Terminal::print("The exchange is not valid, try again.");
                        continue;
                    }

                    int exchange_card = player->hand->exchange(ptr);
                    new_army += exchange_card;
                    Terminal::print("You got " + to_string(exchange_card) + " armies from the exchange.");

                } while(!exchange_valid);

                message = "After trading, you now have " + to_string(new_army) + " units to place.";
                Terminal::print(message);

                if(player->hand->size() < 3) {
                    break;
                } else if (player->hand->size() > 5 ){
                    continue;
                }
                message = "Do you want to trade again?";
                trade_again = Terminal::print_select(message);
            }
        }
        return new_army;
    }

    void HumanPlayerStrategy::reinforce_country(int new_army) {
        while (new_army != 0) {
            auto *reinforce_pair = new pair<int, Country*>;
            vector<Country *> owned_countries = player->get_countries();
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
            reinforce_pair->first = answer;
            reinforce_pair->second = source;
            player->update_reinforce_pair_vector(reinforce_pair);
            new_army -= answer;
        }
    }

    vector<Board::Country *> HumanPlayerStrategy::fortify() {
        vector<Board::Country *> fortificate_country;
        bool validChoice = false;
        int source_country_index;
        Country * source_country;
        vector<string> selections;

        int user_want_fortify = Terminal::print_select("Fortification is possible, would you like to fortify?");
        if (!user_want_fortify) return fortificate_country;

        Terminal::print("Select source country");
        for (auto & country : this->player->get_countries()) {
            selections.push_back(country->to_string_with_neighbors());
        }
        while (!validChoice) {
            source_country_index = Terminal::print_select(selections);
            source_country = this->player->get_countries().at(source_country_index);
            if (source_country->get_armies() > 1) {
                for (auto & country : *source_country->get_neighbors()) {
                    if (this->player == country->get_owner()) {
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
        player->set_source_country(source_country);

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
            if (this->player != target_country->get_owner()) {
                validChoice = false;
                Terminal::print("Must own selected country");
            } else {
                validChoice = true;
            }
        }
        player->set_target_country(target_country);

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
        player->set_number_armies(selected_num_armies);

        source_country->set_armies(source_country->get_armies() - selected_num_armies);
        target_country->set_armies(target_country->get_armies() + selected_num_armies);
        ostringstream os;
        os << selected_num_armies << " armies have been transferred from " << source_country->get_name() << " to " << target_country->get_name();
        Terminal::print(os.str());

        return fortificate_country; // make sure this is empty
    }

    int HumanPlayerStrategy::battle_and_get_last_roll_amount(Country *source, Country *target) const {
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

    HumanPlayerStrategy::~HumanPlayerStrategy() {

    }

    int PlayerStrategies::get_attacker_amount_of_dice(Country *source) const {
        int available_dice_attacker = source->get_armies() - 1;
        // Clamp the amount of attacker's dice to 3
        if (available_dice_attacker > 3) available_dice_attacker = 3;
        return available_dice_attacker;
    }

    void PlayerStrategies::setPlayer(Player * d_player) {
        this->player = d_player;
    }

    string PlayerStrategies::get_type_string() {
        switch (*type) {
            case strategy_type::HUMAN: return "Human";
            case strategy_type::BENEVOLENT: return "Benevolent";
            case strategy_type::AGGRESSIVE: return "Aggressive";
            default: return "ERROR";
        }
    }

    strategy_type PlayerStrategies::get_type() {
        return *type;
    }

    BenevolentPlayerStrategy::BenevolentPlayerStrategy(Player *player) : PlayerStrategies(player, strategy_type::BENEVOLENT) {

    }

    bool BenevolentPlayerStrategy::attack() {
        return false;
    }

    vector<Board::Country *> BenevolentPlayerStrategy::fortify() {
        Terminal::debug("Performing fortify from a benevolent player" + player->get_color());
        // Fortifies in order to move armies to weaker countries
        vector<Board::Country *> answer;
        vector<Board::Country *> owned_countries = player->get_countries();

        //Find weakest country
        int smallest_army = 99999;
        Country * weakest_country = nullptr;
        for (auto &country : owned_countries){
            if (country->get_armies() < smallest_army){
                // Verify if the country has a neighbor that player owns
                for (auto &neighbor_country : * country->get_neighbors()){
                    if (neighbor_country->get_owner() == player){
                        weakest_country = country;
                        smallest_army = country->get_armies();
                        break;
                    }
                }
            }
        }

        if (weakest_country == nullptr){
            return answer;
        }
        // Find the biggest army neighbor to weakest country
        int biggest_army = 0;
        Country * strongest_neighbor = nullptr;
        for (auto &country : * weakest_country->get_neighbors()){
            if (country->get_armies() > biggest_army ){
                biggest_army = country->get_armies();
                strongest_neighbor = country;
            }
        }

        if(strongest_neighbor == nullptr) {
            Terminal::error("Strongest neighbor is null, this should never happen");
            return answer;
        }

        int total_armies = strongest_neighbor->get_armies() + weakest_country->get_armies();
        int total_armies_divided_by_two = total_armies / 2;
        int armies_set;
        if (total_armies % 2 == 0) {
            armies_set = total_armies_divided_by_two;
        } else {
            armies_set = total_armies_divided_by_two + 1;
        }
        int armies_moved = armies_set - weakest_country->get_armies();
        strongest_neighbor->set_armies(armies_set);
        weakest_country->set_armies(total_armies_divided_by_two);

        player->set_autonomous_fortification_phase_state(strongest_neighbor, weakest_country, armies_moved);

        answer.emplace_back(strongest_neighbor); // source
        answer.emplace_back(weakest_country); //target
        return answer;
    }

    void BenevolentPlayerStrategy::reinforce(int i) {
        Terminal::debug("Performing reinforce from a benevolent player" + player->get_color());

        vector<Board::Country *> owned_countries = player->get_countries();
        //Find weakest country
        int smallest_army = 99999;
        Country * weakest_country = nullptr;
        for (auto &country : owned_countries){
            if (country->get_armies() < smallest_army){
                weakest_country = country;
                smallest_army = country->get_armies();
            }
        }

        if(weakest_country == nullptr) {
            Terminal::error("Weakest country is null, this should NEVER happen");
            return;
        }
        auto reinforce_pair = new pair<int, Country*>;
        reinforce_pair->first = i;
        reinforce_pair->second = weakest_country;
        player->update_reinforce_pair_vector(reinforce_pair);

        weakest_country->set_armies(weakest_country->get_armies() + i);

    }

    BenevolentPlayerStrategy::~BenevolentPlayerStrategy() {

    }

    /**
 * an aggressive computer player that focuses on attack (reinforces its strongest
country, then always attack with it until it cannot attack anymore, then fortifies in order to maximize aggregation of
forces in one country),
 * @param i
 */

    bool AggressivePlayerStrategy::attack() {
        bool has_won_a_battle = false;
        //Find strongest country
        int biggest_army = 0;
        Country * strongest_country = nullptr;
        for (auto &country : player->get_countries()){
            if (country->get_armies() > biggest_army){
                // Verify if the country has a neighbor that any other player owns and has army
                for (auto &neighbor_country : * country->get_neighbors()){
                    if (neighbor_country->get_owner() != player && neighbor_country->get_armies() > 1){
                        strongest_country = country;
                        biggest_army = country->get_armies();
                        break;
                    }
                }
            }
        }

        if (strongest_country == nullptr){
            Terminal::error("Player " + player->get_color() + " has no country to attack from");
            return has_won_a_battle;
        }

        while (strongest_country->get_armies() > 1){
            Board::Country * country_under_attack = nullptr;
            for (auto &neighbor : *strongest_country->get_neighbors()){
                if (neighbor->get_owner() != this->player) {
                    country_under_attack = neighbor;
                    break;
                }
            }
            if (country_under_attack == nullptr) return has_won_a_battle;

            int last_roll = battle_and_get_last_roll_amount(strongest_country, country_under_attack);

            if (country_under_attack->get_armies() == 0 ) {
                has_won_a_battle = true;
                this->player->gain_control(country_under_attack);

                int moving_units = strongest_country->get_armies()-1;
                strongest_country->set_armies(strongest_country->get_armies() - moving_units);
                country_under_attack->set_armies(moving_units);
            } else if (strongest_country->get_armies() == 1){
                Terminal::debug("Player " + player->get_color() + " can't attack from this country anymore.");
            }

            return has_won_a_battle;

        }


        return false;
    }

    vector<Board::Country *> AggressivePlayerStrategy::fortify() {
        Terminal::debug("Performing fortify from a aggresive player" + player->get_color());
        // Fortifies in order to move armies to weaker countries
        vector<Board::Country *> answer;
        vector<Board::Country *> owned_countries = player->get_countries();

        //Find strongest country
        int biggest_army = 0;
        Country * strongest_country = nullptr;
        for (auto &country : owned_countries){
            if (country->get_armies() > biggest_army){
                // Verify if the country has a neighbor that player owns and has army
                for (auto &neighbor_country : * country->get_neighbors()){
                    if (neighbor_country->get_owner() == player && neighbor_country->get_armies() > 1){
                        strongest_country = country;
                        biggest_army = country->get_armies();
                        break;
                    }
                }
            }
        }

        if (strongest_country == nullptr){
            return answer;
        }

        // Find the biggest army neighbor to weakest country
        biggest_army = 0;
        Country * strongest_neighbor = nullptr;
        for (auto &country : * strongest_country->get_neighbors()){
            if (country->get_armies() > biggest_army && country->get_owner() == player){
                biggest_army = country->get_armies();
                strongest_neighbor = country;
            }
        }

        if(strongest_neighbor == nullptr) {
            Terminal::error("Strongest neighbor is null, this should never happen");
            return answer;
        }

        int total_armies = strongest_neighbor->get_armies() + strongest_country->get_armies();
        player->set_number_armies(strongest_neighbor->get_armies() - 1);
        strongest_neighbor->set_armies(1);
        strongest_country->set_armies(total_armies-1);


        answer.emplace_back(strongest_neighbor); // source
        answer.emplace_back(strongest_country); //target
        return answer;
    }

    void AggressivePlayerStrategy::reinforce(int i) {
        Terminal::debug("Performing reinforce from a aggresive player" + player->get_color());

        vector<Board::Country *> owned_countries = player->get_countries();
        //Find weakest country
        int big_army = 0;
        Country * strongest_country = nullptr;
        for (auto &country : owned_countries){
            if (country->get_armies() > big_army){
                strongest_country = country;
                big_army = country->get_armies();
            }
        }


        if(strongest_country == nullptr) {
            Terminal::error("Strongest country is null, this should NEVER happen");
            return;
        }

        auto reinforce_pair = new pair<int, Country*>;
        reinforce_pair->first = i;
        reinforce_pair->second = strongest_country;
        player->update_reinforce_pair_vector(reinforce_pair);
        strongest_country->set_armies(strongest_country->get_armies() + i);
    }

    int AggressivePlayerStrategy::battle_and_get_last_roll_amount(Board::Country *source, Board::Country *target) const {
        int last_roll_attacker = 0;
        while (source->get_armies() > 1 && target->get_armies() > 0 ) {
            Terminal::debug("The country aggressive player  " + player->get_color() + " started the attack from is");
            Terminal::debug(source->to_string());
            Terminal::debug("The country aggressive player " + player->get_color() + " is are attacking is");
            Terminal::debug(target->to_string());

            int available_dice_attacker = get_attacker_amount_of_dice(source);
            int nb_of_dice_attacker = available_dice_attacker;
            if (nb_of_dice_attacker == 0) { break; }

            int available_dice_defender = target->get_armies();
            // Clamp the amount of defender's dice to 2
            if (available_dice_defender > 2) available_dice_defender = 2;
            int nb_of_dice_defender = available_dice_defender;


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

    AggressivePlayerStrategy::AggressivePlayerStrategy(Player *player) : PlayerStrategies(player, strategy_type::AGGRESSIVE) {

    }

    AggressivePlayerStrategy::~AggressivePlayerStrategy() {

    }

    RandomPlayerStrategy::RandomPlayerStrategy(Player *player) : PlayerStrategies(player) {

    }

    RandomPlayerStrategy::~RandomPlayerStrategy() {

    }

    bool RandomPlayerStrategy::attack() {
        // attacks a random number of times a random country
        bool has_won_a_battle = false;
        int max_num_of_targets = 15;
        int num_of_attack = rand() % (max_num_of_targets+1);

        while(num_of_attack > 0){
            // This may change after each attack
            vector<Country*> countries_source = player->get_countries_attack_source();

            // Possible that we are not able to attack anymore
            if(countries_source.empty()) return has_won_a_battle;

            int source_index = rand() % countries_source.size();
            Country* source_country = countries_source.at(source_index);

            vector<Country*> avail_targets;
            for(Country* target : *source_country->get_neighbors() ){
                if(target->get_owner() != player){
                    avail_targets.push_back(target);
                }
            }

            if(avail_targets.empty()) return true;

            int target_index = rand() % avail_targets.size();
            Board::Country * country_under_attack = avail_targets.at(target_index);

            int last_roll = battle_and_get_last_roll_amount(source_country, country_under_attack);

            if (country_under_attack->get_armies() == 0 ) {
                has_won_a_battle = true;
                this->player->gain_control(country_under_attack);

                int moving_units = source_country->get_armies()-1;
                source_country->set_armies(source_country->get_armies() - moving_units);
                country_under_attack->set_armies(moving_units);
            }

            num_of_attack--;
        }

        return has_won_a_battle;
    }

    vector<Board::Country *> RandomPlayerStrategy::fortify() {
        Terminal::debug("Performing fortify from player" + player->get_color() + " who is playing randomly.");
        // Fortifies in order to move armies to weaker countries
        vector<Board::Country *> answer;
        vector<Board::Country *> owned_countries = player->get_countries();

        // Find possible source countries
        vector<Country *> possible_sources;
        for (auto &country : owned_countries){
            // Verify if the country has a neighbor that player owns
            for (auto &neighbor_country : * country->get_neighbors()){
                if (neighbor_country->get_owner() == player){
                    possible_sources.push_back(country);
                    break;
                }
            }
        }

        if (possible_sources.empty())
            return answer;

        // Choose source
        Country* source_c = possible_sources.at(rand() % possible_sources.size());

        // Find possible target for selected source
        int biggest_army = 0;
        vector<Country *> possible_targets;
        for (auto &country : *source_c->get_neighbors()){
            if (country->get_owner() == player )
                possible_targets.push_back(country);
        }

        if(possible_targets.empty()) {
            Terminal::error("Could not select a target to reinforce AFTER selecting a source. Should never happen.");
            return answer;
        }

        // Choose target
        Country* target_c = possible_targets.at(rand() % possible_targets.size());

        int armies_moved = rand() % source_c->get_armies();
        target_c->set_armies(target_c->get_armies() + armies_moved);
        source_c->set_armies(source_c->get_armies() - armies_moved);

        player->set_autonomous_fortification_phase_state(target_c, source_c, armies_moved);

        answer.emplace_back(target_c); // source
        answer.emplace_back(source_c); // target
        return answer;
    }

    void RandomPlayerStrategy::reinforce(int i) {
        Terminal::debug("Performing reinforce from player "+player->get_color()+" playing randomly.");

        vector<Board::Country *> owned_countries = player->get_countries();
        int index = rand() % owned_countries.size();
        Country* to_reinforce = owned_countries.at(index);

        auto reinforce_pair = new pair<int, Country*>;
        reinforce_pair->first = i;
        reinforce_pair->second = to_reinforce;

        player->update_reinforce_pair_vector(reinforce_pair);
        to_reinforce->set_armies(to_reinforce->get_armies() + i);
    }

    int RandomPlayerStrategy::battle_and_get_last_roll_amount(Board::Country *source, Board::Country *target) const {
        int last_roll_attacker = 0;

        // Random Player rolls only once, but will attack from random source to random target, a random times.
        // So this may be called multiple times in the same turn.

        Terminal::debug("The country random player  " + player->get_color() + " started the attack from is");
        Terminal::debug(source->to_string());
        Terminal::debug("The country random player " + player->get_color() + " is are attacking is");
        Terminal::debug(target->to_string());

        int available_dice_attacker = get_attacker_amount_of_dice(source);
        int nb_of_dice_attacker = rand() % (available_dice_attacker + 1);
        if (nb_of_dice_attacker == 0) { nb_of_dice_attacker=1; }

        int available_dice_defender = target->get_armies();
        // Clamp the amount of defender's dice to 2
        if (available_dice_defender > 2) available_dice_defender = 2;
        int nb_of_dice_defender = available_dice_defender;

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

        return last_roll_attacker;
    }

    CheaterPlayerStrategy::CheaterPlayerStrategy(Player *player) : PlayerStrategies(player) {

    }

    CheaterPlayerStrategy::~CheaterPlayerStrategy() {

    }

    bool CheaterPlayerStrategy::attack() {
        vector<Board::Country *> current_country = player->get_countries();

        for (auto country : current_country){
            for (auto neighbor : *country->get_neighbors()){
                if (neighbor->get_owner() == player) continue;
                neighbor->set_armies(1);
                player->gain_control(neighbor);
            }
        }

        return true;
    }

    vector<Board::Country *> CheaterPlayerStrategy::fortify() {

        auto all_countries = player->get_countries();
        for (auto country : all_countries){ // Check all his country
            for (auto neighbor : *country->get_neighbors()) { // Check his neighbor
                if (neighbor->get_owner() != player ) { // If a neighbor isnt us
                    country->set_armies(country->get_armies()*2); // double the army
                    break;
                }
            }
        }

        //Return an empty vector
        return vector<Board::Country *>();
    }

    void CheaterPlayerStrategy::reinforce(int i) {
        auto all_countries = player->get_countries();
        for (auto country : all_countries){
            country->set_armies(country->get_armies()*2);
        }
    }

    int CheaterPlayerStrategy::battle_and_get_last_roll_amount(Board::Country *source, Board::Country *target) const {
        return 0;
    }
}