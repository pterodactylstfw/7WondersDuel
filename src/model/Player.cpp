#include "Player.h"

Player::Player(const std::string& playerName):
	m_name(playerName),m_isAI(false), m_coins(GameConstants::STARTING_COINS), m_militaryShields(0), m_victoryPoints(0) { }

void Player::addCard(std::unique_ptr<Card>&& card)
{
	m_constructedCards.push_back(std::move(card));
}

void Player::addWonder(std::unique_ptr<Wonder>&& wonder, int index)
{
	if (index < 0 || index > 4) return;
	m_wonders[index] = std::move(wonder);
}

void Player::addProgressToken(std::unique_ptr<ProgressToken>&& token)
{
	m_progressTokens.push_back(std::move(token));
}


void Player::addResourceProduction(ResourceProduction resourceProduction)
{
	const auto& newFixed = resourceProduction.getFixedResources();
	for (const auto& pair : newFixed)
	{
		m_resourceProduction.addFixedResource(pair.first, pair.second);
	}

	const auto& newChoices = resourceProduction.getChoices();
	for (const auto& choice : newChoices)
	{
		m_resourceProduction.addChoice(choice);
	}
}

void Player::addCoins(int amount)
{
	m_coins += amount;
}

void Player::removeCoins(int amount)
{
	if (m_coins < amount)
	{
		m_coins = 0;
		return;
	}
	m_coins -= amount;

}

void Player::addMilitaryShields(int shields)
{
	m_militaryShields += shields;
}

bool Player::addScientificSymbol(ScientificSymbol symbol)
{
	m_scientificSymbols[symbol] += 1;
	return (m_scientificSymbols[symbol] == 2);

}

void Player::addVictoryPoints(int points)
{
	m_victoryPoints += points;
}

void Player::addTradeDiscount(ResourceType type, int amount)
{
	m_tradeDiscounts.emplace(type, amount);
}

bool Player::canAfford(const Cost& cost, const Player& opponent) const
{
	const int totalCoinCost = calculateTotalCost(cost, opponent);
	return m_coins >= totalCoinCost;
}

int Player::calculateTradeCost(const Cost& cost, const Player& opponent) const
{
	int totalTradeCost = 0;

	const auto& myFixedResources = m_resourceProduction.getFixedResources();
	const auto& myFlexibleChoices = m_resourceProduction.getChoices();
	const auto& opponentResources = opponent.m_resourceProduction.getFixedResources();

	std::vector<bool> choiceUsed(myFlexibleChoices.size());

	for (const auto& re : cost.getResourceCosts())
	{
		ResourceType resourceType = re.first;
		int quantity = re.second;

		auto resourceIt = myFixedResources.find(resourceType);
		int myFixedProduction;
		if (resourceIt != myFixedResources.end())
			myFixedProduction = resourceIt->second;
		else
			myFixedProduction = 0;

		int missingAmount = quantity - myFixedProduction;
		if (missingAmount <= 0)
			continue;

		for (int i = 0; i < myFlexibleChoices.size() && missingAmount > 0; i++)
		{
			if (choiceUsed[i])
				continue;

			const auto& choices = myFlexibleChoices[i];
			bool canProduceResource;
			if (std::find(choices.begin(), choices.end(), resourceType) != choices.end())
				canProduceResource = true;
			else canProduceResource = false;

			if (canProduceResource)
			{
				missingAmount--;
				choiceUsed[i] = true;
			}
		}

		if (missingAmount > 0)
		{
			auto discountIt = m_tradeDiscounts.find(resourceType);
			if (discountIt != m_tradeDiscounts.end() && discountIt->second == 1)
				totalTradeCost += missingAmount;
			else
			{
				auto opponentIt = opponentResources.find(resourceType);
				int opponentProduction;
				if (opponentIt != opponentResources.end())
					opponentProduction = opponentIt->second;
				else opponentProduction = 0;

				int pricePerResource = GameConstants::BASE_TRADE_COST + opponentProduction;
				totalTradeCost += missingAmount * pricePerResource;
			}
		}
	}

	return totalTradeCost;
}

int Player::calculateTotalCost(const Cost& cost, const Player& opponent) const
{
	int totalCost = calculateTradeCost(cost, opponent);
	totalCost += cost.getCoinCost();
	return totalCost;
}

bool Player::hasChainForCard(const Card& card) const
{
	for (const auto& ownedCard : m_constructedCards)
	{
		if (ownedCard!=nullptr && card.canBeBuiltFreelyAfter(*ownedCard))
			return true;
	}
	return false;
}

bool Player::canBuildCard(const Card& card, const Player& opponent) const
{
	if (hasChainForCard(card))
		return true;
	if (canAfford(card.getCost(), opponent))
		return true;
	return false;
}

int Player::getVictoryPoints() const {
	return m_victoryPoints;
}

void to_json(json& j, const Player& player)
{
	j = json{
		{"name", player.m_name},
		{"isAI", player.m_isAI},
		{"coins", player.m_coins},
		{"militaryShields", player.m_militaryShields},
		{"victoryPoints", player.m_victoryPoints},
		{"scientificSymbols", player.m_scientificSymbols},
		{"constructedCards", player.m_constructedCards},
		{"wonders", player.m_wonders},
		{"constructedWonders", player.m_constructedWonders},
		{"progressTokens", player.m_progressTokens},
		{"resourceProduction", player.m_resourceProduction},
		{"tradeDiscounts", player.m_tradeDiscounts}
		
		
	};
}

