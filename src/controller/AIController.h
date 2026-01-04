#pragma once
#include "GameState.h"
#include "Constants.h"
#include "CoreExport.h"
#include <vector>
#include <optional>
enum class AIDifficulty {
	EASY,
	HARD
};
struct AIMove {
	int cardIndex;
	PlayerAction action;
	int wonderIndex = -1;
};

class CORE_API AIController {
private:
	AIDifficulty m_difficulty;
	int pickWonder(const GameState& state);
	AIMove getRandomMove(const GameState& state);
	AIMove getGreedyMove(const GameState& state);
	double evaluateCardValue(const Card& card, const Player& player, const Player& opponent, const GameState& state);
public:
	AIController(AIDifficulty difficulty);
	AIMove decideMove(const GameState& state);

};