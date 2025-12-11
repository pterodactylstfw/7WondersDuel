#pragma once
#include "GameController.h"
class ConsoleUI
{
private:
	GameController m_game;

	void displayHeader() const;
	void displayGameState() const;

	void displayPlayer(const Player& player) const;

	void displayWonderBoard() const;

	void displayPyramid() const;

	void displayMilitaryTrack() const;

	void displayAccesibleCards() const;

	int showMainMenu();
	int showActionMenu();
	void showVictoryScreen(const std::string& winnerName);

	void handlePlayCard(); // logica pick a card to play
	void displayCityDetails(); // view my city details

	int showHighLevelMenu(); // meniu initial - play / view city / exit
	int showCardActionMenu(const Card& card); // meniu actiuni pentru o carte aleasa

public:

	void run();

};

