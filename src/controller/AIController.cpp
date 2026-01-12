#include "AIController.h"
#include <random>
#include <algorithm>
#include <iostream>
#include <ranges>

AIController::AIController(AIDifficulty difficulty) : m_difficulty(difficulty){}

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

	auto accessibleIndices = std::views::iota(0, (int)pyramid.size())
		| std::views::filter([&](int i) { return state.isCardAccessible(i); });

	for (int i : accessibleIndices) {
		auto cardView = state.getCardView(i);
		if (!cardView.has_value()) continue;
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
				int tradeCost = player.calculateTradeCost(wonder.getCost(), opponent);
				
				if (player.getCoins() >= tradeCost) {
					double wonderScore = 15.0;
					if (wonder.getEffect().getGrantsPlayAgain())
						wonderScore += 10.0;
					if (wonder.getEffect().getShields().has_value())
						wonderScore += 5.0;
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
		double discardScore = 1.0;
		if (player.getCoins() < 4)
			discardScore += 4.0;

		double valueForOpponent = evaluateCardValue(card, opponent, player, state);
		if (valueForOpponent > 8.0) {
			discardScore += 3.0;
		}
		if (discardScore > maxScore) {
			maxScore = discardScore;
			bestCardIndex = i;
			bestAction = PlayerAction::DISCARD_FOR_COINS;
			bestWonderIndex = -1;
			
		}
	}
	if (bestCardIndex == -1)
		return getRandomMove(state);

	return { bestCardIndex, bestAction, bestWonderIndex };
}

double AIController::evaluateCardValue(const Card& card, const Player& player, const Player& opponent, const GameState& state)
{
	double score = 0.0;
	const CardEffect& effect = card.getEffect();

	if (effect.getShields().has_value()) {
		int shields = effect.getShields().value();
		score += shields * 4.0;
		int currentDiff = player.getMilitaryShields() - opponent.getMilitaryShields();

		if (currentDiff + shields >= GameConstants::MILITARY_SUPREMACY_DISTANCE)
			return 1000.0;

		if (opponent.getMilitaryShields() - player.getMilitaryShields() > 6)
			score += 15.0;
	}

	if (effect.getScienceSymbol().has_value())
		score += 6.0;

	if (effect.getVictoryPointsPerCard().has_value())
		score += effect.getVictoryPointsPerCard().value() * 1.5;

	if (card.getColor() == CardColor::PURPLE)
		score += 5.0;

	if (!effect.getProduction().isEmpty()){
		int age = state.getCurrentAge();
		if (age == 1)
			score += 5.5;
		else if (age == 2)
			score += 2.5;
		else
			score += 0.1;
	}
	if (effect.getBaseCoins().has_value())
		score += effect.getBaseCoins().value() * 0.5;
	
	int tradeCost = player.calculateTradeCost(card.getCost(), opponent);
	if (tradeCost > 0)
		score -= (tradeCost * 0.6);

	return score;
}

AIMove AIController::decideMove(const GameState& state){
	if (m_difficulty == AIDifficulty::EASY)
		return getRandomMove(state);
	else
		return getGreedyMove(state);
}
int AIController::pickWonder(const GameState& state)
{
	const auto& draftedWonders = state.getDraftedWonders();
	if (draftedWonders.empty()) return -1;

	int bestIndex = 0;
	double maxScore = -1.0;

	for (int i = 0; i < draftedWonders.size(); ++i) {
		const auto& wonder = draftedWonders[i];
		if (!wonder) continue;
		double score = 5.0;

		if (wonder->getEffect().getGrantsPlayAgain())
			score += 20.0;

		if (wonder->getEffect().getVictoryPointsPerCard().has_value())
			score += wonder->getEffect().getVictoryPointsPerCard().value() * 2.0;

		if (wonder->getEffect().getShields().has_value())
			score += wonder->getEffect().getShields().value() * 3.0;

		if (wonder->getEffect().getOpponentLosesCoins().has_value())
			score += 4.0;

		if (score > maxScore) {
			maxScore = score;
			bestIndex = i;
		}
	}
	return bestIndex;
}