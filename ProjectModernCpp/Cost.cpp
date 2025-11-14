#include "Cost.h"
#include <sstream>
#include <algorithm>
#include <numeric>
#include <compare>

Cost::Cost() :m_coinCost(0) {}
Cost::Cost(int coins) : m_coinCost(coins) {}
Cost::Cost(int coins, const std::map<ResourceType, int>& resources)
    : m_coinCost(coins), m_resourceCosts(resources) {
}

Cost& Cost::withCoinCost(int coins)
{
    m_coinCost = coins;
	return *this;
}

Cost& Cost::withResourceCost(ResourceType type, int amount)
{
    m_resourceCosts[type] = amount;
	return *this;
}

int Cost::getCoinCost() const {
    return m_coinCost;
}

const std::map<ResourceType, int>& Cost::getResourceCosts() const {
    return m_resourceCosts;
}

int Cost::getResourceCost(ResourceType type) const {
    auto it = m_resourceCosts.find(type);
    if (it != m_resourceCosts.end())
        return  it->second;
    else
        return 0;
}

bool Cost::isFree() const {
    return m_coinCost == 0 && m_resourceCosts.empty();
}

bool Cost::hasResourceCost() const {
    return !m_resourceCosts.empty();
}

bool Cost::hasCoinCost() const {
    return m_coinCost > 0;
}

int Cost::getTotalResourceCount() const {
    return std::accumulate(m_resourceCosts.begin(), m_resourceCosts.end(), 0,
        [](int sum, const auto& pair) { return sum + pair.second; }
    );
}

std::string Cost::toString() const {
    if (isFree())
        return "FREE";
    std::stringstream ss;
    if (m_coinCost > 0) {
        ss << m_coinCost << " coin";
        if (m_coinCost > 1)
            ss << "s";
    }
    if (!m_resourceCosts.empty()) {
        if (m_coinCost > 0)
            ss << " + ";
        bool first = true;
        for (const auto& [type, quantity] : m_resourceCosts) {
            if (!first)
                ss << " + ";
            ss << quantity << " " << resourceToString(type);
            if (quantity > 1)
                ss << "s";
            first = false;
        }
    }
    return ss.str();
}

std::string Cost::toShortString() const {
    if (isFree())
        return "FREE";
    std::stringstream ss;
    if (m_coinCost > 0) {
        ss << m_coinCost << "G";
    }
    if (!m_resourceCosts.empty()) {
        if (m_coinCost > 0)
            ss << "+";
        bool first = true;
        for (const auto& [type, quantity] : m_resourceCosts) {
            if (!first)
                ss << "+";
            ss << quantity;
            switch (type) {
            case ResourceType::WOOD:    ss << "W"; break;
            case ResourceType::CLAY:    ss << "C"; break;
            case ResourceType::STONE:   ss << "S"; break;
            case ResourceType::GLASS:   ss << "Gl"; break;
            case ResourceType::PAPYRUS: ss << "P"; break;
            default: break;
            }
            first = false;
        }
    }

    return ss.str();
}

bool Cost::canAfford(int availableCoins,
    const std::map<ResourceType, int>& availableResources) const {
    if (m_coinCost > availableCoins) {
        return false;
    }
    for (const auto& pair : m_resourceCosts) {
        ResourceType type = pair.first;
        int required = pair.second;

        auto it = availableResources.find(type);
        int available = (it != availableResources.end()) ? it->second : 0;
        if (available < required) {
            return false;
        }
    }

    return true;
}

int Cost::calculateTradingCost(const std::map<ResourceType, int>& playerResources, const std::map<ResourceType, int>& opponentProduction, const std::map<ResourceType, int>& tradingDiscounts) const {
    int totalTradingCost = 0;
    for (const auto& [type, required] : m_resourceCosts) {
        auto playerIt = playerResources.find(type);
        int available = (playerIt != playerResources.end()) ? playerIt->second : 0;
        int missing = required - available;
        if (missing <= 0) {
            continue;
        }

        auto discountIt = tradingDiscounts.find(type);
        if (discountIt != tradingDiscounts.end() && discountIt->second == 1) {
            totalTradingCost += missing * 1;
        }
        else {
            auto opponentIt = opponentProduction.find(type);
            int opponentCount = (opponentIt != opponentProduction.end()) ? opponentIt->second : 0;
            int costPerResource = GameConstants::BASE_TRADE_COST + opponentCount;

            totalTradingCost += missing * costPerResource;
        }
    }
    return totalTradingCost;
}

