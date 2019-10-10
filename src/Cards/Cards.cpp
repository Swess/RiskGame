//
// Created by Noah on 2019-09-26.
//

#include "Cards.h"
#include <algorithm>
#include <iostream>
#include <ctime>

using namespace std;

namespace Cards {

Card::Card() {
    this->country = new string;
    this->type = new Card::Type;
}

Card::Card(string country, Card::Type type) {
    this->country = new string(country);
    this->type = new Card::Type(type);
}

Card::~Card() {
    delete country;
    delete type;

    country = nullptr;
    type = nullptr;
}

string Card::getCountry() {
    return *country;
}

Card::Type Card::getType() {
    return *type;
}

Card::Card(Card &card) {
    this->country = new string(card.getCountry());
    this->type = new Card::Type(card.getType());
}

Deck::Deck() {
    cards = new vector<Card *>;
}

Deck::Deck(vector<Card *> &cards) {
    this->cards = new vector<Card *>(cards.size());
    auto deckCardsIterator = this->cards->begin();
    auto passedCardsIterator = cards.begin();
    for (size_t i = 0; i < cards.size(); i++) {
        *deckCardsIterator = new Card(**passedCardsIterator);
        deckCardsIterator++;
        passedCardsIterator++;
    }
}

Deck::~Deck() {
    delete cards;

    cards = nullptr;
}


Card Deck::draw() {
    Card card(*(cards->back()));
    cards->pop_back();
    return card;
}

void Deck::shuffleDeck() {
    unsigned long random = 0;
    const auto iter = cards->begin();
    srand (time(NULL));
    for (size_t i = 0; i < cards->size(); i++) {
        random = rand() % cards->size();
        iter_swap(iter + i, iter+random);
    }
}

Deck::Deck(string *countries, int size) {
    cards = new vector<Card *>;
    int starting = rand() % 3;
    for (int i = 0; i < size; i++) {
        Card card(countries[i], Card::Type((starting + i) % 3));
        cards->push_back(new Card(card));
    }
}

Hand::Hand() {
    cards = new vector<Card *>;
}

Hand::~Hand() {
    delete cards;

    cards = nullptr;
}



void Hand::insertCard(Card card) {
    cards->push_back(new Card(card));
}

int Hand::exchange(int cardIndices[]) {
    static int *totalSetsTraded = new int(0);
    sort(cardIndices, cardIndices+3);
    cards->erase(cards->begin() + cardIndices[2]);
    cards->erase(cards->begin() + cardIndices[1]);
    cards->erase(cards->begin() + cardIndices[0]);

    int numArmiesToReturn;
    if (*totalSetsTraded < 5) {
        numArmiesToReturn = 4 + *totalSetsTraded * 2;
    } else {
        numArmiesToReturn = 15 + 5 * (*totalSetsTraded - 5);
    }
    (*totalSetsTraded)++;
    return numArmiesToReturn;
}

bool Hand::sameCardCheck(const int *handIndices) {
    return (handIndices[0] == handIndices[1] || handIndices[0] == handIndices[2] || handIndices[1] == handIndices[2]);
}

bool Hand::cardsValidForExchange(const int *handIndices){
    if (sameCardCheck(handIndices)) {
        return false;
    }
    bool typeFlags[] = {false, false, false};
    for (int i = 0; i < 3; i++) {
        typeFlags[this->cards->at(handIndices[i])->getType()] = true;
    }
    int count = 0;
    for (bool typeFlag : typeFlags) {
        if (typeFlag) {
            count++;
        }
    }
    return (count == 1 || count == 3);
}
}