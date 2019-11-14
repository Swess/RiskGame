//
// Created by ker10 on 2019-10-23.
//

#include "GameEngine.h"
#include <windows.h>
#include <ctime>
#include <algorithm>
#include <random>
#include <sstream>
#include "../MapLoader/MapLoader.h"
#include "../Terminal/Terminal.h"
#include "../exceptions.h"

using std::to_string;

namespace GameEngine {
    GameEngine *GameEngine::game_engine_instance = nullptr;

    void callback(Observer::PlayerSubject *player) {
        ostringstream os;
        os << endl;
        os << "////////////////////////////////////\n";
        os << player->get_color() << " player: " << player->get_phase_string() << " phase\n\n";
        if (player->get_battle_number() > 0) {
            os << "Battle number: " << player->get_battle_number() << "\n";
        }
        if (player->get_number_armies_used() > 0) {
            os << "Number of armies used: " << player->get_number_armies_used() << "\n";
        }
        if (player->get_armies_gained_by_continent_owned() >= 0) {
            os << "number of armies gained from continents owned: " << player->get_armies_gained_by_continent_owned() << endl;
        }
        if (player->get_armies_gained_by_country_owned() >= 0) {
            os << "number of armies gained from country owned: " << player->get_armies_gained_by_country_owned() << endl;
        }
        if (player->get_armies_gained_by_exchange() >= 0) {
            os << "number of armies gained from exchange: " << player->get_armies_gained_by_exchange() << endl;
            int sum = player->get_armies_gained_by_exchange() + player->get_armies_gained_by_country_owned() + player->get_armies_gained_by_continent_owned();
            os << "Total number of armies gained: " << sum << endl;
        }
        if (!player->get_reinforcement_vector()->empty()) {
            for (auto int_country : *player->get_reinforcement_vector()) {
                os << "Country: " << int_country->second->get_name() << " reinforced with " << int_country->first << " armies" << endl;
            }
        }
        if (player->get_source_country()) {
            os << "Source country: " << player->get_source_country()->get_name() << "\n";
        }
        if (player->get_target_country()) {
            os << "Target country: " << player->get_target_country()->get_name() << "\n";
        }
        if (player->get_success()) {
            string result = player->get_success() ? "success" : "failure";
            os << "The attack was a " << result << "\n";
        }
        os << "////////////////////////////////////\n";
        Terminal::print(os.str());
    }

    GameEngine *GameEngine::instance() {
        if (!game_engine_instance) game_engine_instance = new GameEngine;
        return game_engine_instance;
    }

    /*
     * get_available_map list the ressources/maps/ directory and removes the .map from the file
     * It uses windows.h :: FindNextFile() to traverse the directory.
     *
     * TODO: make it cross-platform with mac since some team member uses it
     *
     * https://docs.microsoft.com/en-us/windows/win32/api/fileapi/nf-fileapi-findnextfilea
     */
    vector<string> GameEngine::get_available_map() {
        vector<string> results;

        string search_path = "ressources/maps/*.*";
        WIN32_FIND_DATA fd;
        HANDLE h_find = FindFirstFile(search_path.c_str(), &fd);
        if(h_find != INVALID_HANDLE_VALUE) {
            do {
                if(! (fd.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY) ) {
                    string map_name = fd.cFileName;
                    size_t extension = map_name.find(".map");

                    // If not of extension .map
                    if(extension >= map_name.length())
                        continue;

                    map_name = map_name.substr(0, extension);
                    results.emplace_back(map_name);
                }
            } while( FindNextFile(h_find, &fd));
            FindClose(h_find);
        }

        return results;
    }

    void GameEngine::select_map() {
        Terminal::print("Select the map you want to play in.");
        vector<string> maps = get_available_map();

        int map_selection;
        bool found_valid = false;

        while(!found_valid){
            try {
                MapLoader::MapLoader mapLoader;
                map_selection = Terminal::print_select(maps);
                map = mapLoader.load(map_selection)->build();

                if(!map->is_connected()){
                    Terminal::print("The selected does not represent a valid connected graph and playable map.");
                    Terminal::print("Please select another one.");
                    continue;
                }

                found_valid = true;
            } catch (IOException &e) {
                string error = e.what(maps[map_selection]);
                Terminal::error(error);
            } catch(runtime_error &e) {
                string error = e.what();
                Terminal::error(error);
            } catch(exception &e){
                string error = e.what();
                Terminal::error(error);
            }
        }

        Terminal::debug("Map defined in GameEngine");
    }

    void GameEngine::create_deck() {
        Terminal::debug("Creating the deck");

        vector<string> countries_name;

        for (auto * country : map->get_countries())
            countries_name.emplace_back(country->get_name());

        deck = new Cards::Deck(countries_name);

        Terminal::debug("Finish deck");
    }

    void GameEngine::select_player() {
        Terminal::print("How many player do you want to play with ?");

        vector<string> choices{
            "Two players",
            "Three players",
            "Four players",
            "Five players",
            "Six players",
        };

        int answer = Terminal::print_select(choices);
        answer += 2; // Shift the answer since it start with at least two player ( 0th index = 2 player, 1st index = 3 player)

        Terminal::debug("Initializing " + to_string(answer) + " players.");

        for (int i = 1; i <= answer ; ++i) {
            Player::Player * p = new Player::Player;
            Observer::PlayerObserver *observer = new Observer::PlayerObserver(p, &callback);
            player_observers->emplace_back(observer);
            players->emplace_back(p);
        }

        Terminal::debug("Players all defined in GameEngine");
    }

