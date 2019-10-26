//
// Created by ker10 on 2019-10-23.
//

#ifndef TEAM12_DOMINATIONGAME_GAMEENGINEDRIVER_H
#define TEAM12_DOMINATIONGAME_GAMEENGINEDRIVER_H


namespace GameEngine {
    namespace Driver {
        bool map_loaded();
        bool right_amount_of_player();
        bool right_amount_of_card_in_deck();
        bool mutex_country_to_players();
        bool correct_country_players_assignation_count();
        bool run();
    }
};


#endif //TEAM12_DOMINATIONGAME_GAMEENGINEDRIVER_H
