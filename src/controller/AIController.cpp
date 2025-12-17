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
	const auto& pyramid = state.getPyramid();
	const Player& player = state.getCurrentPlayer();
	const Player& opponent = state.getOpponent();

	int bestCardIndex = -1;
	double maxScore = -1.0;
	PlayerAction bestAction = PlayerAction::DISCARD_FOR_COINS;
	int bestWonderIndex = -1;

	for (int i = 0; i < pyramid.size(); i++) {
		if (!state.isCardAccessible(i))
			continue;
		auto cardView = state.getCardView(i);
		if (!cardView.has_value())
			continue;
		const Card& card = cardView.value().get();

		if (player.canBuildCard(card, opponent)) {
			double score = evaluateCardValue(card, player, opponent, state);
			if (player.hasChainForCard(card) || card.getCost().isFree())
				score += 2.0;
			if (score > maxScore) {
				maxScore = score;
				bestCardIndex = i;
				bestAction = PlayerAction::CONSTRUCT_BUILDING;
				bestWonderIndex = -1;
			}
		}
		const auto& wonders = player.getWonders();
		for (int wonderID = 0; wonderID < 4; wonderID++) {
			if (wonders[wonderID] && !wonders[wonderID]->isBuilt()) {
				const Wonder& wonder = *wonders[wonderID];
				if (player.canAfford(wonder.getCost(), opponent)) {
					double wonderScore = 15.0;
					if (wonder.getEffect().getGrantsPlayAgain())
						wonderScore += 10;
					if (wonder.getEffect().getShields().has_value())
						wonderScore += 5;

					double sacrificedCardValue = evaluateCardValue(card, player, opponent, state);
					wonderScore -= (sacrificedCardValue * 0.5);

					if (wonderScore > maxScore) {
						maxScore = wonderScore;
						bestCardIndex = i;
						bestAction = PlayerAction::CONSTRUCT_WONDER;
						bestWonderIndex = wonderID;
					}
				}
			}
		}
	}
	return { bestCardIndex, bestAction, bestWonderIndex };
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
