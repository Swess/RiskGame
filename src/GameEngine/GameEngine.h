//
// Created by ker10 on 2019-10-23.
//

#ifndef TEAM12_DOMINATIONGAME_GAMEENGINE_H
#define TEAM12_DOMINATIONGAME_GAMEENGINE_H

#include <vector>
#include <string>
#include "../Map/Map.h"
#include "../Player/Player.h"
#include "../Cards/Cards.h"

using std::string;
using std::vector;

namespace GameEngine {

    class GameEngine {
    public:
        /**
         * Gets the Singleton instance
         * @return
         */
        static GameEngine *instance();

        /**
         * Starting point of the engine/game
         */
        void start();
        void start_test(int map_index, int nb_player);
        void startup_phase();
        void game_loop();
        void reset_test(); // ONLY USE FOR TESTS
        void assign_player_order_randomly();
        void assign_country_to_player();
        void assign_armies_into_country();
        bool is_game_done();
        Board::Map * get_map();
        vector<Player::Player*> * get_players();
        Cards::Deck * get_deck();
        vector<int> * get_player_order();
    private:
        GameEngine();
        virtual ~GameEngine();

        /**
         * Returns the list of all .map files available for loading.
         * @return
         */
        vector<string> get_available_map();
        /**
         * Let's the player select the map tha the game will be on.
         * Load the resources accordingly, and create objects.
         */
        void select_map();
        void select_player();
        void create_deck();
        bool game_done();
        static GameEngine *game_engine_instance;
        Board::Map *map;
        vector<Player::Player*> * players;
        Cards::Deck * deck;
        vector<int> * player_order;
    };


}
#endif //TEAM12_DOMINATIONGAME_GAMEENGINE_H
