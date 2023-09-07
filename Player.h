#pragma once

#include "Deck.h"

#include <string>
#include <random>

class Player {
	private:
		std::string name;

		int score = 0;
		bool revealed = false;

		Deck hand = Deck();

	public:
		Player();
		Player(std::string);
		Player(const Player&);

		void draw(Deck*, int);
		void hitMe(Deck*);
		virtual void update(Deck*) {};

		void setName(std::string);

		int getScore();

		bool hasRevealed();

		std::string getName();
		std::string reveal();

		friend std::ostream& operator<<(std::ostream&, Player&);
};

class AI : public Player {
	public:
		AI();
		AI(std::string);
		void quickUpdate();
		void update(Deck*);

		friend std::ostream& operator<<(std::ostream&, AI&);
};

class Players {
	private:
		std::vector<AI> ai_players = {};
		Player player;
		Deck* associatedDeck;

		bool weHaveAWinner();

	public:
		Players();
		Players(Deck*);
		~Players();

		void add(Player&);
		void add(AI&);
		void draw(int);
		void charDrawOne(std::string);
		void charReveal(std::string);
		void quickUpdate();
		void update();
		bool gameOver();

		Player getPlayerChar();
		AI getAI(std::string);

		Player getWinners();

		friend std::ostream& operator<<(std::ostream&, Players&);
};