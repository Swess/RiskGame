//
// Created by NohaHorowitz on 10/4/2019.
//

#include <vector>
#include <cassert>
#include <iostream>
#include "Cards.h"

namespace Cards {
    namespace Driver {

        bool testDeck() {

            const int NUMBER_OF_CARDS = 43;
            string countries[NUMBER_OF_CARDS];

            for (int i = 0; i < NUMBER_OF_CARDS; i++) {
                countries[i] = "Stub name" + to_string(i);
            }

            Deck deck(countries, NUMBER_OF_CARDS);
            deck.shuffleDeck();

            // confirm equal amount of types in deck
            int typeCounter[] = {0, 0, 0};
            for (int i = 0; i < NUMBER_OF_CARDS; i++) {
                Card card(deck.draw());

                typeCounter[card.getType()]++;
            }

            // assert
            assert(abs(typeCounter[0] - typeCounter[1]) < 2 && abs(typeCounter[0] - typeCounter[2]) < 2 && abs(typeCounter[1] - typeCounter[2]) < 2 );

            return true;    // Func needed to return
        }


        bool testHand () {

            int expectedArmiesReturned[] = {4, 6, 8, 10, 12, 15, 20, 25, 30};
            Hand hand;
            const int DECK_SIZE = 42;
            auto cards = vector<Card *>(DECK_SIZE);
            // fill hand with 42 cards
            for (int i = 0; i<DECK_SIZE; i++) {
                 cards.at(i) = new Card("Stub name " + to_string(i), Card::Type(i%3));
            }

            Deck deck(cards);

            // pre-select hand indices to exchange
            int exchangeIndices[] = {2, 1, 0};
            int i = 0;
            while (i < 9) {
                // add three cards to hand
                hand.insertCard(deck.draw());
                hand.insertCard(deck.draw());
                hand.insertCard(deck.draw());

                // assert exchange of pre-selected indices resulted in the expected number of armies returned.
                assert(hand.exchange(exchangeIndices) == expectedArmiesReturned[i]);
                i++;
            }

            return true;
        }

        bool run() {
            cout << "[TEST] -----" << endl;
            cout << "[TEST] - Running Cards component tests." << endl;

            testDeck();
            testHand();

            cout << "[TEST] - End of Cards component testing." << endl;
            cout << "[TEST] -----" << endl;
            return true;
        }

    }


}
