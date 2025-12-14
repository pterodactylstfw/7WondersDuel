#include "AIController.h"
#include <random>
#include <algorithm>
#include <iostream>

AIMove AIController::getRandomMove(const GameState& state)
{
	std::vector<int> accessibleIndices;
	const auto& pyramid = state.getPyramid();

	for (int i = 0; i < pyramid.size(); i++) {
		if (state.isCardAccessible(i))
			accessibleIndices.push_back(i);
	}
	if (accessibleIndices.empty())
		return { -1,PlayerAction::DISCARD_FOR_COINS };

	std::random_device rd;
	std::mt19937 g(rd());
	std::shuffle(accessibleIndices.begin(), accessibleIndices.end(), g);

	int chosenIndex = accessibleIndices[0];
	auto cardView = state.getCardView(chosenIndex);
	if (!cardView.has_value())
		return { -1, PlayerAction::DISCARD_FOR_COINS };

	const Card& card = cardView.value().get();
	const Player& player = state.getCurrentPlayer();
	const Player& opponent = state.getOpponent();
	if (player.canBuildCard(card, opponent)) 
		return { chosenIndex, PlayerAction::CONSTRUCT_BUILDING };

	return { chosenIndex,PlayerAction::DISCARD_FOR_COINS };
}

AIMove AIController::getGreedyMove(const GameState& state)
{
	// to do 

	//temporar pentru ca nu compila
	return getRandomMove(state);
}

double AIController::evaluateCardValue(const Card& card, const Player& player, const Player& opponent, const GameState& state)
{
	// to do 

	//temporar pentru ca nu compila
	return 0.0;
}

AIController::AIController(AIDifficulty difficulty): m_difficulty(difficulty){}

AIMove AIController::decideMove(const GameState& state) {
	if (m_difficulty == AIDifficulty::EASY) {
		return getRandomMove(state);
	}
	/*
	Implementing greedy AI next week
	else {
		return getGreedyMove(state);
	}
	*/
}
