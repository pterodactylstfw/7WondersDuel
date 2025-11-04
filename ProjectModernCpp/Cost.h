#pragma once
#include "Constants.h"
#include <map>
#include <vector>
class Cost {
private:
	int m_coinCost;
	std::map<ResourceType, int> m_resourceCosts;
public:
	Cost();
	Cost(int coins);
	Cost(int coins, const std::map<ResourceType, int>& resources);

	Cost(const Cost&) = default;
	Cost& operator=(const Cost&) = default;
	Cost(Cost&&) = default;
	Cost& operator=(Cost&&) = default;
	~Cost() = default;

	int getCoinCost() const;
	const std::map<ResourceType, int>& getResourceCosts() const;
    int getResourceCost(ResourceType type) const;
	
    void setCoinCost(int coins);
    void addResourceCost(ResourceType type, int quantity);
	
	bool isFree() const;
	bool hasResourceCost() const;
	bool hasCoinCost() const;
	int getTotalResourceCount() const;

	std::string toString() const;
	std::string toShortString() const;
	std::string toDetailedString() const;

	bool canAfford(int availableCoins,
		const std::map<ResourceType, int>& availableResources) const;
	int calculateTradingCost(
		const std::map<ResourceType, int>& playerResources,
		const std::map<ResourceType, int>& opponentProduction,
		const std::map<ResourceType, int>& tradingDiscounts = {}) const;

	Cost applyDiscount(ResourceType type, int amount) const;
	Cost applyGeneralDiscount(int amount) const;
	Cost applyGeneralDiscount(int amount, const std::vector<ResourceType>& preferredOrder) const;
	Cost applyCustomDiscount(const std::map<ResourceType, int>& discounts) const;

	Cost applyArchitectureDiscount(int amount = 2) const;
	Cost applyMasonryDiscount(int amount = 2) const;

	Cost operator+(const Cost& other) const;
	Cost& operator+=(const Cost& other);
	Cost operator-(const Cost& discount) const;
	Cost& operator-=(const Cost& discount);

	auto operator<=>(const Cost& other) const;
	bool operator==(const Cost& other) const = default;

	friend std::ostream& operator<<(std::ostream& os, const Cost& cost);
};
