//
// Created by Noah Horowitz on 11/9/2019.
//

#pragma once

#include <vector>
#include <string>
//#include "../Player/Player.h"
//#include "../Map/Map.h"


using namespace std;
namespace Player {class Player;}
namespace Board {class Country;}

namespace Observer {

    /**
     * Subject class to be extended to
     */
    class Subject{
    public:
        Subject();
        ~Subject();
        void attach(class Observer *observer);
        void detach(Observer *observer);
        void notify();
    private:
        vector<Observer*>  *observers;
    };

    class Observer {
    public:
        Observer(Subject *model);
        virtual void update() = 0;
    private:
    };

    class PlayerSubject : public Subject {
    public:
        virtual string get_color() = 0;
        virtual string get_phase_string() = 0;
        virtual Board::Country *get_source_country() = 0;
        virtual Board::Country *get_target_country() = 0;
        virtual int get_battle_number() = 0;
        virtual bool *get_success() = 0;
        virtual int get_number_armies_used() = 0;
        virtual int get_armies_gained_by_country_owned() = 0;
        virtual int get_armies_gained_by_continent_owned() = 0;
        virtual int get_armies_gained_by_exchange() = 0;
        virtual vector<pair<int, Board::Country*>*> *get_reinforcement_vector() = 0;

    };

    class PlayerObserver : public Observer {
    public:
        PlayerObserver(PlayerSubject *model, void (*display_callback)(PlayerSubject*));
        ~PlayerObserver();
        void update() override;
        void (*display_callback)(PlayerSubject*);
    private:
        PlayerSubject *model;
    };

    class GameStateSubject : public Subject {
    public:
        virtual vector<Player::Player *> *get_players_in_game() = 0;
        virtual bool is_game_over() = 0;
        virtual Player::Player *get_winner() = 0;
    };

    class GameStateObserver : Observer {
    public:
        GameStateObserver(GameStateSubject *model, void (*display_callback)(GameStateSubject*));
        ~GameStateObserver();
        void update() override;
        void (*display_callback)(GameStateSubject*);
    private:
        GameStateSubject *model;
    };
}

