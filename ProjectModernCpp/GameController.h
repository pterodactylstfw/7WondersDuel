#pragma once

#include "GameState.h"
#include "Player.h"
#include "CardFactory.h"
#include "WonderFactory.h"
#include "ProgressTokenFactory.h"

class GameController
{
private:
	std::unique_ptr<GameState> m_gameState;

	void prepareAge(int age);
	void checkEndAge();
	void draftWondersAuto();
	void prepareProgressTokens();

	bool handleConstructBuilding(int cardIndex);
	bool handleDiscardCard(int cardIndex);
	bool handleConstructWonders(int cardIndex, int wonderIndex, bool & outPlayAgain);

public:
	GameController() = default;

	void startNewGame(const std::string& player1, const std::string& player2);
	
	bool executeAction(int cardIndex, PlayerAction action, int wonderIndex = -1);

	const GameState& getGameState() const;
	bool isGameOver() const; // acestea 2 pentru Ui - readonly

	void loadGame(const std::string& filename);
	void saveGame(const std::string& filename) const;

	void applyWonderEffect(Player& player, Player& opponent, const Wonder& wonder);

	~GameController() = default;
};