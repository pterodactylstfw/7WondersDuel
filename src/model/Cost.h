#pragma once
#include "Constants.h"
#include <map>
#include <vector>
#include <compare>
#include <cstdint>
#include "JsonUtils.h"
#include "CoreExport.h"

class CORE_API Cost {
private:
	uint8_t m_coinCost;
	std::map<ResourceType, uint8_t> m_resourceCosts;
public:
	Cost();
	Cost(uint8_t coins);
	Cost(uint8_t coins, const std::map<ResourceType, uint8_t>& resources);
	Cost(const Cost&) = default;
	Cost& operator=(const Cost&) = default;
	Cost(Cost&&) = default;
	Cost& operator=(Cost&&) = default;
	~Cost() = default;

	Cost& withCoinCost(uint8_t coins);
	Cost& withResourceCost(ResourceType type, uint8_t amount);

	uint8_t getCoinCost() const;
	const std::map<ResourceType, uint8_t>& getResourceCosts() const;
	uint8_t getResourceCost(ResourceType type) const;
	bool isFree() const;

	bool hasResourceCost() const;
	bool hasCoinCost() const;
	int getTotalResourceCount() const;

	std::string toString() const;
	std::string toShortString() const;
	bool canAfford(int availableCoins, const std::map<ResourceType, int>& availableResources) const;
	int calculateTradingCost(const std::map<ResourceType, int>& playerResources, const std::map<ResourceType, int>& opponentProduction, const std::map<ResourceType, int>& tradingDiscounts = {}) const;

	Cost applyDiscount(ResourceType type, int amount) const;
	Cost applyCustomDiscount(const std::map<ResourceType, int>& discounts) const;
	Cost applyGeneralDiscount(int amount, const std::vector<ResourceType>& preferredOrder = {}) const;

	Cost applyArchitectureDiscount(int amount = 2) const;
	Cost applyMasonryDiscount(int amount = 2) const;

	Cost operator+(const Cost& other) const;
	Cost& operator+=(const Cost& other);
	Cost operator-(const Cost& discount) const;
	Cost& operator-=(const Cost& discount);

	std::strong_ordering operator<=>(const Cost& other) const;
	bool operator==(const Cost& other) const;

	friend std::ostream& operator<<(std::ostream& os, const Cost& cost);

	friend void to_json(json& j, const Cost& cost);
	friend void from_json(const json& j, Cost& cost);
};
