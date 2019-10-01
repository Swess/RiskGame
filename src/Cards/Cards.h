//
// Created by Noah on 2019-09-26.
//

#ifndef TEAM12_DOMINATIONGAME_CARDS_H
#define TEAM12_DOMINATIONGAME_CARDS_H

#pragma once
#include <string>

using namespace std;

class Card {
public:
    enum Type {infantry = 0, artillery = 1, cavalry = 2};
    Card();
    Card(string*, Type*);

private:
    string *country;
    Type *type;
};


class Deck {
public:
    Deck(string countries[], int *size);
    Card* draw();

private:
    int *size;
    Card *cards; // Array of cards
    int *currentCardIndex;
    void shuffleDeck();

    void createCards(string *countries);
};

class Hand {
public:
    Hand();
    Card::Type *exchange();

private:
    Card* cards;
};



#endif //TEAM12_DOMINATIONGAME_CARDS_H