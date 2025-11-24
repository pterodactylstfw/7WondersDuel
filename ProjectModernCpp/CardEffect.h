#pragma once

#include <iostream>
#include <map>
#include <optional>

#include "Constants.h"
#include "ResourceProduction.h"

using json = nlohmann::json;

class CardEffect
{
private:
	std::optional<int> m_victoryPoints;
	std::optional<int> m_shields;
	std::optional<int> m_baseCoins;
	std::optional<ScientificSymbol> m_scienceSymbol;

	std::map<ResourceType, int> m_discounts;
	ResourceProduction m_production;

	std::optional<int> m_coinsPerWonder;
	std::map<CardColor, int> m_coinsPerCardType;

	std::optional<int> m_pointsPerWonder;
	std::map<CardColor, int> m_pointsPerCardType;

	std::optional<bool> m_playAgain;
	std::optional<bool> m_grantsProgressToken;
	std::optional<bool> m_countOpponentCards;
	std::optional<bool> m_copyGuild;
	std::optional<std::string> m_customDescription;

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
	CardEffect& withCustomDescription(const std::string& desc);
	CardEffect& grantsPlayAgain() noexcept;
	CardEffect& grantsProgressToken() noexcept;
	CardEffect& grantsGuildCopy() noexcept;
	CardEffect& countsOpponentCards() noexcept;


	bool isEmpty() const;
	
	std::string getDescription() const;


	friend void to_json(json& j, const CardEffect& cardEffect);
	friend void from_json(const json& j, CardEffect& cardEffect);

	std::optional<int> getVictoryPointsPerCard() const;
	std::optional<int> getShields() const;
	std::optional<int> getBaseCoins() const;
	std::optional<ScientificSymbol> getScienceSymbol() const;

	const std::map<ResourceType, int>& getDiscounts() const;
	const ResourceProduction& getProduction() const;

	std::optional<int> getCoinsPerWonder() const;
	const std::map<CardColor, int>& getCoinsPerCardType() const;

	std::optional<int> getPointsPerWonder() const;
	const std::map<CardColor, int>& getPointsPerCardType() const;
};