    void GameEngine::start() {
        Terminal::debug("Game Engine Starting...");
        select_map();
        create_deck();
        select_player();
        Terminal::debug("Game Engine Done with start part.");
    }

    void GameEngine::start_test(int map_index, int nb_player) {
        nb_player -= 2;
        Terminal::debug("Game Engine Starting...");
        Terminal::set_input(map_index);
        select_map();
        create_deck();
        Terminal::set_input(nb_player);
        select_player();
        Terminal::debug("Game Engine Done.");
    }

    GameEngine::GameEngine() {
        map = nullptr;
        deck = nullptr;
        players = new vector<Player::Player *>();
        player_order = new vector<int>();
        player_observers = new vector<Observer::PlayerObserver*>();
    }

    GameEngine::~GameEngine() {
        delete map;
        delete players;
        delete deck;
        delete player_order;
        player_order = nullptr;
        deck = nullptr;
        map = nullptr;
        players = nullptr;
    }

    Map * GameEngine::get_map() {
        return map;
    }

    vector<Player::Player *> * GameEngine::get_players() {
        return players;
    }

    Cards::Deck *GameEngine::get_deck() {
        return deck;
    }

    void GameEngine::reset_test() {
        delete map;
        delete players;
        delete deck;
        delete player_order;
        map = nullptr;
        deck = nullptr;
        player_order = new vector<int>();
        Player::Player::reset_player_count();
        players = new vector<Player::Player *>();
    }

    vector<int> *GameEngine::get_player_order() {
        return player_order;
    }

    void GameEngine::startup_phase() {
        assign_player_order_randomly();
        assign_country_to_player();
        assign_armies_into_country();
    }

    void GameEngine::assign_player_order_randomly() {
        Terminal::debug("Determining player order");

        // Seeding random function for unpredictable results
        srand(time(0));

        for (auto player : *players){
            bool unique = true;
            int order = 0;
            do {
                order = rand() % players->size();
                unique = true;
                for (auto i : *player_order) {
                    if (order == i) {
                        unique = false;
                        break;
                    }
                }
            } while(!unique);
            player_order->emplace_back(order);
        }

        Terminal::debug("The order is");
        for (auto order : * player_order)
            Terminal::debug("Player " + players->at(order)->get_color());
    }

    void GameEngine::assign_country_to_player() {
        // Loop countries with player index increasing
        vector<Country *> countries = map->get_countries();

        shuffle( countries.begin(), countries.end(), default_random_engine(time(0)));

        int owner_index = 0;
        for(auto & country : countries){
            // Giving in Round-Robin
            players->at(owner_index)->gain_control(country);

            Terminal::debug("Gave control of country '" + country->get_name()+ "' to player " + players->at(owner_index)->get_color() );
            owner_index = ++owner_index % (int)players->size();
        }
    }

    void GameEngine::assign_armies_into_country() {
        int total_placed = 0;
        int armies_allowed = 40 - (players->size()-2)*5;

        // Init players specific counter
        int remaining[players->size()];
        for(int i=0; i < players->size(); i++)
            remaining[i] = armies_allowed;

        // Auto place 1 army to every country
        for(int player_index : *player_order){
            vector<Country *> countries = players->at(player_index)->get_countries();
            for (Country *c : countries) {
                c->set_armies(c->get_armies() + 1);
                remaining[player_index]--;
                total_placed++;
            }
        }

        Terminal::print("An single army have been placed on all of your countries.");

        // Let players place their remaining armies one at the time.
        while(total_placed < players->size()*armies_allowed){
            for(int player_index : *player_order){
                // Player may have placed all his armies already
                if(remaining[player_index] <= 0) continue;
                Player::Player * current_player = players->at(player_index);

                Terminal::print("");
                Terminal::print("It is now the turn of Player "+ current_player->get_color());

                // Build list of choices with current amount of armies for display
                vector<Country *> countries = current_player->get_countries();
                vector<string> options;
                for(auto c_ptr : countries)
                    options.emplace_back(c_ptr->get_name() + " ("+to_string(c_ptr->get_armies())+" armies present)");

                // Actually ask player
                int answer = Terminal::print_select(options);

                // Process
                countries.at(answer)->set_armies(countries.at(answer)->get_armies()+1);
                remaining[player_index]--;
                total_placed++;
            }
        }

    }

    void GameEngine::game_loop() {
        Terminal::debug("Starting game loop.");
        while (!game_done()) {
            for(int player_index : *player_order){
                if (game_done()) continue;
                Player::Player * current_player = players->at(player_index);
                if (current_player->is_player_dead()) continue;
                current_player->turn();
            }
        }
    }

    bool GameEngine::game_done() {
        for(int player_index : *player_order){
            if (players->at(player_index)->get_countries().size() == map->get_countries().size()) {
                Terminal::debug("There is a winner.");
                return true;
            }
        }
        return false;
    }

    bool GameEngine::is_game_done() {
        return game_done();
    }

}
