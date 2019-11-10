//
// Created by Francesco Benzi on 2019-11-10.
//

#include <iostream>
#include <vector>
#include <sstream>
#include <string>
#include "../Terminal/Terminal.h"
#include "../GameEngine/GameEngine.h"
#include "GameObservers.h"


//using namespace std;
//using namespace Board;

namespace GameObservers{

    //GameObserver
    //..

    //GameStatsObserver
        //Init
            //Constructs a GameStatsObserver object for a desired game
            GameStatsObserver::GameStatsObserver(GameEngine *game) {
                this->game = GameEngine::GameEngine::instance();
            }
            //Destroys a GameStatsObserver object
            GameStatsObserver::~GameStatsObserver() {
                game->detach(this);
            }

        //Display Methods
            void GameObservers::GameStatsObserver::displayStats() {
                //Display map?
                //Display stats for active players
                    //Countries owned
                    //Continents owned

                    //graph or table depicting what percentage of the world is currently being controlled by each player.

                    //Display cards in hand
            }

            void GameStatsObserver::displayWinner() {
                //Display congrats message to winner
            }

            void GameStatsObserver::displayUpdate() {
                //Clear screen
                //if game ended -> displayWinner()
                //otherwise -> displayStats()
            }

}

