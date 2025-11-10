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
	int totalTradeCost = 0;

	const auto& myFixedResources = resourceProduction.getFixedResources();
	const auto& myFlexibleChoices = resourceProduction.getChoices();
	const auto& opponentResources = opponent.resourceProduction.getFixedResources();

	std::vector<bool> choiceUsed(myFlexibleChoices.size());

	for (const auto& re : cost.getResourceCosts())
	{
		ResourceType resourceType = re.first;
		int quantity = re.second;

		auto resourceIt = myFixedResources.find(resourceType);
		int myFixedProduction;
		if (resourceIt != myFixedResources.end())
			myFixedProduction = resourceIt->second;
		else
			myFixedProduction = 0;

		int missingAmount = quantity - myFixedProduction;
		if (missingAmount <= 0)
			continue;

		for (int i = 0; i < myFlexibleChoices.size() && missingAmount > 0; i++)
		{
			if (choiceUsed[i])
				continue;

			const auto& choices = myFlexibleChoices[i];
			bool canProduceResource;
			if (std::find(choices.begin(), choices.end(), resourceType) != choices.end())
				canProduceResource = true;
			else canProduceResource = false;

			if (canProduceResource)
			{
				missingAmount--;
				choiceUsed[i] = true;
			}
		}

		if (missingAmount > 0)
		{
			auto discountIt = tradeDiscounts.find(resourceType);
			if (discountIt != tradeDiscounts.end() && discountIt->second == 1)
				totalTradeCost += missingAmount;
			else
			{
				auto opponentIt = opponentResources.find(resourceType);
				int opponentProduction;
				if (opponentIt != opponentResources.end())
					opponentProduction = opponentIt->second;
				else opponentProduction = 0;

				int pricePerResource = GameConstants::BASE_TRADE_COST + opponentProduction;
				totalTradeCost += missingAmount * pricePerResource;
			}
		}
	}
	totalTradeCost += cost.getCoinCost();

	return totalTradeCost;
}

bool Player::hasChainForCard(const Card& card) const
{
	for (const auto& ownedCard : constructedCards)
	{
		if (ownedCard!=nullptr && card.canBeBuiltFreelyAfter(*ownedCard))
			return true;
	}
	return false;
}

bool Player::canBuildCard(const Card& card, const Player& opponent) const
{
	if (hasChainForCard(card))
		return true;
	if (canAfford(card.getCost(), opponent))
		return true;
	return false;
}
