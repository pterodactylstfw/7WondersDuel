#pragma once
#include "GameController.h"
class ConsoleUI: public IGameView
{
private:
	GameController m_game;

	void displayHeader() const;
	void displayGameState() const;

	void displayPlayer(const Player& player) const;

	void displayWonderBoard() const;

	void displayPyramid() const;

	void displayMilitaryTrack() const;

	void displayAccessibleCards() const;

	int showMainMenu();
	int showActionMenu();
	void showVictoryScreen(const std::string& winnerName);

	void handlePlayCard(); // logica pick a card to play
	void displayCityDetails(); // view my city details

	int showHighLevelMenu(); // meniu initial - play / view city / exit
	int showCardActionMenu(const Card& card); // meniu actiuni pentru o carte aleasa

	bool confirmPurchaseInteraction(int totalCost, int tradeCost);

	void onMessage(const std::string& message) override;
	void onError(const std::string& error) override;

	void onStateUpdated() override;

	int askInt(int min, int max, const std::string& prompt) override;
	ResourceType askResourceSelection(const std::vector<ResourceType>& options, const std::string& prompt) override;
	int askWonderSelection(const std::vector<std::unique_ptr<Wonder>>& wonders, const std::string& playerName) override;
	int askTokenSelection(const std::vector<std::unique_ptr<ProgressToken>>& tokens, const std::string& prompt) override;
	int askCardSelectionFromList(const std::vector<std::reference_wrapper<const Card>>& cards, const std::string& prompt) override;

public:
	ConsoleUI();
	void run();

};

