//
// Created by ker10 on 2019-11-13.
//
#define private public

#include "PlayerStrategiesDriver.h"
#include "../GameEngine/GameEngine.h"
#include "./PlayerStrategies.h"
#include "../Terminal/Terminal.h"
#include <cassert>


namespace PlayerStrategies {
    namespace Driver {
        bool test_benevolent_strategy(){
            auto gameInstance = GameEngine::GameEngine::instance();
            gameInstance->start_test(1, 2);
            gameInstance->startup_phase();

            auto player = gameInstance->get_players()->at(0);
            auto player2 = gameInstance->get_players()->at(1);
            auto all_countries = gameInstance->get_map()->get_countries();
            auto first_country = all_countries.at(0);
            auto neighbor_country = first_country->get_neighbors()->at(0);


            player->setPlayerStrategy(new Player::BenevolentPlayerStrategy(player));

            player2->gain_control(all_countries);

            player->gain_control(first_country);
            player->gain_control(neighbor_country);

            first_country->set_armies(10);
            neighbor_country->set_armies(1);

            player->reinforce();

            // Players did transfer stronger to weakest
            assert(neighbor_country->get_armies() == 4);
            assert(first_country->get_armies() == 10);

            player->attack();

            // attack did nothing
            assert(neighbor_country->get_armies() == 4);
            assert(first_country->get_armies() == 10);

            player->fortify();

            // he did split up his strongest country with his weakest
            assert(neighbor_country->get_armies() == 7);
            assert(first_country->get_armies() == 7);

            gameInstance->reset_test();
            return true;
        }
        bool test_aggresive_strategy(){
            auto gameInstance = GameEngine::GameEngine::instance();
            gameInstance->start_test(1, 2);
            gameInstance->startup_phase();

            auto player = gameInstance->get_players()->at(0);
            auto player2 = gameInstance->get_players()->at(1);
            auto all_countries = gameInstance->get_map()->get_countries();
            auto first_country = all_countries.at(0);
            auto neighbor_country = first_country->get_neighbors()->at(0);


            player->setPlayerStrategy(new Player::AggressivePlayerStrategy(player));

            player2->gain_control(all_countries);

            player->gain_control(first_country);
            player->gain_control(neighbor_country);

            first_country->set_armies(10);
            neighbor_country->set_armies(5);

            player->reinforce();

            // Players did reinforce stronger
            assert(neighbor_country->get_armies() == 5);
            assert(first_country->get_armies() == 13);

            for(auto &voisin : * first_country->get_neighbors() ){
               if (voisin->get_owner() != player){
                   voisin->set_armies(999);
               }
            }

            player->attack();

            // attack only with biggest army until it's one
            assert(neighbor_country->get_armies() == 5);
            assert(first_country->get_armies() == 1);

            neighbor_country->set_armies(5);
            first_country->set_armies(13);

            player->fortify();

            // he did sent all his unit to his strongest country
            assert(neighbor_country->get_armies() == 1);
            assert(first_country->get_armies() == 17);

            gameInstance->reset_test();
            return true;
        }
        bool test_random_strategy(){
            auto gameInstance = GameEngine::GameEngine::instance();
            gameInstance->start_test(1, 2);
            gameInstance->startup_phase();

            auto player = gameInstance->get_players()->at(0);
            auto player2 = gameInstance->get_players()->at(1);
            auto all_countries = gameInstance->get_map()->get_countries();
            auto first_country = all_countries.at(0);
            auto neighbor_country = first_country->get_neighbors()->at(0);


            player->setPlayerStrategy(new Player::RandomPlayerStrategy(player));

            player2->gain_control(all_countries);
            player->gain_control(first_country);
            player->gain_control(neighbor_country);

            first_country->set_armies(10);
            neighbor_country->set_armies(5);

            player->reinforce();

            // Players did reinforce one of them
            assert(neighbor_country->get_armies() == 8 || first_country->get_armies() == 13);

            first_country->set_armies(10);
            neighbor_country->set_armies(5);

            for(auto &voisin : * first_country->get_neighbors() ){
                if (voisin->get_owner() != player){
                    voisin->set_armies(999);
                }
            }

            player->attack();

            // attack only with biggest army until it's one
            assert(neighbor_country->get_armies() < 10 || first_country->get_armies() < 5);

            //Verify 5 time just to make sure it's really working
            for (int i = 0; i > 5; i++){
                neighbor_country->set_armies(5);
                first_country->set_armies(13);

                player->fortify();

                bool either_one_country_got_forfitied = neighbor_country->get_armies() > 5 || first_country->get_armies() > 13;
                bool no_fortification_done = neighbor_country->get_armies() == 5 && first_country->get_armies() ==  13;

                assert(either_one_country_got_forfitied || no_fortification_done);
            }

            gameInstance->reset_test();
            return true;
        }

        bool test_cheater_strategy(){
            auto gameInstance = GameEngine::GameEngine::instance();
            gameInstance->start_test(1, 2);
            gameInstance->startup_phase();

            auto player = gameInstance->get_players()->at(0);
            auto player2 = gameInstance->get_players()->at(1);
            auto all_countries = gameInstance->get_map()->get_countries();
            auto first_country = all_countries.at(0);
            auto neighbor_country = first_country->get_neighbors()->at(0);


            player->setPlayerStrategy(new Player::CheaterPlayerStrategy(player));

            player2->gain_control(all_countries);
            player->gain_control(first_country);
            player->gain_control(neighbor_country);

            first_country->set_armies(10);
            neighbor_country->set_armies(5);

            player->reinforce();

            // Players did reinforce double both
            assert(neighbor_country->get_armies() == 10 || first_country->get_armies() == 20);

            first_country->set_armies(10);
            neighbor_country->set_armies(5);

            for(auto &voisin : * first_country->get_neighbors() ){
                if (voisin->get_owner() != player){
                    voisin->set_armies(999);
                }
            }

            player->attack();

            for(auto &voisin : * first_country->get_neighbors() ){
                assert(voisin->get_owner() == player);
            }

            neighbor_country->set_armies(5);
            first_country->set_armies(13);

            player->fortify();

            // verify it doubled the neighbor_country but not the first_country (since first_country doesnt have enemy country around anymore)
            assert(neighbor_country->get_armies() == 10 || first_country->get_armies() == 13);

            gameInstance->reset_test();
            return true;
        }

        bool run(){
            return ( test_random_strategy() && test_cheater_strategy() );
        }
    }
};