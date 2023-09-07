#include "Player.h"

//Player
//--Public
Player::Player() {
	this->name = "Player";
};

Player::Player(std::string name) {
	this->name = name;
};

Player::Player(const Player& other) {
	this->name = other.name;
	this->score = other.score;
	this->hand = other.hand;
	this->revealed = other.revealed;
};

void Player::draw(Deck* deck, int amount) {
	for (int x = 0; x < amount; x++) {
		this->hitMe(deck);
	}
};

void Player::hitMe(Deck* deck) {
	Card card = deck->draw();
	card.flip();
	this->score += card.getValue();
	this->hand.add(card);
};

void Player::setName(std::string name) {
	this->name = name;
};

int Player::getScore() {
	return this->score;
};

bool Player::hasRevealed() {
	return this->revealed;
};

std::string Player::getName() {
	return this->name;
};

std::string Player::reveal() {
	this->revealed = true;
	return this->hand.rowStr(8);
};

std::ostream& operator<<(std::ostream& out, Player& player) {
	out << std::format("\nYour Hand | Score: {}\n", player.score) << player.hand.rowStr(8);
	return out;
};
//End Player

//AI
//--Public
AI::AI() {};
AI::AI(std::string name) {
	this->setName(name);
};

void AI::quickUpdate() {
	if (this->getScore() >= 21) {
		this->reveal();

	} else {
		std::default_random_engine generator;

		double confidence = double(this->getScore()) / double(21);

		std::uniform_real_distribution<double> dist(confidence / 1.075, 1.1);
		double choice = dist(generator);

		if (choice < confidence) {
			this->reveal();
		}
	}
};

void AI::update(Deck* deck) {
	if (this->getScore() >= 21) {
		this->reveal();
	} else {
		std::default_random_engine generator;

		double confidence = double(this->getScore()) / double(21);

		std::uniform_real_distribution<double> dist(confidence / 1.075, 1.1);
		double choice = dist(generator);

		if (choice < confidence) {
			this->reveal();
		} else {
			this->hitMe(deck);
		}

		if (this->hasRevealed() == false && this->getScore() >= 21) {
			this->reveal();
		}
	}
};

std::ostream& operator<<(std::ostream& stream, AI& ai) {
	stream << std::format("AI - {0:15} | revealed: {1:5}| Score: ", ai.getName(), ai.hasRevealed());
	
	if (ai.hasRevealed() == true) {
		stream << ai.getScore();
	} else {
		stream << "?";
	}

	return stream;
}
//End AI

//Players
//--Private
bool Players::weHaveAWinner() {
	bool output = false;

	if (this->player.getScore() == 21) {
		output = true;
	} else {
		for (AI player : this->ai_players) {
			if (player.getScore() == 21) {
				output = true;
			}
		}
	}

	return output;
};

//--Public
Players::Players() {
	this->associatedDeck = new Deck();
};

Players::Players(Deck* deck) {
	this->associatedDeck = deck;
};

Players::~Players() {
	this->associatedDeck = NULL;
};

void Players::add(Player& player) {
	if (player.getName() == "Player") {
		this->player = player;
	}
}

void Players::add(AI& player) {
	this->ai_players.push_back(player);
}

void Players::draw(int amount = 2) {
	for (int x = 0; x < 2; x++) {
		this->player.hitMe(this->associatedDeck);
	}

	for (unsigned int x = 0; x < this->ai_players.size(); x++) {
		for (int y = 0; y < amount; y++) {
			this->ai_players[x].hitMe(this->associatedDeck);
		}
	}
};

void Players::charDrawOne(std::string name) {
	if (name == "Player") {
		this->player.hitMe(this->associatedDeck);
	} else {
		for (unsigned int x = 0; x < this->ai_players.size(); x++) {
			if (this->ai_players[x].getName() == name) {
				this->ai_players[x].hitMe(this->associatedDeck);
			}
		}
	}
};

void Players::charReveal(std::string name) {
	if (name == "Player") {
		this->player.reveal();
	} else {
		for (unsigned int x = 0; x < this->ai_players.size(); x++) {
			if (this->ai_players[x].getName() == name) {
				this->ai_players[x].reveal();
			}
		}
	}
};

void Players::quickUpdate() {
	for (unsigned int x = 0; x < this->ai_players.size(); x++) {
		if (this->ai_players[x].hasRevealed() == false) {
			this->ai_players[x].quickUpdate();
		}
	}
}

void Players::update() {
	for (unsigned int x = 0; x < this->ai_players.size(); x++) {
		if (this->ai_players[x].hasRevealed() == false) {
			this->ai_players[x].update(this->associatedDeck);
		}
	}
}

bool Players::gameOver() {
	bool output = false;
	int count = 0;

	if (this->player.hasRevealed() == true) {
		count++;
	}

	for (AI player : this->ai_players) {
		if (player.hasRevealed() == true) {
			count++;
		}
	}

	if (count == this->ai_players.size() + 1 || this->weHaveAWinner()) {
		output = true;
	}

	return output;
};

Player Players::getPlayerChar() {
	return this->player;
};

AI Players::getAI(std::string name) {
	AI output;
	for (AI player : this->ai_players) {
		if (player.getName() == name) {
			output = player;
		}
	}

	return output;
};

Player Players::getWinners() {
	std::vector<Player> candidates = {};

	if (this->player.getScore() <= 21) {
		candidates.push_back(this->player);
	}

	for (AI player : this->ai_players) {
		if (player.getScore() <= 21) {
			candidates.push_back(player);
		}
	}

	Player winner;
	switch (candidates.size()) {
		case (0):
			throw std::overflow_error("");
			break;

		case(1):
			winner = candidates[0];
			break;

		default:
			winner = candidates[0];
			for (Player player : candidates) {
				if (player.getScore() > winner.getScore()) {
					winner = player;
				}
			}
	}

	return winner;
};

std::ostream& operator<<(std::ostream& stream, Players& players) {
	for (AI player : players.ai_players) {
		stream << player << "\n";
	}

	stream << players.player << "\n";

	return stream;
};
//End Players