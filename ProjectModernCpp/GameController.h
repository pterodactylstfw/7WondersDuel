#pragma once

#include "GameState.h"
#include "Player.h"

class GameController
{
private:
	std::unique_ptr<GameState> m_gameState;

public:
	GameController() = default;

	void startNewGame(const std::string& player1, const std::string& player2);

	void loadGame(const std::string& filename);
	void saveGame(const std::string& filename) const;

	void applyWonderEffect(Player& player, Player& opponent, const Wonder& wonder, GameState& gameState);

	~GameController() = default;
};