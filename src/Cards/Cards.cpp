//
// Created by Noah on 2019-09-26.
//

#include "Cards.h"
#include <algorithm>
#include <iostream>
#include <utility>

using namespace std;

namespace Cards {
int *totalSetsTraded = new int(0);

// Since we have pointers, you HAVE to init some things here...
// Otherwise getCountry with give an error for example..
// But this is not ideal... as it makes no sense to let anybody create a card without any param.
// because you don't have any setters later...
// The default constructor is used when calling : Card myCardName; ..
// but this makes the country & type ptr as nullptr, make sure that its what you want

Card::Card(string country, Card::Type type) {
    this->country = new string(std::move(country));
    this->type = new Card::Type(type);
}

Card::~Card() {
    // This destructor is doing the right thing !
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

Deck::Deck() {
    cards = new vector<Card>;
}

Deck::Deck(vector<Card> &cards) {
    this->cards = new vector<Card>(cards);
    shuffleDeck();
}

Deck::~Deck() {
    delete cards;

    cards = nullptr;
}


Card Deck::draw() {
    Card card = Card(cards->back().getCountry(), cards->back().getType());
    cards->pop_back();
    return card;
}

void Deck::shuffleDeck() {
    int random = 0;
    (*cards).begin();
    for (int i = 0; i < cards->size(); i++) {
        random = rand() % cards->size();
//        iter_swap(cards->begin() + i, cards->begin() + random);
    }
}

Deck::Deck(string *countries, int size) {
    cards = new vector<Card>;
    Card::Type type;
    int starting = rand() % 3;
    for (int i = 0; i < size; i++) {
        // Replaced with push_back...

        // This line is problematic because you create a Card on the stack
        // but it will get deleted the moment it leave the scope.. meaning, the moment the line
        // finishes, the card is actually destroyed...
        // When it gets destroyed, it destroys the values it points to (in the destructor)
        // During that call it creates a copy, but only of the ptr (country / type), but the other one destroys them
        // This leads to an vector of Cards that points to undefined mem values...
        // The reason why it crashes later...

        cards->push_back(Card(countries[i], Card::Type(starting + (i %3))));

    }
}

Hand::Hand() {
    cards = new vector<Card>;
}

Hand::~Hand() {
    delete cards;

    cards = nullptr;
}



void Hand::insertCard(Card card) {
    Card copyCard(card);
    cards->insert(cards->end(), Card(card));
}

int Hand::exchange() {
    int exchangeIndices[3];
    Card *currentCard;

    cout << "these are your current cards";
    for (int i = 0; i < cards->size(); i++) {
        currentCard = &cards->at(i);
        cout << (i+1) << ": (country: " << currentCard->getCountry() << ") (type: " << currentCard->getType() << ")";
    }
    string exchangeString;
    do {
        cout << "select 3 cards you would like to exchange";
        cout << "First card: ";
        cin >> exchangeIndices[0];
        cout << "Second card: ";
        cin >> exchangeIndices[0];
        cout << "Third card: ";
        cin >> exchangeIndices[0];
    } while (cardsValidForExchange(exchangeIndices));

    int numArmiesToReturn;
    if (*totalSetsTraded < 7) {
        numArmiesToReturn = 2 + *totalSetsTraded * 3;
    } else {
        numArmiesToReturn = 15 + 5 * (*totalSetsTraded - 6);
    }
    (*totalSetsTraded)++;
    return numArmiesToReturn;
}

int Hand::exchange(int cardIndices[]) {
    cards->erase(cards->begin() + cardIndices[0]);
    cards->erase(cards->begin() + cardIndices[1]);
    cards->erase(cards->begin() + cardIndices[2]);
    int numArmiesToReturn;
    if (*totalSetsTraded < 7) {
        numArmiesToReturn = 2 + *totalSetsTraded * 3;
    } else {
        numArmiesToReturn = 20 + 5 * (*totalSetsTraded - 6);
    }
    cout << to_string(numArmiesToReturn) << "\n";
    (*totalSetsTraded)++;
    return numArmiesToReturn;
}

bool Hand::sameCardCheck(const int *handIndices) {
    return (handIndices[0] == handIndices[1] || handIndices[0] == handIndices[2] || handIndices[1] == handIndices[2]);
}

bool Hand::cardsValidForExchange(const int *handIndices){
    bool typeFlags[3];
    for (int i = 0; i < 3; i++) {
        typeFlags[cards->at(handIndices[i]).getType()] = true;
    }
    int count = 0;
    for (bool typeFlag : typeFlags) {
        if (typeFlag) {
            count++;
        }
    }
    return (!sameCardCheck(handIndices) & (count == 1 || count == 3));
}
}   // namespace
