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

CardEffect& CardEffect::withCustomDescription(const std::string& desc) {
    m_customDescription = desc;
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
    if (m_customDescription.has_value() && !m_customDescription.value().empty()) {
        return m_customDescription.value();
    }
    if (isEmpty()) {
        return "Niciun efect special.";
    }

    std::stringstream ss;

    if (m_victoryPoints.has_value())
        ss << "Ofera " << m_victoryPoints.value() << " VP. ";

    if (m_baseCoins.has_value())
        ss << "Ofera " << m_baseCoins.value() << " monede. ";

    if (m_shields.has_value())
        ss << "Ofera " << m_shields.value() << " scuturi. ";

    if (!m_production.isEmpty())
        ss << "Produce: " << m_production.getDescription() << ". ";

    if (m_scienceSymbol.has_value())
        ss << "Simbol: " << scientificSymbolToString(m_scienceSymbol.value()) << ". ";

    if (!m_discounts.empty()) {
        for (const auto& [type, amount] : m_discounts) {
            ss << "Reducere " << amount << " la " << resourceToString(type) << ". ";
        }
    }

    if (m_playAgain.has_value() && m_playAgain.value())
        ss << "Joci din nou. ";

    return ss.str();
}

template<typename T>
std::optional<T> get_optional(const json& j, const std::string& key) {
    if (j.contains(key) && !j[key].is_null()) {
        return std::optional<T>(j[key].get<T>());
    }
    return std::nullopt;
}

void to_json(json& j, const CardEffect& cardEffect)
{
    j = json{
        {"victoryPoints", cardEffect.m_victoryPoints},
        {"shields", cardEffect.m_shields},
        {"baseCoins", cardEffect.m_baseCoins},
        {"scienceSymbol", cardEffect.m_scienceSymbol},
        {"discounts", cardEffect.m_discounts},
        {"production", cardEffect.m_production},
        {"coinsPerWonder", cardEffect.m_coinsPerWonder},
        {"coinsPerCardType", cardEffect.m_coinsPerCardType},
        {"pointsPerWonder", cardEffect.m_pointsPerWonder},
        {"pointsPerCardType", cardEffect.m_pointsPerCardType},
        {"playAgain", cardEffect.m_playAgain},
        {"grantsProgressToken", cardEffect.m_grantsProgressToken},
        {"countOpponentCards", cardEffect.m_countOpponentCards},
        {"copyGuild", cardEffect.m_copyGuild},
        {"customDescription", cardEffect.m_customDescription} 
    };
}

void from_json(const json& j, CardEffect& cardEffect)
{
    cardEffect.m_victoryPoints = get_optional<int>(j, "victoryPoints");
    cardEffect.m_shields = get_optional<int>(j, "shields");
    cardEffect.m_baseCoins = get_optional<int>(j, "baseCoins");
    cardEffect.m_scienceSymbol = get_optional<ScientificSymbol>(j, "scienceSymbol");
    cardEffect.m_coinsPerWonder = get_optional<int>(j, "coinsPerWonder");
    cardEffect.m_pointsPerWonder = get_optional<int>(j, "pointsPerWonder");
    cardEffect.m_playAgain = get_optional<bool>(j, "playAgain");
    cardEffect.m_grantsProgressToken = get_optional<bool>(j, "grantsProgressToken");
    cardEffect.m_countOpponentCards = get_optional<bool>(j, "countOpponentCards");
	cardEffect.m_copyGuild = get_optional<bool>(j, "copyGuild");    // am folosit functia template pentru a evita repetarea

    cardEffect.m_discounts = j.value("discounts", std::map<ResourceType, int>{});
    cardEffect.m_production = j.value("production", ResourceProduction{});
    cardEffect.m_coinsPerCardType = j.value("coinsPerCardType", std::map<CardColor, int>{});
    cardEffect.m_pointsPerCardType = j.value("pointsPerCardType", std::map<CardColor, int>{});
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
