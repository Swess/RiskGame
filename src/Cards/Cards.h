//
// Created by Noah on 2019-09-26.
//

#pragma once
#include <string>
#include <vector>

using namespace std;

namespace Cards {

class Card {
public:
    enum Type {INFANTRY = 0, ARTILLERY = 1, CAVALRY = 2};
    Card();
    Card(Card &card);
    Card(string country, Type type);
    ~Card();
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
    explicit Deck(vector<Card *> &cards);
    ~Deck();
    void shuffleDeck();
    Card draw();
    vector<Card *> *cards;
};

class Hand {
public:
    Hand();
    ~Hand();
    void insertCard(Card card);
    int exchange(int cardIndices[]);
    bool cardsValidForExchange(const int handIndices[]);
    int size();
private:
    vector<Card *> *cards;
    static bool sameCardCheck(const int handIndices[]);
};

}