Cost Cost::applyDiscount(ResourceType type, int amount) const {
    Cost result = *this;
    auto it = result.m_resourceCosts.find(type);
    if (it != result.m_resourceCosts.end()) {
        it->second = std::max(0, it->second - amount);
        if (it->second == 0) {
            result.m_resourceCosts.erase(it);
        }
    }
    return result;
}


Cost Cost::applyCustomDiscount(const std::map<ResourceType, int>& discounts) const {
    Cost result = *this;
    for (const auto& [type, discountAmount] : discounts) {
        auto it = result.m_resourceCosts.find(type);
        if (it != result.m_resourceCosts.end()) {
            it->second = std::max(0, it->second - discountAmount);
            if (it->second == 0) {
                result.m_resourceCosts.erase(it);
            }
        }
    }

    return result;
}
Cost Cost::operator-(const Cost& discount) const {
    Cost result = *this;
    result.m_coinCost = std::max(0, result.m_coinCost - discount.m_coinCost);
    for (const auto& [type, discountAmount] : discount.m_resourceCosts) {
        auto it = result.m_resourceCosts.find(type);
        if (it != result.m_resourceCosts.end()) {
            it->second = std::max(0, it->second - discountAmount);
            if (it->second == 0) {
                result.m_resourceCosts.erase(it);
            }
        }
    }
    return result;
}

Cost& Cost::operator-=(const Cost& discount) {
    *this = *this - discount;
    return *this;
}

std::strong_ordering Cost::operator<=>(const Cost& other) const {
    int thisTotal = m_coinCost + getTotalResourceCount();
    int otherTotal = other.m_coinCost + other.getTotalResourceCount();
    if (auto cmp = thisTotal <=> otherTotal; cmp != 0)
        return cmp;
    if (auto cmp = m_coinCost <=> other.m_coinCost; cmp != 0)
        return cmp;

    return m_resourceCosts <=> other.m_resourceCosts;
}

bool Cost::operator==(const Cost& other) const {
    return m_coinCost == other.m_coinCost &&
        m_resourceCosts == other.m_resourceCosts;
}

Cost Cost::operator+(const Cost& other) const {
    Cost result = *this;
    result.m_coinCost += other.m_coinCost;
    for (const auto& [type, amount] : other.m_resourceCosts) {
        result.m_resourceCosts[type] += amount;
    }
    return result;
}

Cost& Cost::operator+=(const Cost& other) {
    *this = *this + other;
    return *this;
}

Cost Cost::applyGeneralDiscount(int amount, const std::vector<ResourceType>& preferredOrder) const {
    Cost result = *this;
    int remaining = amount;

    for (ResourceType type : preferredOrder) {
        if (remaining <= 0) break;

        auto it = result.m_resourceCosts.find(type);
        if (it != result.m_resourceCosts.end()) {
            int canDiscount = std::min(remaining, it->second);
            it->second -= canDiscount;
            remaining -= canDiscount;

            if (it->second == 0) {
                result.m_resourceCosts.erase(it);
            }
        }
    }

    return result;
}

Cost Cost::applyArchitectureDiscount(int amount) const {
    return applyGeneralDiscount(amount);
}

Cost Cost::applyMasonryDiscount(int amount) const {
    return applyGeneralDiscount(amount);
}

std::ostream& operator<<(std::ostream& os, const Cost& cost) {
    os << cost.toString();
    return os;
}

void to_json(json& j, const Cost& cost)
{
    j = json{
        {"coinCost", cost.m_coinCost},
        {"resourceCosts", cost.m_resourceCosts}
    };
}

void from_json(const json& j, Cost& cost)
{
    cost.m_coinCost = j.value("coinCost", 0);
    cost.m_resourceCosts = j.value("resourceCosts", std::map<ResourceType, int>{});
}
