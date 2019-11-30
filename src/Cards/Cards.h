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
    string * get_country();
    Card::Type get_type();
    string to_string();

private:
    string *country;
    Type *type;
};


class Deck {
public:
    Deck();
    explicit Deck(const vector<string>& countries);
    explicit Deck(vector<Card *> &cards);
    ~Deck();
    void shuffleDeck();
    Card draw();
    int get_size();
    static int totalSetsTraded;
    vector<Card *> *cards;
};

class Hand {
public:
    Hand();
    ~Hand();
    void clear_hand();
    void insertCard(Card card);
    int exchange(int cardIndices[]);
    bool cardsValidForExchange(const int handIndices[]);
    int size();
    vector<Card *> * get_cards();
private:
    vector<Card *> * cards;
    static bool sameCardCheck(const int handIndices[]);
};

}