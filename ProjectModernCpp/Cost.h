#pragma once
#include "Constants.h"
#include <map>
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
	bool isFree() const;
    void setCoinCost(int coins);
    void addResourceCost(ResourceType type, int quantity);
    std::string toString() const;
};
