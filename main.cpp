#include <cstdlib>
#include <ctime>
#include <format>

#if defined _WIN32
#include <windows.h>
#include <conio.h>
#elif defined (__LINUX__) || defined (__gnu_linux__) || defined (__linux__) || defined (__APPLE__)
#include <unistd.h>
#endif

#include "Player.h"

using namespace std;

bool game(vector<string>);
string help();
void waitForInput();
void clear();

int main() {
	//Names
	vector<string> names = {
		"Quagmire", "Brian", "Louis Griffon",
		"Peter Griffon", "Stan Smith", "Francine Smith",
		"Jordan Shanks", "Mark Fishbauch", "Roger",
		"Steven", "Pearl", "Hank Hill",
		"Peggy Hill", "Dale Gribble", "John Redcorne",
		"Redneck Joe", "Boomhauer"
	};

	//Seed
	srand(int(time(NULL)));

	//Main Play
	bool playing = true;
	while (playing == true) {
		clear();
		playing = game(names);
	}

	return 0;
}

bool game(vector<string> names) {
	bool playAgain = true;
	bool quit = false;
	Deck* deck = new Deck(true);
	Players players = Players(deck);

	for (int x = 0; x < rand() % 4 + 2; x++) {
		int randNum = rand() % (names.size() - 1);

		AI temp = AI(names[randNum]);
		names.erase(names.begin() + randNum);

		players.add(temp);
	}

	Player player = Player();
	players.add(player);
	players.draw(2);
	players.quickUpdate();

	while (players.gameOver() == false && quit == false) {
		clear();

		//Update Main Player
		if (players.getPlayerChar().hasRevealed() == false) {
			if (players.getPlayerChar().getScore() > 21) {
				players.charReveal("Player");

				cout << "Deck remaining: " << format("{0:2}\n{1}\n\n", deck->size(), Card::join(Card::backArt(), "\n")) << players << endl;
			} else {
				bool validInput;
				string input = "";
				string placeholder = "";

				do {
					validInput = true;

					clear();
					cout << "Deck remaining: " << format("{0:2}\n{1}\n\n", deck->size(), Card::join(Card::backArt(), "\n")) << players << endl;
					cout << "What would you like to do (hit/reveal/help/quit)?\n>>";

					cin >> input;
					if (input == "hit") {
						players.charDrawOne("Player");

					} else if (input == "reveal") {
						players.charReveal("Player");

					} else if (input == "help") {
						clear();
						cout << help() << endl;
						waitForInput();

					} else if (input == "quit") {
						quit = true;
					} else {
						cout << "Unrecognized command. Please try again." << endl;
						waitForInput();
						validInput = false;

					}
				} while (validInput == false && quit == false);
			}
		}

		if (quit == false) {
			//Update Ais
			players.update();
			cout << "AI Updating" << endl;
		}
	}

	if (quit == false) {
		clear();
		cout << "Deck remaining: " << format("{0:2}\n{1}\n\n", deck->size(), Card::join(Card::backArt(), "\n")) << players << endl;

		try {
			Player winner = players.getWinners();

			if (winner.getName() == "Player") {
				cout << "You won with a score of " << winner.getScore() << "!" << endl;
			} else {
				cout << "You lost to " << winner.getName() << " with a score of " << winner.getScore() << "!" << endl;
			}

		} catch (std::overflow_error) {
			cout << "Nobody won." << endl;
		}

		if (deck != NULL) {
			deck = NULL;
		}

		string yn = "";
		cout << "Would you like to play again?\n>>";
		cin >> yn;

		if (yn == "n") {
			playAgain = false;
		}
	} else {
		playAgain = false;
	}

	return playAgain;
};

string help() {
	string out = "";

	out.append("----Help----\n");
	out.append("Card Types:\n - S: Spades\n - H: Hearts\n - D: Diamonds\n - C: Clubs\n\n");
	out.append("Numbers:\n - A: 1\n - 2 - 10\n - J: Jack\n - Q: Queen\n - K: King\n\n");
	out.append("Rules:\n");
	out.append(" - Each player takes a turn too either draw a card (hit), or reveal their hand.");
	out.append(" The first player to reach 21 or whomever has the largest hand under 21 wins.");
	out.append(" If your hand's value rises above 21 you immediately lose.");
	out.append("Card values:\n - A = 1\n - 2-10 = 2-10\n - J = 11\n - Q = 12\n - K = 13\n");

	return out;
};

void waitForInput() {
	string placeholder;
	cout << "Press enter to continue...\n>>";
	cin.ignore();
	getline(cin, placeholder);
};

void clear() {
#if defined _WIN32
	system("cls");
	Sleep(5);

#elif defined (__LINUX__) || defined (__gnu_linux__) || defined (__linux__) || defined (__APPLE__)
	system("clear");
	usleep(5000);

#endif
};