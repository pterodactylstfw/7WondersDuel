#pragma once
#include <string>
#include <iostream>

namespace Utils {
	inline std::string getStringInput(const std::string& prompt) {
		std::cout << prompt;
		std::string s;
		std::getline(std::cin, s);
		return s;
	}

	inline int getIntInput(const std::string& prompt) {
        while (true) {
            std::cout << prompt;
            std::string input;
            std::getline(std::cin, input);
            try {
                size_t pos;
                int val = std::stoi(input, &pos);
                return val;
            }
            catch (...) {
                std::cout << "[!] Te rog introdu un numar valid (cifre)!\n";
            }
        }
    }

    inline int getIntRange(int min, int max, const std::string& prompt) {
        while (true) {
            int val = getIntInput(prompt);
            if (val >= min && val <= max) return val;
            std::cout << "[!] Numarul trebuie sa fie intre " << min << " si " << max << ".\n";
        }
    }
}

