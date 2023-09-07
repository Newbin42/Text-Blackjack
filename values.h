#pragma once

#include <vector>
#include <string>

class values {
	private:
		std::vector<std::string> keys = {
			"A", "2", "3", "4", "5",
			"6", "7", "8", "9", "10",
			"J", "Q", "K", "#"
		};

		std::vector<int> value = {
			1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 100
		};

	public:
		values() {};

		values(const values& other) {
			this->keys = other.keys;
			this->value = other.value;
		};

		int get(std::string key) {
			int output = 0;
			bool stop = false;

			for (unsigned int x = 0; (x < this->keys.size() && stop == false); x++) {
				if (this->keys[x] == key) {
					output = this->value[x];
					stop = true;
				}
			};

			return output;
		};
};