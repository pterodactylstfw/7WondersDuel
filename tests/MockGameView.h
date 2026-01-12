#pragma once
#include "IGameView.h"
#include <iostream>

class MockGameView : public IGameView {
public:
	void onMessage(const std::string& message) override {}
	void onError(const std::string& error) override {}
	void onStateUpdated() override {}

	// returnam valori simple pentru a putea testa logica jocului
	int askInt(int min, int max, const std::string& prompt) override { return min; }

	ResourceType askResourceSelection(const std::vector<ResourceType>& options, const std::string& prompt) override {
		return options.empty() ? ResourceType::NONE : options[0];
	}

	int askWonderSelection(const std::array<std::unique_ptr<Wonder>, GameConstants::WONDERS_PER_PLAYER>& wonders, const std::string& playerName) override {
		return 0;
	}

	int askTokenSelection(const std::vector<std::unique_ptr<ProgressToken>>& tokens, const std::string& prompt) override {
		return 0;
	}

	int askCardSelectionFromList(const std::vector<std::reference_wrapper<const Card>>& cards, const std::string& prompt) override {
		return 0;
	}
};