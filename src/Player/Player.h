//
// Created by ker10 on 2019-09-26.
//
#include <vector>
#include "../Map/Map.h"
#include "../Cards/Cards.h"
#include "../Dice/Dice.h"

using namespace std;
namespace Player {
// This is not used, but may be used later
    enum player_color {
        RED, BLUE, GREEN, BLACK, GRAY, WHITE
    };

    class Player {

    public:
        string fortify();
        string reinforce();
        string attack();
        Player();
        virtual ~Player();
        // Maybe this should be private. Let's check later.
        Dice::Dice * dice;
        vector<Map::Country*> * countries;
        Cards::Hand * hand;
    };
}
