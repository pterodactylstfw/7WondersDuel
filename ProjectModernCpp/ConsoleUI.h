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

	//void displayPyramid() const;

	int showMainMenu();
	int showActionMenu();
	void showVictoryScreen(const std::string& winnerName);

public:

	void run();

};

