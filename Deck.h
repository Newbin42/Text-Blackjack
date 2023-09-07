#pragma once

#include "Card.h"

#include <algorithm>
#include <random>

class Deck {
	private:
		std::vector<Card> deck = {};

		//Initialize the deck
		void init() {
			for (int sym = 0; sym < 4; sym++) {
				for (int val = 1; val < 14; val++) {
					Card newCard = Card(val, sym);
					this->deck.push_back(newCard);
				}
			}
		};

	public:
		Deck() {};

		Deck(bool preShuffle) {
			this->init();

			if (preShuffle == true) {
				this->shuffle();
			}
		};

		Deck(Deck& other) {
			this->deck = other.deck;
		};

		//Shuffle deck
		void shuffle() {
			std::random_device rd;
			std::mt19937 generator(rd());

			std::shuffle(this->deck.begin(), this->deck.end(), generator);
		};

		//Add a card to the deck
		void add(Card card) {
			this->deck.push_back(card);
		};

		//Get the size of the deck
		int size() {
			return this->deck.size();
		};

		//Convert a set of cards into one row
		std::string rowStr(int cardsPerRow) {
			std::string out = "";

			if (this->size() > 0) {
				int i = 0;
				int r = this->size() % cardsPerRow;

				for (int x = 0; x < int(this->size() / cardsPerRow); x++) {
					for (unsigned int y = 0; y < this->deck[0].rawArt().size(); y++) {
						for (int card = i; card < i + cardsPerRow; card++) {
							if (this->deck[card].isFaceUp()) {
								out.append(this->deck[card].rawArt()[y]);
							}
							else {
								out.append(this->deck[card].back[y]);
							}

							out.append(" ");
						}
					}

					out.append("\n");
					i += cardsPerRow;
				}

				for (unsigned int y = 0; y < this->deck[0].rawArt().size(); y++) {
					for (int card = i; card < i + r; card++) {
						if (this->deck[card].isFaceUp()) {
							out.append(this->deck[card].rawArt()[y]);
						}
						else {
							out.append(this->deck[card].back[y]);
						}

						out.append(" ");
					}

					out.append("\n");
				}
			}

			return out;
		};

		//Draw a card from the deck
		//Return NullCard if it cannot return anything
		Card draw() {
			int deckSize = this->deck.size();
			if (deckSize > 0) {
				Card drawn = this->deck[this->size() - 1];
				this->deck.pop_back();

				return drawn;
			}

			Card drawn = Card();
			drawn.null(true);
			return drawn;
		};

		friend std::ostream& operator<<(std::ostream& stream, Deck& deck) {
			for (Card card : deck.deck) {
				stream << card << "\n";
			}

			return stream;
		};
};