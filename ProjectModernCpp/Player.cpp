#include "Player.h"

Player::Player(const std::string& playerName) : 
	name(playerName), coins(GameConstants::STARTING_COINS), militaryShields(0), victoryPoints(0) { }

void Player::addCard(std::unique_ptr<Card>&& card)
{
	constructedCards.push_back(std::move(card));
}

void Player::addWonder(std::unique_ptr<Wonder>&& wonder, int index)
{
	if (index < 0 || index > 4) return;
	wonders[index] = std::move(wonder);
}

void Player::addProgressToken(std::unique_ptr<ProgressToken>&& token)
{
	progressTokens.push_back(std::move(token));
}

void Player::addResource(ResourceType type, int qty)
{
	resourceProduction.addFixedResource(type, qty);
}

void Player::addResourceChoice(std::vector<ResourceType>& choices)
{
	resourceProduction.addChoice(choices);
}

void Player::addCoins(int amount)
{
	coins += amount;
}

bool Player::removeCoins(int amount)
{
	if (coins < amount)
		return false;
	coins -= amount;
	return true;
}

void Player::addMilitaryShields(int shields)
{
	militaryShields += shields;
}

void Player::addScientificSymbol(ScientificSymbol symbol)
{
	scientificSymbols[symbol] += 1;
}

void Player::addVictoryPoints(int points)
{
	victoryPoints += points;
}

bool Player::canAfford(const Cost& cost, const Player& opponent) const
{
	const int totalCoinCost = calculateResourceCost(cost, opponent);
	return coins >= totalCoinCost;
}

int Player::calculateResourceCost(const Cost& cost, const Player& opponent) const
{

}