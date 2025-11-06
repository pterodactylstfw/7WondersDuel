#include "CardFactory.h"

std::vector<std::unique_ptr<Card>> CardFactory::createAgeIDeck()
{
	std::vector<std::unique_ptr<Card>> ageIDeck;
	ageIDeck.reserve(23);

	ageIDeck.push_back(std::make_unique<Card>(
		"Lumber Yard", CardColor::BROWN, 1,
		Cost(),
		CardEffect().withProduction(ResourceProduction().addFixedResource(ResourceType::WOOD, 1))
	));

	ageIDeck.push_back(std::make_unique<Card>(
		"Logging Camp", CardColor::BROWN, 1,
		Cost().withCoinCost(1),
		CardEffect().withProduction(ResourceProduction().addFixedResource(ResourceType::WOOD, 1))
	));

	ageIDeck.push_back(std::make_unique<Card>(
		"Clay Pool", CardColor::BROWN, 1,
		Cost(),
		CardEffect().withProduction(ResourceProduction().addFixedResource(ResourceType::CLAY, 1))
	));

	ageIDeck.push_back(std::make_unique<Card>(
		"Clay Pit", CardColor::BROWN, 1,
		Cost().withCoinCost(1),
		CardEffect().withProduction(ResourceProduction().addFixedResource(ResourceType::CLAY, 1))
	));

	ageIDeck.push_back(std::make_unique<Card>(
		"Querry", CardColor::BROWN, 1,
		Cost(),
		CardEffect().withProduction(ResourceProduction().addFixedResource(ResourceType::STONE, 1))
	));

	ageIDeck.push_back(std::make_unique<Card>(
		"Stone Pit", CardColor::BROWN, 1,
		Cost().withCoinCost(1),
		CardEffect().withProduction(ResourceProduction().addFixedResource(ResourceType::STONE, 1))
	));

	ageIDeck.push_back(std::make_unique<Card>(
		"Glassworks", CardColor::GREY, 1,
		Cost().withCoinCost(1),
		CardEffect().withProduction(ResourceProduction().addFixedResource(ResourceType::GLASS, 1))
	));

	ageIDeck.push_back(std::make_unique<Card>(
		"Press", CardColor::GREY, 1,
		Cost().withCoinCost(1),
		CardEffect().withProduction(ResourceProduction().addFixedResource(ResourceType::PAPYRUS, 1))
	));

	ageIDeck.push_back(std::make_unique<Card>(
		"Guard Tower", CardColor::RED, 1,
		Cost(),
		CardEffect().withShields(1)
	));

	ageIDeck.push_back(std::make_unique<Card>(
		"Workshop", CardColor::GREEN, 1,
		Cost().withResourceCost(ResourceType::PAPYRUS, 1),
		CardEffect().withScienceSymbol(ScientificSymbol::DRAFTING_COMPASS).withVictoryPoints(1)
	));

	ageIDeck.push_back(std::make_unique<Card>(
		"Apothecary", CardColor::GREEN, 1,
		Cost().withResourceCost(ResourceType::GLASS, 1),
		CardEffect().withScienceSymbol(ScientificSymbol::WHEEL).withVictoryPoints(1)
	));

	ageIDeck.push_back(std::make_unique<Card>(
		"Stone Reserve", CardColor::YELLOW, 1,
		Cost().withCoinCost(3),
		CardEffect().withDiscount(ResourceType::STONE, 1)
	));

	ageIDeck.push_back(std::make_unique<Card>(
		"Clay Reserve", CardColor::YELLOW, 1,
		Cost().withCoinCost(3),
		CardEffect().withDiscount(ResourceType::CLAY, 1)
	));

	ageIDeck.push_back(std::make_unique<Card>(
		"Wood Reserve", CardColor::YELLOW, 1,
		Cost().withCoinCost(3),
		CardEffect().withDiscount(ResourceType::WOOD, 1)
	));

	ageIDeck.push_back(std::make_unique<Card>(
		"Stable", CardColor::RED, 1,
		Cost().withResourceCost(ResourceType::WOOD, 1),
		CardEffect().withShields(1),
		std::nullopt,
		"Horse Breeders"
	));

	ageIDeck.push_back(std::make_unique<Card>(
		"Garrison", CardColor::RED, 1,
		Cost().withResourceCost(ResourceType::CLAY, 1),
		CardEffect().withShields(1),
		std::nullopt,
		"Barracks"
	));

	ageIDeck.push_back(std::make_unique<Card>(
		"Palisade", CardColor::RED, 1,
		Cost().withCoinCost(2),
		CardEffect().withShields(1),
		std::nullopt,
		"Fortifications"
	));

	ageIDeck.push_back(std::make_unique<Card>(
		"Scriptorium", CardColor::GREEN, 1,
		Cost().withCoinCost(2),
		CardEffect().withScienceSymbol(ScientificSymbol::QUILL_INKWELL),
		std::nullopt,
		"Library"
	));

	ageIDeck.push_back(std::make_unique<Card>(
		"Pharmacist", CardColor::GREEN, 1,
		Cost().withCoinCost(2),
		CardEffect().withScienceSymbol(ScientificSymbol::MORTAR_PESTLE),
		std::nullopt,
		"Dispensary"
	));

	ageIDeck.push_back(std::make_unique<Card>(
		"Theater", CardColor::BLUE, 1,
		Cost(),
		CardEffect().withVictoryPoints(3),
		std::nullopt,
		"Statue"
	));

	ageIDeck.push_back(std::make_unique<Card>(
		"Altar", CardColor::BLUE, 1,
		Cost(),
		CardEffect().withVictoryPoints(3),
		std::nullopt,
		"Temple"
	));

	ageIDeck.push_back(std::make_unique<Card>(
		"Baths", CardColor::BLUE, 1,
		Cost().withResourceCost(ResourceType::STONE, 1),
		CardEffect().withVictoryPoints(3),
		std::nullopt,
		"Aqueduct"
	));

	ageIDeck.push_back(std::make_unique<Card>(
		"Tavern", CardColor::YELLOW, 1,
		Cost(),
		CardEffect().withBaseCoins(4),
		std::nullopt,
		"Lighthouse"
	));

	return ageIDeck;
}
