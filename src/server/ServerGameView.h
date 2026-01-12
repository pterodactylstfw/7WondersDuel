#pragma once
#include <iostream>

#include "IGameView.h"


class ServerGameView: public IGameView {
public:
    void onMessage(const std::string &message) override {
        std::println("[GAME INFO] {}",  message);
    }

    void onError(const std::string &errorMessage) override {
        std::cerr<< "[GAME ERROR] " << errorMessage << std::endl;
    }

    void onStateUpdated() override {
        // TODO
    }

    int askInt(int, int, const std::string&) override {
        return 0;
    }

    ResourceType askResourceSelection(const std::vector<ResourceType> &options, const std::string &prompt) override {
        return options.empty() ? ResourceType::NONE : options[0];
    }

    int askWonderSelection(const std::array<std::unique_ptr<Wonder>, GameConstants::WONDERS_PER_PLAYER>&, const std::string&) override {
        return 0;
    }

    int askTokenSelection(const std::vector<std::unique_ptr<ProgressToken>>&, const std::string&) override {
        return 0;
    }

    int askCardSelectionFromList(const std::vector<std::reference_wrapper<const Card>>&, const std::string&) override {
        return 0;
    }
};
