//
// Created by Noah on 2019-09-26.
//

#pragma once
#include <string>
#include <vector>

using namespace std;

namespace Cards {
extern int *totalSetsTraded;

class Card {
public:
    enum Type {INFANTRY = 0, ARTILLERY = 1, CAVALRY = 2};
//    Card operator =(const Card &otherCard) {
//        return Card(otherCard);
//    }
    Card();
    Card(const Card &card);
    Card(string country, Type type);
    ~Card();
    string getCountry() const;
    Card::Type getType() const;

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
private:
    void createCards(string &countries, int &size);
};

class Hand {
public:
    Hand();
    ~Hand();
    void insertCard(Card card);
    int exchange(int cardIndices[]);

private:
    vector<Card *> *cards;
    static bool sameCardCheck(const int handIndices[]);
    bool cardsValidForExchange(const int handIndices[]);
};

}