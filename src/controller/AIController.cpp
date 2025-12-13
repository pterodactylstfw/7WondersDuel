#include "AIController.h"
#include <random>
#include <algorithm>
#include <iostream>

AIController::AIController(AIDifficulty difficulty): m_difficulty(difficulty){}

AIMove AIController::decideMove(const GameState& state) {
	if (m_difficulty == AIDifficulty::EASY) {
		return getRandomMove(state);
	}
	else {
		return getGreedyMove(state);
	}
}