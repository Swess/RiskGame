//
// Created by Noah on 2019-09-26.
//

#ifndef TEAM12_DOMINATIONGAME_CARDS_H
#define TEAM12_DOMINATIONGAME_CARDS_H

#pragma once
#include <string>
#include <vector>

using namespace std;

class Card {
public:
    enum Type {INFANTRY = 0, ARTILLERY = 1, CAVALRY = 2};
    Card();
    Card(string country, Type type);
//    ~Card();
    string getCountry();
    Card::Type getType();

private:
    string *country;
    Type *type;
};


class Deck {
public:
    Deck();
    Deck(string countries[], int size);
    explicit Deck(vector<Card> &cards);
    ~Deck();
    void shuffleDeck();
    Card draw();

private:
    vector<Card> *cards;
    void createCards(string &countries, int &size);
};

class Hand {
public:
    Hand();
    ~Hand();
    void insertCard(Card card);
    int exchange();
    int exchange(int cardIndices[]);

private:
    vector<Card> *cards;
    static bool sameCardCheck(const int handIndices[]);
    bool cardsValidForExchange(const int handIndices[]);
};



#endif //TEAM12_DOMINATIONGAME_CARDS_H