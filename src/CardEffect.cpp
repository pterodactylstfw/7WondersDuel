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
    m_discounts.emplace(type, amount);
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

CardEffect& CardEffect::withPointPerCoins(int points, int coins) noexcept
{
    m_pointsPerCoins = std::make_pair(points, coins);
    return *this;
}

CardEffect& CardEffect::withCustomDescription(const std::string& desc)
{
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
    m_grantsProgressToken = true;
    return *this;
}

CardEffect& CardEffect::grantsGuildCopy() noexcept
{
    m_copyGuild = true;
    return *this;
}

CardEffect& CardEffect::opponentLosesCoins(int coins) noexcept
{
    m_opponentLosesCoins = coins;
    return *this;
}

CardEffect& CardEffect::opponentlosesCard(CardColor color) noexcept
{
    m_opponentLosesCard = color;
    return *this;
}

CardEffect& CardEffect::grantsDiscardedCard() noexcept
{
    m_grantsDiscardedCard = true;
    return *this;
}

CardEffect& CardEffect::withStrategyEffect() noexcept
{
    m_strategyEffect = true;
    return *this;
}

CardEffect& CardEffect::withMasonryEffect() noexcept
{
    m_masonryEffect = true;
    return *this;
}

CardEffect& CardEffect::withArchitectureEffect() noexcept
{
    m_architectureEffect = true;
    return *this;
}

CardEffect& CardEffect::withEconomyEffect() noexcept
{
    m_economyEffect = true;
    return *this;
}

CardEffect& CardEffect::withMathematicsEffect() noexcept
{
    m_mathematicsEffect = true;
    return *this;
}

CardEffect& CardEffect::withTheologyEffect() noexcept
{
    m_theologyEffect = true;
    return *this;
}

CardEffect& CardEffect::withUrbanismEffect() noexcept
{
    m_urbanismEffect = true;
    return *this;
}


bool CardEffect::hasStrategyEffect() const
{
    return m_strategyEffect.value_or(false);
}

bool CardEffect::hasMasonryEffect() const
{
    return m_masonryEffect.value_or(false);
}

bool CardEffect::hasArchitectureEffect() const
{
    return m_architectureEffect.value_or(false);
}

bool CardEffect::hasEconomyEffect() const
{
    return m_economyEffect.value_or(false);
}

bool CardEffect::hasMathematicsEffect() const
{
    return m_mathematicsEffect.value_or(false);
}

bool CardEffect::hasTheologyEffect() const
{
    return m_theologyEffect.value_or(false);
}

bool CardEffect::hasUrbanismEffect() const
{
    return m_urbanismEffect.value_or(false);
}

bool CardEffect::isEmpty() const
{
    return !m_customDescription.has_value() &&
        !m_grantsProgressToken &&
        m_coinsPerWonder == 0 &&
        m_coinsPerCardType.empty() &&
        m_pointsPerWonder == 0 &&
        m_pointsPerCardType.empty() &&
        m_pointsPerCoins.has_value() &&
        !m_playAgain &&
        !m_strategyEffect && !m_masonryEffect && !m_architectureEffect && !m_economyEffect && !m_mathematicsEffect &&
        !m_countOpponentCards &&
        !m_copyGuild &&
        m_production.isEmpty() &&
        m_scienceSymbol == ScientificSymbol::NONE &&
        m_victoryPoints == 0 &&
        m_shields == 0 &&
        m_baseCoins == 0 &&
        !m_discounts.empty() &&
        !m_opponentLosesCoins &&
        !m_opponentLosesCard &&
        !m_grantsDiscardedCard;
}

std::string CardEffect::getDescription() const // pentru aceasta functie am pus doar efectele cartilor normale, pentru ca guild cards, wonders si progress tokens au custom description
{
    if (m_customDescription.has_value() && !m_customDescription.value().empty())
    {
        return m_customDescription.value();
    }

    if (isEmpty())
    {
        return "No special effect.";
    }

    std::stringstream ss;

    if (m_victoryPoints.has_value())
    {
        ss << "Grants " << m_victoryPoints.value() << " VP. ";
    }

    if (m_baseCoins.has_value())
    {
        ss << "Grants " << m_baseCoins.value() << " coins. ";
    }

    if (m_shields.has_value())
    {
        ss << "Grants " << m_shields.value() << " shields. ";
    }

    if (m_scienceSymbol.has_value())
    {
        ss << "Science Symbol: " << scientificSymbolToString(m_scienceSymbol.value()) << ". ";
    }

    if (!m_discounts.empty())
    {
        for (const auto& discount : m_discounts)
        {
            ss << "Discount of " << discount.second << " on " << resourceToString(discount.first) << ". ";
        }
    }

    if (!m_production.isEmpty())
    {
        ss << "Resource Production: " << m_production.getDescription();
    }

    if (m_coinsPerWonder.has_value())
    {
        ss << "Grants " << m_coinsPerWonder.value() << " coins per constructed Wonder. ";
    }

    for (const auto& [color, points] : m_pointsPerCardType)
    {
        ss << "Grants " << points << " VP per " << colorToString(color) << " card. ";
    }

    for (const auto& [color, coins] : m_coinsPerCardType)
    {
        ss << "Grants " << coins << " coins per " << colorToString(color) << " card. ";
    }

    return ss.str();
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
        {"customDescription", cardEffect.m_customDescription},
        {"strategyEffect", cardEffect.m_strategyEffect},
        {"masonryEffect", cardEffect.m_masonryEffect},
        {"architectureEffect", cardEffect.m_architectureEffect},
        {"economyEffect", cardEffect.m_economyEffect},
        {"mathematicsEffect", cardEffect.m_mathematicsEffect}
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
    cardEffect.m_copyGuild = get_optional<bool>(j, "copyGuild");
    cardEffect.m_customDescription = get_optional<std::string>(j, "customDescription");

    cardEffect.m_strategyEffect = get_optional<bool>(j, "strategyEffect");
    cardEffect.m_masonryEffect = get_optional<bool>(j, "masonryEffect");
    cardEffect.m_architectureEffect = get_optional<bool>(j, "architectureEffect");
    cardEffect.m_economyEffect = get_optional<bool>(j, "economyEffect");
    cardEffect.m_mathematicsEffect = get_optional<bool>(j, "mathematicsEffect");

    cardEffect.m_discounts = j.value("discounrs", std::map<ResourceType, int>{});
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

std::optional<std::pair<int, int>> CardEffect::getPointsPerCoins() const
{
    return m_pointsPerCoins;
}

bool CardEffect::getGrantsPlayAgain() const
{
    return m_playAgain ? true: false;
}

bool CardEffect::getGrantsProgressToken() const
{
    return m_grantsProgressToken ? true : false;
}

bool CardEffect::getGrantsGuildCopy() const
{
    return m_copyGuild ? true : false;
}

std::optional<int> CardEffect::getOpponentLosesCoins() const
{
    return m_opponentLosesCoins;
}

std::optional<CardColor> CardEffect::getOpponentLosesCard() const
{
    return m_opponentLosesCard;
}

bool CardEffect::getGrantsDiscardedCard() const
{
    return m_grantsDiscardedCard ? true : false;
}
