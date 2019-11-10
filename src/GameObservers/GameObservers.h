//
// Created by Francesco Benzi on 2019-11-10.
//

#ifndef TEAM12_DOMINATIONGAME_GAMEOBSERVERS_H
#define TEAM12_DOMINATIONGAME_GAMEOBSERVERS_H

#include <vector>
#include "../Terminal/Terminal.h"
#include "../GameEngine/GameEngine.h"

namespace GameObservers {

    //class GameObserver{};

    class GameStatsObserver{

        private:
            //Variables to observe
                //Gathering game to Observe
                GameEngine* game;
                //Gathering currently selected map
                Map* map;
                //Gathering active players
                std::vector<Player*> players;

            //Display Methods
                //Statistics displayer
                void displayStats();
                //Congratulations displayer
                void displayWinner();

        public:
            //ObserverStats constructor
            GameStatsObserver(GameEngine* game);

            //ObserverStats destructor
            ~GameStatsObserver();

            //Display updater
            void displayUpdate();
    };
}
#endif //TEAM12_DOMINATIONGAME_GAMEOBSERVERS_H
