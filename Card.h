#pragma once

#include "values.h"

#include <format>
#include <iostream>

class Card {
	private:
		int value;

		bool faceUp = false;
		bool invalid = false;

		std::string symbol;
		std::string num;
		std::string ascii;
		std::vector<std::string> asciiArt;

		values cardValues = values();

		std::vector<std::string> draw() {
			std::vector<std::string> art = {
				",,,,,,,",
				std::format("|{0:2}  {1}|", this->num, this->ascii),
				std::format("|/{1}{0:>2}\\|", this->num, this->ascii),
				std::format("|\\{0:2}{1}/|", this->num, this->ascii),
				std::format("|{1}  {0:>2}|", this->num, this->ascii),
				"```````"
			};

			return art;
		};

	public:
		Card() {
			this->num = Card::asciiValues[0];
			this->symbol = Card::symbols[0];
			this->value = this->cardValues.get(this->num);
			this->ascii = Card::asciiSyms[0];
			this->asciiArt = this->draw();
		};

		Card(int value, int symbol) {
			if (value > 0 && value < 15) {
				this->num = Card::asciiValues[value - 1];
			}
			else {
				this->num = Card::asciiValues[0];
			}

			if (symbol > -1 && symbol < 4) {
				this->symbol = Card::symbols[symbol];
				this->ascii = Card::asciiSyms[symbol];
			}
			else {
				this->symbol = Card::symbols[0];
				this->ascii = Card::asciiSyms[0];
			}

			this->value = this->cardValues.get(this->num);
			this->asciiArt = this->draw();
		};
		
		Card(const Card& other) {
			this->num = other.num;
			this->symbol = other.symbol;
			this->value = other.value;
			this->ascii = other.ascii;
			this->asciiArt = other.asciiArt;
			this->faceUp = other.faceUp;
		};

		void flip() {
			if (this->isFaceUp()) {
				this->faceUp = false;
			}
			else {
				this->faceUp = true;
			}
		};

		void null(bool val) {
			this->invalid = val;
		};

		void copyAssets(Card& other) {
			this->num = other.num;
			this->symbol = other.symbol;
			this->value = other.value;
			this->ascii = other.ascii;
			this->asciiArt = other.asciiArt;
			this->faceUp = other.faceUp;
		};

		int getValue() {
			return this->value;
		};

		bool isFaceUp() {
			return this->faceUp;
		};

		bool isNull() {
			return this->invalid;
		};

		//Join card rows
		static std::string join(std::vector<std::string> vector, std::string seperator) {
			std::string out = "";
			std::string temp;

			for (unsigned int x = 0; x < vector.size(); x++) {
				temp = "";
				temp.append(vector[x]);
				temp.append(seperator);

				out.append(temp);
			}

			return out;
		};

		//Return the formatted card art
		std::string art() {
			std::string out = "";

			if (this->isFaceUp()) {
				out = Card::join(this->asciiArt, "\n");
			}
			else {
				out = Card::join(this->back, "\n");
			}

			return out;
		};

		//Return the raw card art
		std::vector<std::string> rawArt() {
			return this->asciiArt;
		};

		//Get the raw back art
		static std::vector<std::string> backArt() {
			return Card().back;
		};

		//Friends
		friend std::ostream& operator<<(std::ostream& out, Card& card) {
			out << card.art();
			return out;
		};

		//Data
		std::vector<std::string> symbols = { "spades", "hearts", "clubs", "diamonds" };
		std::vector<char> asciiSyms = {'S', 'H', 'C', 'D'};
		std::vector<std::string> asciiValues = {
			"A", "2", "3", "4", "5",
			"6", "7", "8", "9", "10",
			"J", "Q", "K", "#"
		};
		std::vector<std::string> back = {
			",,,,,,,",
			"|×/C\\×|",
			"|/ A \\|",
			"|\\ R /|",
			"|×\\D/×|",
			"```````"
		};
};