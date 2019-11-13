//
// Created by ker10 on 2019-10-10.
//

#ifndef TEAM12_DOMINATIONGAME_PLAYERDRIVER_H
#define TEAM12_DOMINATIONGAME_PLAYERDRIVER_H


namespace Player {
    namespace Driver {
        bool test_amount_of_dice();
        bool test_attacking_and_attacked_country_validity();
        bool test_deducted_armies();
        bool test_multiple_attacks();
        bool test_attack();
        bool test_fortify();
        bool test_reinforce();
        bool run();
        void case_1_country_9_cards_2_exchanges();
        void case_1_country_8_cards_2_exchanges();
        void case_all_country_0_cards();
    }
};


#endif //TEAM12_DOMINATIONGAME_PLAYERDRIVER_H
