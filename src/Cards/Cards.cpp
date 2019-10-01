//
// Created by Noah on 2019-09-26.
//

#include "Cards.h"
#include <algorithm>
#include <iostream>

using namespace std;

// Card functions


Card::Card() {

}

Card::Card(string*, Card::Type*) {

}

// Deck Functions


// Card functions
Deck::Deck(string countries[], int *numCountries) {
    size = numCountries;
    cards = new Card[*numCountries];
    *currentCardIndex = 0;
    createCards(countries);
    shuffleDeck();
}

void Deck::createCards(string countries[]) {
    auto *currentType = new Card::Type;
    int starting = rand() % 3;
    for (int i = 0; i < *size; i++){
        *currentType = Card::Type((starting + i) % 3);
        cards[i] = Card(&countries[i], currentType);
    }
}

void Deck::shuffleDeck() {
    int *tempCardIndex = new int;
    for (int i = 0; i < *size; i++) {
        *tempCardIndex = random() % *size;
        swap(cards[i], cards[*tempCardIndex]);
    }
}

Card *Deck::draw() {
    if (*currentCardIndex == *size) {
        cout << "no more cards remaining";
        return nullptr;
    } else {
        return &cards[*currentCardIndex++];
    }
}

// Deck functions
// Hand functions


Hand::Hand() {

}


// Hand functions
