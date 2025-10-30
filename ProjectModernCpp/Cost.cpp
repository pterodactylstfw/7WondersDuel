#include "Cost.h"
#include <sstream>

Cost::Cost():m_coinCost(0){}
Cost::Cost(int coins) : m_coinCost(coins) {}
Cost::Cost(int coins, const std::map<ResourceType, int>& resources)
    : m_coinCost(coins), m_resourceCosts(resources) {}

int Cost::getCoinCost() const{ 
    return m_coinCost; 
}

const std::map<ResourceType, int>& Cost::getResourceCosts() const{
    return m_resourceCosts;
}

int Cost::getResourceCost(ResourceType type) const {
    auto it = m_resourceCosts.find(type);
    if (it != m_resourceCosts.end())
        return  it->second;
    else
        return 0;
}

bool Cost::isFree() const{
    return m_coinCost == 0 && m_resourceCosts.empty();
}

void Cost::addResourceCost(ResourceType type, int quantity) {
    if (type == ResourceType::NONE)
        return;
    m_resourceCosts[type] += quantity;
}

void Cost::setCoinCost(int coins) {
    m_coinCost = coins;
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