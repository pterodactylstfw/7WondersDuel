#pragma once
#include <string>
#include <iostream>
#include <regex>

namespace Utils {

    inline bool isValidFormat(const std::string& input, const std::string& pattern) {
        try {
            std::regex rgx(pattern);
            return std::regex_match(input, rgx);
        } catch (const std::regex_error& e) {
            std::cerr << "Regex error: " << e.what() << std::endl;
            return false;
        }
    }

	inline std::string getStringInput(const std::string& prompt, const std::string& regexPattern = "",
	    const std::string& errorMessage = "Invalid input format!")
    {
        while (true) {
            std::cout << prompt;
            std::string s;
            std::getline(std::cin, s);

            s.erase(0, s.find_first_not_of(" \t\n\r\f\v"));
            s.erase(s.find_last_not_of(" \t\n\r\f\v") + 1);

            if (s.empty()) {
                std::cout<<"[!] Input cannot be empty. Please try again.\n";
                continue;
            }

            if (regexPattern.empty())
                return s;

            if (isValidFormat(s, regexPattern))
                return s;

            std::cout<<"[!] " << errorMessage << "\n";
        }
	}

	inline int getIntInput(const std::string& prompt) {

        std::regex intPattern("^-?\\d+$"); // optional -, urmat de una sau mai multe cifre

        while (true) {
            std::cout << prompt;
            std::string input;
            std::getline(std::cin, input);

            if (std::regex_match(input, intPattern)) {
                try {
                    return std::stoi(input); // conversie sigura
                }
                catch (...) {
                    std::cout << "[!] Number is too large or invalid.\n";
                }
            }
            else {
                std::cout << "[!] Please enter a valid number (digits only).\n";
            }
        }
    }

    inline int getIntRange(int min, int max, const std::string& prompt) {
        while (true) {
            int val = getIntInput(prompt);
            if (val >= min && val <= max) return val;
            std::cout << "[!] Number must be between " << min << " and " << max << ".\n";
        }
    }

    inline void clearScreen() {
	    std::cout << "\033[2J\033[1;1H";// cod ansi care curata ecranul, un fel de system cls/clear
	    //sterg daca nu este ok
	}

    inline void waitForEnter(const std::string& prompt = "Press Enter to continue...") {
        std::cout << prompt;
        std::string s;
        std::getline(std::cin, s);
    }
}

