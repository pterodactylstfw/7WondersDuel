#include "CardEffect.h"
#include <sstream>

CardEffect& CardEffect::withVictoryPoints(int points) noexcept
{
    m_victoryPoints = points; 
    return *this;
}

CardEffect& CardEffect::withShields(int shields) noexcept
{
    m_shields = shields; 
    return *this;
}

CardEffect& CardEffect::withBaseCoins(int coins) noexcept
{
    m_baseCoins = coins; 
    return *this;
}

CardEffect& CardEffect::withScienceSymbol(ScientificSymbol symbol) noexcept
{
    m_scienceSymbol = symbol;
    return *this;
}

CardEffect& CardEffect::withDiscount(ResourceType type, int amount)
{
    m_discounts[type] += amount;
    return *this;
}

CardEffect& CardEffect::withProduction(const ResourceProduction& prod)
{
    m_production = prod;
    return *this;
}

CardEffect& CardEffect::withProduction(ResourceProduction&& prod)
{
    m_production = std::move(prod);
    return *this;
}

CardEffect& CardEffect::withPointsPerWonder(int points) noexcept
{
    m_pointsPerWonder = points; 
    return *this;
}

CardEffect& CardEffect::withPointsPerCardType(CardColor color, int points)
{
    m_pointsPerCardType[color] = points; 
    return *this;
}

CardEffect& CardEffect::withCoinsPerWonder(int coins) noexcept
{
    m_coinsPerWonder = coins;
    return *this;
}

CardEffect& CardEffect::withCoinsPerCardType(CardColor color, int coins)
{
    m_coinsPerCardType[color] = coins; 
    return *this;
}

CardEffect& CardEffect::grantsPlayAgain() noexcept
{
    m_playAgain = true; 
    return *this;
}

CardEffect& CardEffect::grantsProgressToken() noexcept
{
    m_grantsProgressToken = true; return *this;
}

CardEffect& CardEffect::grantsGuildCopy() noexcept
{
    m_copyGuild = true;
    return *this;
}

CardEffect& CardEffect::countsOpponentCards() noexcept
{
	m_countOpponentCards = true;
	return *this;
}

bool CardEffect::isEmpty() const
{
	return !m_grantsProgressToken &&
		m_coinsPerWonder == 0 &&
		m_coinsPerCardType.empty() &&
		m_pointsPerWonder == 0 &&
		m_pointsPerCardType.empty() &&
		!m_playAgain &&
		!m_countOpponentCards &&
		!m_copyGuild &&
		m_production.isEmpty() &&
		m_scienceSymbol == ScientificSymbol::NONE &&
		m_victoryPoints == 0 &&
		m_shields == 0 &&
		m_baseCoins == 0 &&
		m_discounts.empty();
}

std::string CardEffect::getDescription() const
{
    if (isEmpty()) {
        return "Niciun efect special.";
    }

    std::stringstream ss;

    if (m_victoryPoints.has_value()) 
        ss << "Ofera " << m_victoryPoints.value() << " puncte de victorie. ";
    
    if (m_baseCoins.has_value()) 
        ss << "Ofera " << m_baseCoins.value() << " monede. ";
    
    if (m_shields.has_value()) 
        ss << "Ofera " << m_shields.value() << " scuturi militare. ";
    
    if (!m_production.isEmpty()) 
        ss << "Produce: " << m_production.getDescription() << ". ";
    
    if (m_scienceSymbol.has_value()) 
        ss << "Ofera simbolul stiintific: " << scientificSymbolToString(m_scienceSymbol.value()) << ". ";

    if(!m_discounts.empty()) {
        for (const auto& [type, amount] : m_discounts) {
            ss << "Ofera reducere de " << amount << " la resursa " << resourceToString(type) << ". ";
        }
	}

    if (m_coinsPerWonder.has_value()) 
        ss << "Ofera " << m_coinsPerWonder.value() << " monede/minune construita. ";
    
    if (!m_coinsPerCardType.empty()) {
        for (const auto& [color, coins] : m_coinsPerCardType) {
            ss << "Ofera " << coins << " monede/carte de tip " << colorToString(color) << ". ";
        }
    }

    if (m_pointsPerWonder.has_value()) 
        ss << "Ofera " << m_pointsPerWonder.value() << " puncte/minune construita la finalul jocului. ";
    
    if (!m_pointsPerCardType.empty()) {
        for (const auto& [color, points] : m_pointsPerCardType) {
            ss << "Ofera " << points << " puncte/carte de tip " << colorToString(color) << " la finalul jocului. ";
        }
    }

    if (m_playAgain.has_value() && m_playAgain.value()) 
        ss << "Permite jucatorului sa joace o noua tura imediat. ";
    
    if (m_grantsProgressToken.has_value() && m_grantsProgressToken.value()) 
        ss << "Permite jucatorului sa aleaga un jeton de progres. ";
    
    if (m_copyGuild.has_value() && m_copyGuild.value()) 
        ss << "Copiaza efectul unei carti de breasla (mov) din orasul unui oponent. ";

    if(m_countOpponentCards.has_value() && m_countOpponentCards.value())
		ss << "Numara cartile din orasul oponentului. ";
    
    return ss.str();
}

std::optional<int> CardEffect::getVictoryPointsPerCard() const
{
    return m_victoryPoints;
}

std::optional<int> CardEffect::getShields() const
{
    return m_shields;
}

std::optional<int> CardEffect::getBaseCoins() const
{
    return m_baseCoins;
}

std::optional<ScientificSymbol> CardEffect::getScienceSymbol() const
{
    return m_scienceSymbol;
}

const std::map<ResourceType, int>& CardEffect::getDiscounts() const
{
    return m_discounts;
}

const ResourceProduction& CardEffect::getProduction() const
{
    return m_production;
}

std::optional<int> CardEffect::getCoinsPerWonder() const
{
    return m_coinsPerWonder;
}

const std::map<CardColor, int>& CardEffect::getCoinsPerCardType() const
{
    return m_coinsPerCardType;
}

std::optional<int> CardEffect::getPointsPerWonder() const
{
    return m_pointsPerWonder;
}

const std::map<CardColor, int>& CardEffect::getPointsPerCardType() const
{
    return m_pointsPerCardType;
}
