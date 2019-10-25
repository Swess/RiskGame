//
// Created by ker10 on 2019-10-23.
//

#include "GameEngine.h"
#include <windows.h>
#include <ctime>
#include <algorithm>
#include <random>
#include "../Map/Map.h"
#include "../MapLoader/MapLoader.h"
#include "../Terminal/Terminal.h"
#include "../exceptions.h"

using std::to_string;

namespace GameEngine {
    GameEngine *GameEngine::game_engine_instance = nullptr;

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
                    map_name = map_name.substr(0, extension);
                    results.emplace_back(map_name);
                }
            } while( FindNextFile(h_find, &fd)) ;
            FindClose(h_find);
        }

        return results;
    }

    void GameEngine::select_map() {
        MapLoader::MapLoader mapLoader;
        Terminal::print("Select the map you want to play in.");
        vector<string> maps = get_available_map();

        int map_selection;
        bool found_valid = false;

        while(!found_valid){
            try {
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

        for (int i = 1; i < answer ; ++i) {
            Player::Player * p = new Player::Player;
            players->emplace_back(p);

            // Place player pointers in the registry also
            map_registry->add_player(p);
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
        nb_player -= 1; // needed since 3 players = choice nb 2;
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
        map_registry = new Map::MapRegistry();
    }

    GameEngine::~GameEngine() {
        delete map;
        delete players;
        delete deck;
        delete player_order;
        delete map_registry;

        map_registry = nullptr;
        player_order = nullptr;
        deck = nullptr;
        map = nullptr;
        players = nullptr;
    }

    Map::Map * GameEngine::get_map() {
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
            Terminal::debug("Player " + to_string(order+1));
    }

    void GameEngine::assign_country_to_player() {
        // Loop countries with player index increasing
        vector<Map::Country *> countries = map->get_countries();

        shuffle( countries.begin(), countries.end(), default_random_engine(time(0)));

        int owner_index = 0;
        for(auto & countrie : countries){
            // Giving in Round-Robin
            map_registry->gain_control(players->at(owner_index), countrie);
            Terminal::debug("Gave control of country '"+countrie->get_name()+"' to player "+to_string(owner_index) );
            owner_index = ++owner_index % (int)players->size();
        }
    }

    void GameEngine::assign_armies_into_country() {

    }

}