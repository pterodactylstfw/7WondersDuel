#pragma once

#include <algorithm>
#include "GameState.h"
#include "Player.h"
#include "CardFactory.h"
#include "IGameView.h"
#include "WonderFactory.h"
#include "Utils.h"
#include "ProgressTokenFactory.h"


class GameController
{
private:
	std::unique_ptr<GameState> m_gameState;
	std::reference_wrapper<IGameView> m_view;

	void prepareAge(int age);
	void checkEndAge();

	void draftWondersAuto();
	void prepareDraftRound();

	void prepareProgressTokens();

	void checkMilitaryLooting(int previousShields, int currentShields);
	void checkInstantVictory();

	void applyEffect(Player& player, const CardEffect& effect);
	void grantCardToPlayer(Player& player, std::unique_ptr<Card> card);

	bool handleConstructBuilding(int cardIndex);
	bool handleDiscardCard(int cardIndex);
	bool handleConstructWonders(int cardIndex, int wonderIndex, bool & outPlayAgain);

public:
	explicit GameController(IGameView& view);

	void startNewGame(const std::string& player1, const std::string& player2);
	
	bool executeAction(int cardIndex, PlayerAction action, int wonderIndex = -1);

	const GameState& getGameState() const;
	bool isGameOver() const; // acestea 2 pentru Ui - readonly

	void loadGame(const std::string& filename);
	void saveGame(const std::string& filename) const;
  
	void applyProgressTokenEffect(Player& player, Player& opponent, ProgressToken& token);
	bool pickWonder(int wonderIndex);

	~GameController() = default;
};