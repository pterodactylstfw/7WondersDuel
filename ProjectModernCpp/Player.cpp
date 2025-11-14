#include "Player.h"

NLOHMANN_JSON_NAMESPACE_BEGIN

template <typename T> struct adl_serializer<std::unique_ptr<T>> {
	template <typename BasicJsonType> static void to_json(BasicJsonType& json_value, const std::unique_ptr<T>& ptr)
	{
		if (ptr.get()) {
			json_value = *ptr;
		}
		else {
			json_value = nullptr;
		}
	}

	template <typename BasicJsonType> static void from_json(const BasicJsonType& json_value, std::unique_ptr<T>& ptr)
	{
		T inner_val = json_value.template get<T>();
		ptr = std::make_unique<T>(std::move(inner_val));
	}
};

NLOHMANN_JSON_NAMESPACE_END // pentru rezolvarea problemei cu serializarea pointerilor unique - dupa discutia de pe forum nlohmann

Player::Player(const std::string& playerName) : 
	name(playerName), coins(GameConstants::STARTING_COINS), militaryShields(0), victoryPoints(0) { }

void Player::addCard(std::unique_ptr<Card>&& card)
{
	constructedCards.push_back(std::move(card));
}

void Player::addWonder(std::unique_ptr<Wonder>&& wonder, int index)
{
	if (index < 0 || index > 4) return;
	wonders[index] = std::move(wonder);
}

void Player::addProgressToken(std::unique_ptr<ProgressToken>&& token)
{
	progressTokens.push_back(std::move(token));
}


void Player::addResource(ResourceType type, int qty)
{
	resourceProduction.addFixedResource(type, qty);
}

void Player::addResourceChoice(std::vector<ResourceType>& choices)
{
	resourceProduction.addChoice(choices);
}

void Player::addCoins(int amount)
{
	coins += amount;
}

bool Player::removeCoins(int amount)
{
	if (coins < amount)
		return false;
	coins -= amount;
	return true;
}

void Player::addMilitaryShields(int shields)
{
	militaryShields += shields;
}

void Player::addScientificSymbol(ScientificSymbol symbol)
{
	scientificSymbols[symbol] += 1;
}

void Player::addVictoryPoints(int points)
{
	victoryPoints += points;
}

bool Player::canAfford(const Cost& cost, const Player& opponent) const
{
	const int totalCoinCost = calculateResourceCost(cost, opponent);
	return coins >= totalCoinCost;
}

int Player::calculateResourceCost(const Cost& cost, const Player& opponent) const
{
	int totalTradeCost = 0;

	const auto& myFixedResources = resourceProduction.getFixedResources();
	const auto& myFlexibleChoices = resourceProduction.getChoices();
	const auto& opponentResources = opponent.resourceProduction.getFixedResources();

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
			auto discountIt = tradeDiscounts.find(resourceType);
			if (discountIt != tradeDiscounts.end() && discountIt->second == 1)
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
	totalTradeCost += cost.getCoinCost();

	return totalTradeCost;
}

bool Player::hasChainForCard(const Card& card) const
{
	for (const auto& ownedCard : constructedCards)
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

void to_json(json& j, const Player& player)
{
	j = json{
		{"name", player.name},
		{"coins", player.coins},
		{"militaryShields", player.militaryShields},
		{"victoryPoints", player.victoryPoints},
		{"scientificSymbols", player.scientificSymbols},
		{"constructedCards", player.constructedCards},
		{"wonders", player.wonders},
		{"constructedWonders", player.constructedWonders},
		{"progressTokens", player.progressTokens},
		{"resourceProduction", player.resourceProduction},
		{"tradeDiscounts", player.tradeDiscounts}
		
		
	};
}

void from_json(const json& j, Player& player)
{
	j.at("name").get_to(player.name);
	j.at("coins").get_to(player.coins);
	j.at("militaryShields").get_to(player.militaryShields);
	j.at("victoryPoints").get_to(player.victoryPoints);
	j.at("scientificSymbols").get_to(player.scientificSymbols);
	j.at("constructedCards").get_to(player.constructedCards);
	j.at("wonders").get_to(player.wonders);
	j.at("constructedWonders").get_to(player.constructedWonders);
	j.at("progressTokens").get_to(player.progressTokens);
	j.at("resourceProduction").get_to(player.resourceProduction);
	j.at("tradeDiscounts").get_to(player.tradeDiscounts);
}
std::map<ResourceType, int> Player::getTotalResources() const
{
	return resourceProduction.getTotalProduction();
}

int Player::getCardsOfType(const CardColor& color) const
{
	int nr = 0;
	for (const auto& card : constructedCards)
	{
		if (card && card->getColor() == color)
			nr++;
	}

	return nr;
}

int Player::getConstructedWondersCount() const
{
	return constructedWonders.size();
}

int Player::getFinalScore(const Player& opponent) const
{
	//+ military victory points ce urmeaza sa fie discutat
	int nr = 0;

	for (const auto& card : constructedCards)
	{
		const auto& vp = card->getEffect().getVictoryPointsPerCard();
		if (vp.has_value())
			nr += vp.value();
	}

	for (const auto& wonder : constructedWonders)
	{
		nr  += wonder->getVictoryPoints();
	}

	//+ victory points from progress token cand am clasa

	nr += coins / 3;
	return nr;
}

bool Player::hasScientificVictory() const
{
	return scientificSymbols.size() > GameConstants::SCIENTIFIC_SUPREMACY_SYMBOLS;
}

int Player::getMilitaryShields() const
{
	return militaryShields;
}

int Player::getCoins() const
{
	return coins;
}

std::array<std::unique_ptr<Wonder>, 4>& Player::getWonders()
{
	return wonders;
}

std::vector<std::unique_ptr<Wonder>>& Player::getConstructedWonders()
{
	return constructedWonders;
}