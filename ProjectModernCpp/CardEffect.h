#pragma once

#include <iostream>
#include <map>
#include <optional>

#include "Constants.h"
#include "ResourceProduction.h"

class CardEffect
{
private:

	std::optional<int> m_victoryPoints;
	std::optional<int> m_shields;
	std::optional<int> m_baseCoins;

	std::optional<ScientificSymbol> m_scienceSymbol;

	std::map<ResourceType, int> m_discounts;

	ResourceProduction m_production;

	std::optional<int> m_coinsPerWonder = 0;
	std::map<CardColor, int> m_coinsPerCardType;

	std::optional<int> m_pointsPerWonder = 0;
	std::map<CardColor, int> m_pointsPerCardType;

	std::optional<bool> m_playAgain = false;
	std::optional<bool> m_grantsProgressToken = false;
	std::optional<bool> m_countOpponentCards = false;
	std::optional<bool> m_copyGuild = false;


public:

	CardEffect() = default;

	CardEffect& withVictoryPoints(int points) noexcept;
	CardEffect& withShields(int shields) noexcept;
	CardEffect& withBaseCoins(int coins) noexcept;

	CardEffect& withScienceSymbol(ScientificSymbol symbol) noexcept;

	CardEffect& withDiscount(ResourceType type, int amount);

	CardEffect& withProduction(const ResourceProduction& prod);
	CardEffect& withProduction(ResourceProduction&& prod);

	CardEffect& withPointsPerWonder(int points) noexcept;
	CardEffect& withPointsPerCardType(CardColor color, int points);

	CardEffect& withCoinsPerWonder(int coins) noexcept;
	CardEffect& withCoinsPerCardType(CardColor color, int coins);

	CardEffect& grantsPlayAgain() noexcept;
	CardEffect& grantsProgressToken() noexcept;
	CardEffect& grantsGuildCopy() noexcept;
	CardEffect& countsOpponentCards() noexcept;


	bool isEmpty() const;
	
	std::string getDescription() const;

};

