#pragma once

#include <iostream>
#include <map>
#include <optional>

#include "../utils/Constants.h"
#include "ResourceProduction.h"
#include "CoreExport.h"
#include "JsonUtils.h"

class CORE_API CardEffect
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

	std::optional<std::pair<int, int>> m_pointsPerCoins;

	std::optional<bool> m_playAgain;
	std::optional<bool> m_grantsProgressToken;
	std::optional<bool> m_countOpponentCards;
	std::optional<bool> m_copyGuild;

	std::optional<std::string> m_customDescription;

	std::optional<int> m_opponentLosesCoins;
	std::optional<CardColor> m_opponentLosesCard;
	std::optional<bool> m_grantsDiscardedCard;

	std::optional<bool> m_strategyEffect;
	std::optional<bool> m_masonryEffect;      
	std::optional<bool> m_architectureEffect; 
	std::optional<bool> m_economyEffect;      
	std::optional<bool> m_mathematicsEffect;
	std::optional<bool> m_theologyEffect;
	std::optional<bool> m_urbanismEffect;

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

	CardEffect& withPointPerCoins(int points, int coins) noexcept;

	CardEffect& withCustomDescription(const std::string& desc);

	CardEffect& grantsPlayAgain() noexcept;
	CardEffect& grantsProgressToken() noexcept;
	CardEffect& grantsGuildCopy() noexcept;

	CardEffect& opponentLosesCoins(int coins) noexcept;
	CardEffect& opponentlosesCard(CardColor color) noexcept;

	CardEffect& grantsDiscardedCard() noexcept;

	CardEffect& withStrategyEffect() noexcept;
	CardEffect& withMasonryEffect() noexcept;
	CardEffect& withArchitectureEffect() noexcept;
	CardEffect& withEconomyEffect() noexcept;
	CardEffect& withMathematicsEffect() noexcept;
	CardEffect& withTheologyEffect() noexcept;
	CardEffect& withUrbanismEffect() noexcept;

	bool isEmpty() const;
	std::string getDescription() const;

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

	std::optional<std::pair<int, int>> getPointsPerCoins() const;

	bool getGrantsPlayAgain() const;
	bool getGrantsProgressToken() const;
	bool getGrantsGuildCopy() const;

	std::optional<int> getOpponentLosesCoins() const;
	std::optional<CardColor> getOpponentLosesCard() const;
	
	bool getGrantsDiscardedCard() const;

	bool hasStrategyEffect() const;
	bool hasMasonryEffect() const;
	bool hasArchitectureEffect() const;
	bool hasEconomyEffect() const;
	bool hasMathematicsEffect() const;
	bool hasTheologyEffect() const;
	bool hasUrbanismEffect() const;

	friend void to_json(json& j, const CardEffect& cardEffect);
	friend void from_json(const json& j, CardEffect& cardEffect);
};

