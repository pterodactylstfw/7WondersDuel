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

using json = nlohmann::json;

class Player
{
private:
	std::string name;
	int coins;
	int militaryShields;
	int victoryPoints;

	std::map<ScientificSymbol, int> scientificSymbols;
	std::vector< std::unique_ptr<Card>> constructedCards;
	std::array<std::unique_ptr<Wonder>,4> wonders;
	std::vector< std::unique_ptr<Wonder>> constructedWonders;
	std::vector< std::unique_ptr<ProgressToken>> progressTokens;

	ResourceProduction resourceProduction;
	std::map<ResourceType, int> tradeDiscounts;

public:

	Player() = default;
	
	Player(const std::string& playerName);

	void addCard(std::unique_ptr<Card>&& card);
	void addWonder(std::unique_ptr<Wonder>&& wonder, int index);
	//void constructWonder(int index, State_GameState& state);
	void addProgressToken(std::unique_ptr<ProgressToken>&& token);

	void addResource(ResourceType type, int qty);
	void addResourceChoice(std::vector<ResourceType>& choices);
	void addCoins(int amount);
	void removeCoins(int amount);
	void addMilitaryShields(int shields);
	void addScientificSymbol(ScientificSymbol symbol);
	void addVictoryPoints(int points);
	//void setDiscountRaw(int discount);
	//void setDiscountManufactured(int discount);

	bool canAfford(const Cost& cost, const Player& opponent) const;
	int calculateResourceCost(const Cost& cost, const Player& opponent) const;
	bool hasChainForCard(const Card& card) const;
	bool canBuildCard(const Card& card, const Player& opponent) const;

	std::map<ResourceType, int> getTotalResources() const;
	std::vector<const Card*> getCardsOfType(const CardColor& color) const;
	int getConstructedWondersCount() const;
	int getFinalScore(const Player& opponent) const; 

	bool hasScientificVictory() const;
	int getMilitaryShields() const;
	int getCoins() const;
	std::array<std::unique_ptr<Wonder>, 4>& getWonders();
    std::vector<std::unique_ptr<Wonder>>& getConstructedWonders();
    const std::string& getName() const;

	friend void to_json(json& j, const Player& player);
	friend void from_json(const json& j, Player& player);

	void removeCard(const Card& card);
};
