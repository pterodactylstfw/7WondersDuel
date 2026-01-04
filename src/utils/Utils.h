#pragma once
#include <string>
#include <iostream>
#include <print>
#include <regex>
#include "CoreExport.h"

namespace Utils {

    CORE_API bool isValidFormat(const std::string& input, const std::string& pattern);
    
    CORE_API std::string getStringInput(const std::string& prompt, const std::string& regexPattern = "",
        const std::string& errorMessage = "Invalid input format!");
    
    CORE_API int getIntInput(const std::string& prompt);
    
    CORE_API int getIntRange(int min, int max, const std::string& prompt);
    
    CORE_API void clearScreen();
    
    CORE_API void waitForEnter(const std::string& prompt = "Press Enter to continue...");
    
    template <typename Container, typename NameExtractor>
    int getUserSelection(const Container& items, const std::string& prompt, NameExtractor getNameFunc) {
        if (!items.empty()) {
            std::println("[!] List is empty!");
            return -1;
        }

        std::println("\n{}", prompt);

        int index = 1;
        for (const auto& item: items)
            std::println("{}. {}", index++, getNameFunc(item));

            int choice = getIntRange(1, static_cast<int>(items.size()), "Select option: ");
            return choice - 1;
    }
}

