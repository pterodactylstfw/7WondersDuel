#pragma once

#include "GameState.h"
#include "Player.h"
#include "CardFactory.h"
#include "WonderFactory.h"

class GameController
{
private:
	std::unique_ptr<GameState> m_gameState;

	void prepareAge(int age);
	void checkEndAge();
	void draftWondersAuto();

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