void from_json(const json& j, Player& player)
{
	j.at("name").get_to(player.m_name);
	player.m_isAI = j.value("isAI", false);
	j.at("coins").get_to(player.m_coins);
	j.at("militaryShields").get_to(player.m_militaryShields);
	j.at("victoryPoints").get_to(player.m_victoryPoints);
	j.at("scientificSymbols").get_to(player.m_scientificSymbols);
	j.at("constructedCards").get_to(player.m_constructedCards);
	j.at("wonders").get_to(player.m_wonders);
	j.at("constructedWonders").get_to(player.m_constructedWonders);
	j.at("progressTokens").get_to(player.m_progressTokens);
	j.at("resourceProduction").get_to(player.m_resourceProduction);
	j.at("tradeDiscounts").get_to(player.m_tradeDiscounts);
}
std::map<ResourceType, int> Player::getTotalResources() const
{
	return m_resourceProduction.getTotalProduction();
}

std::string Player::getResourceDescription() const
{
	return m_resourceProduction.getDescription();
}

std::vector<std::reference_wrapper<const Card>> Player::getCardsOfType(const CardColor& color) const
{
	std::vector<std::reference_wrapper<const Card>> result;
	for (const auto& cardPtr : m_constructedCards) {
		if (cardPtr && cardPtr->getColor() == color)
			result.push_back(std::cref(*cardPtr));
	}
	return result;
}

int Player::getConstructedWondersCount() const
{
	return static_cast<int>(m_constructedWonders.size());
}

int Player::getFinalScore(const Player& opponent) const
{
	int score = 0;

	score += m_victoryPoints;

	for (const auto& card : m_constructedCards) {
		if (card->getEffect().getVictoryPointsPerCard().has_value()) {
			score += card->getEffect().getVictoryPointsPerCard().value();
		}
	}

	for (const auto& wonder : m_constructedWonders)
	{
		if (wonder->getEffect().getVictoryPointsPerCard().has_value()) {
			score += wonder->getEffect().getVictoryPointsPerCard().value();
		}
	}

	score += m_coins / 3;

	if (hasProgressToken(ProgressTokenType::MATHEMATICS)) {
		score += (3 * static_cast<int>(m_progressTokens.size()));
	}

	for (const auto& card : m_constructedCards)
	{
		if (card->getColor() == CardColor::PURPLE)
		{
			const auto& effect = card->getEffect();

			for (auto const& [color, points] : effect.getPointsPerCardType())
			{
				int myCount = (int)getCardsOfType(color).size();
				int oppCount = (int)opponent.getCardsOfType(color).size();
				
				score += std::max(myCount, oppCount) * points;
			}

			if (effect.getPointsPerWonder().has_value())
			{
				int myWonders = getConstructedWondersCount();
				int oppWonders = opponent.getConstructedWondersCount();
				score += std::max(myWonders, oppWonders) * effect.getPointsPerWonder().value();
			}
		}
	}

	return score;
}

void Player::setAI(bool isAI)
{
	m_isAI = isAI;
}

bool Player::isAI() const
{
	return m_isAI;
}

bool Player::hasScientificVictory() const
{
	return m_scientificSymbols.size() >= GameConstants::SCIENTIFIC_SUPREMACY_SYMBOLS;
}

int Player::getMilitaryShields() const
{
	return m_militaryShields;
}

int Player::getCoins() const
{
	return m_coins;
}

std::vector<std::unique_ptr<Card>>& Player::getConstructedCards()
{
	return m_constructedCards;
}

int Player::getCardPerColor(const CardColor& color)
{
	int count = 0;
	for (const auto& card : m_constructedCards)
		if (card->getColor() == color)
			count++;
	return count;
}

std::array<std::unique_ptr<Wonder>, 4>& Player::getWonders()
{
	return m_wonders;
}

const std::array<std::unique_ptr<Wonder>, 4>& Player::getWonders() const
{
	return m_wonders;
}

std::vector<std::unique_ptr<Wonder>>& Player::getConstructedWonders()
{
	return m_constructedWonders;
}

const std::vector<std::unique_ptr<Wonder>>& Player::getConstructedWonders() const
{
	return m_constructedWonders;
}

std::vector<std::unique_ptr<ProgressToken>>& Player::getProgressTokens()
{
	return m_progressTokens;
}

const std::vector<std::unique_ptr<ProgressToken>>& Player::getProgressTokens() const
{
	return m_progressTokens;
}

const std::string& Player::getName() const {
  return m_name;
}

std::unique_ptr<Card> Player::removeCard(const Card& card)
{
	const std::string& cardName = card.getName();

	for (auto it = m_constructedCards.begin(); it != m_constructedCards.end(); )
	{
		if ( it->get()->getName() == cardName)
		{
			const auto& cardProduction = (*it)->getEffect().getProduction();
			if (!cardProduction.isEmpty())
				m_resourceProduction.removeFixedResource(cardProduction);

			auto removed = std::move(*it);
			m_constructedCards.erase(it);
			return removed;
		}
		else
			it++;
	}
	return nullptr;
}

bool Player::hasProgressToken(ProgressTokenType type) const {
	for (const auto& token : m_progressTokens) {
		if (token->getType() == type) return true;
	}
	return false;
}