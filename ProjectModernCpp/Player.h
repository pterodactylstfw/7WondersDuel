#pragma once
#include <string>
#include <map>
#include <vector>
#include <memory>
#include <array>

#include "Constants.h"
#include "Wonder.h"
#include "Card.h"
#include "ResourceProduction.h"
#include "ProgressToken.h"
#include "JsonUtils.h"

class Player
{
private:
	std::string m_name;
	int m_coins;
	int m_militaryShields;
	int m_victoryPoints;

	std::map<ScientificSymbol, int> m_scientificSymbols;
	std::vector< std::unique_ptr<Card>> m_constructedCards;
	std::array<std::unique_ptr<Wonder>,4> m_wonders;
	std::vector<std::unique_ptr<Wonder>> m_constructedWonders;
	std::vector<std::unique_ptr<ProgressToken>> m_progressTokens;

	ResourceProduction m_resourceProduction;
	std::map<ResourceType, int> m_tradeDiscounts;

public:

	Player() = default;
	
	Player(const std::string& playerName);

	void addCard(std::unique_ptr<Card>&& card);
	void addWonder(std::unique_ptr<Wonder>&& wonder, int index);
	void addProgressToken(std::unique_ptr<ProgressToken>&& token);

	void addResource(ResourceType type, int qty);
	void addResourceChoice(std::vector<ResourceType> choices);
	void addCoins(int amount);
	void removeCoins(int amount);
	void addMilitaryShields(int shields);
	void addScientificSymbol(ScientificSymbol symbol);
	void addVictoryPoints(int points);

	bool canAfford(const Cost& cost, const Player& opponent) const;
	int calculateResourceCost(const Cost& cost, const Player& opponent) const;
	bool hasChainForCard(const Card& card) const;
	bool canBuildCard(const Card& card, const Player& opponent) const;

	std::map<ResourceType, int> getTotalResources() const;
	std::string_view getResourceDescription() const;
	std::vector<const Card*> getCardsOfType(const CardColor& color) const;
	int getConstructedWondersCount() const;
	int getFinalScore(const Player& opponent) const; 

	bool hasScientificVictory() const;
	int getMilitaryShields() const;
	int getCoins() const;
	std::array<std::unique_ptr<Wonder>, 4>& getWonders();
	const std::array<std::unique_ptr<Wonder>, 4>& getWonders() const; 
    std::vector<std::unique_ptr<Wonder>>& getConstructedWonders();
    const std::vector<std::unique_ptr<Wonder>>& getConstructedWonders() const;
	std::vector<std::unique_ptr<ProgressToken>>& getProgressTokens();
	const std::vector<std::unique_ptr<ProgressToken>>& getProgressTokens() const;
    const std::string& getName() const;
	int getVictoryPoints() const;

	friend void to_json(json& j, const Player& player);
	friend void from_json(const json& j, Player& player);

	std::unique_ptr<Card> removeCard(const Card& card);
};
