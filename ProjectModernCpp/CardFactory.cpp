#include "CardFactory.h"

std::vector<std::unique_ptr<Card>> CardFactory::createAgeIDeck()
{
	std::vector<std::unique_ptr<Card>> ageIDeck;
	ageIDeck.reserve(23);

	ageIDeck.push_back(std::make_unique<Card>(
		"Lumber Yard", CardColor::BROWN, 1,
		Cost(),
		CardEffect().withProduction(
			ResourceProduction().addFixedResource(ResourceType::WOOD, 1))
	));

	ageIDeck.push_back(std::make_unique<Card>(
		"Logging Camp", CardColor::BROWN, 1,
		Cost().withCoinCost(1),
		CardEffect().withProduction(
			ResourceProduction().addFixedResource(ResourceType::WOOD, 1))
	));

	ageIDeck.push_back(std::make_unique<Card>(
		"Clay Pool", CardColor::BROWN, 1,
		Cost(),
		CardEffect().withProduction(
			ResourceProduction().addFixedResource(ResourceType::CLAY, 1))
	));

	ageIDeck.push_back(std::make_unique<Card>(
		"Clay Pit", CardColor::BROWN, 1,
		Cost().withCoinCost(1),
		CardEffect().withProduction(
			ResourceProduction().addFixedResource(ResourceType::CLAY, 1))
	));

	ageIDeck.push_back(std::make_unique<Card>(
		"Querry", CardColor::BROWN, 1,
		Cost(),
		CardEffect().withProduction(
			ResourceProduction().addFixedResource(ResourceType::STONE, 1))
	));

	ageIDeck.push_back(std::make_unique<Card>(
		"Stone Pit", CardColor::BROWN, 1,
		Cost().withCoinCost(1),
		CardEffect().withProduction(
			ResourceProduction().addFixedResource(ResourceType::STONE, 1))
	));

	ageIDeck.push_back(std::make_unique<Card>(
		"Glassworks", CardColor::GREY, 1,
		Cost().withCoinCost(1),
		CardEffect().withProduction(
			ResourceProduction().addFixedResource(ResourceType::GLASS, 1))
	));

	ageIDeck.push_back(std::make_unique<Card>(
		"Press", CardColor::GREY, 1,
		Cost().withCoinCost(1),
		CardEffect().withProduction(
			ResourceProduction().addFixedResource(ResourceType::PAPYRUS, 1))
	));

	ageIDeck.push_back(std::make_unique<Card>(
		"Guard Tower", CardColor::RED, 1,
		Cost(),
		CardEffect().withShields(1)
	));

	ageIDeck.push_back(std::make_unique<Card>(
		"Workshop", CardColor::GREEN, 1,
		Cost().withResourceCost(ResourceType::PAPYRUS, 1),
		CardEffect().withScienceSymbol(ScientificSymbol::DRAFTING_COMPASS)
					.withVictoryPoints(1)
	));

	ageIDeck.push_back(std::make_unique<Card>(
		"Apothecary", CardColor::GREEN, 1,
		Cost().withResourceCost(ResourceType::GLASS, 1),
		CardEffect().withScienceSymbol(ScientificSymbol::WHEEL)
					.withVictoryPoints(1)
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


std::vector<std::unique_ptr<Card>> CardFactory::createAgeIIDeck()
{
	std::vector<std::unique_ptr<Card>> ageIIDeck;
	ageIIDeck.reserve(23);
	
	ageIIDeck.push_back(std::make_unique<Card>(
		"Sawmill", CardColor::BROWN, 2,
		Cost().withCoinCost(2),
		CardEffect().withProduction(
			ResourceProduction().addFixedResource(ResourceType::WOOD, 2))
	));

	ageIIDeck.push_back(std::make_unique<Card>(
		"Brickyard", CardColor::BROWN, 2,
		Cost().withCoinCost(2),
		CardEffect().withProduction(
			ResourceProduction().addFixedResource(ResourceType::CLAY, 2))
	));

	ageIIDeck.push_back(std::make_unique<Card>(
		"Shelf Quarry", CardColor::BROWN, 2,
		Cost().withCoinCost(2),
		CardEffect().withProduction(
			ResourceProduction().addFixedResource(ResourceType::STONE, 2))
	));

	ageIIDeck.push_back(std::make_unique<Card>(
		"Glass Blower", CardColor::GREY, 2,
		Cost(),
		CardEffect().withProduction(
			ResourceProduction().addFixedResource(ResourceType::GLASS, 1))
	));

	ageIIDeck.push_back(std::make_unique<Card>(
		"Drying Room", CardColor::GREY, 2,
		Cost(),
		CardEffect().withProduction(
			ResourceProduction().addFixedResource(ResourceType::PAPYRUS, 1))
	));

	ageIIDeck.push_back(std::make_unique<Card>(
		"Walls", CardColor::RED, 2,
		Cost().withResourceCost(ResourceType::STONE, 2),
		CardEffect().withShields(2)
	));

	ageIIDeck.push_back(std::make_unique<Card>(
		"Forum", CardColor::YELLOW, 2,
		Cost().withCoinCost(3)
			  .withResourceCost(ResourceType::CLAY, 1),
		CardEffect().withProduction(
			ResourceProduction().addChoice({ResourceType::GLASS, ResourceType::PAPYRUS}))
	));

	ageIIDeck.push_back(std::make_unique<Card>(
		"Caravansery", CardColor::YELLOW, 2,
		Cost().withCoinCost(2)
			  .withResourceCost(ResourceType::GLASS, 1)
		      .withResourceCost(ResourceType::PAPYRUS, 1),
		CardEffect().withProduction(
			ResourceProduction().addChoice({ ResourceType::WOOD, 
											 ResourceType::CLAY,
											 ResourceType::STONE }))
	));

	ageIIDeck.push_back(std::make_unique<Card>(
		"Customs House", CardColor::YELLOW, 2,
		Cost().withCoinCost(4),
		CardEffect().withDiscount(ResourceType::PAPYRUS, 1)
					.withDiscount(ResourceType::GLASS, 1)
	));

	ageIIDeck.push_back(std::make_unique<Card>(
		"Tribunal", CardColor::BLUE, 2,
		Cost().withResourceCost(ResourceType::WOOD, 2)
			  .withResourceCost(ResourceType::GLASS, 1),
		CardEffect().withVictoryPoints(5)
	));

	ageIIDeck.push_back(std::make_unique<Card>(
		"Horse Breeders", CardColor::RED, 2,
		Cost().withResourceCost(ResourceType::CLAY, 1)
			  .withResourceCost(ResourceType::WOOD, 1),
		CardEffect().withShields(1),
		"Stable",
		std::nullopt
	));

	ageIIDeck.push_back(std::make_unique<Card>(
		"Barracks", CardColor::RED, 2,
		Cost().withCoinCost(3),
		CardEffect().withShields(1),
		"Garrison",
		std::nullopt
	));

	ageIIDeck.push_back(std::make_unique<Card>(
		"Archery Range", CardColor::RED, 2,
		Cost().withResourceCost(ResourceType::STONE, 1)
			  .withResourceCost(ResourceType::WOOD, 1)
			  .withResourceCost(ResourceType::PAPYRUS, 1),
		CardEffect().withShields(2),
		std::nullopt,
		"Siege Workshop"
	));

	ageIIDeck.push_back(std::make_unique<Card>(
		"Parade Ground", CardColor::RED, 2,
		Cost().withResourceCost(ResourceType::CLAY, 2)
			  .withResourceCost(ResourceType::GLASS, 1),
		CardEffect().withShields(2),
		std::nullopt,
		"Circus"
	));

	ageIIDeck.push_back(std::make_unique<Card>(
		"Library", CardColor::GREEN, 2,
		Cost().withResourceCost(ResourceType::STONE, 1)
			  .withResourceCost(ResourceType::WOOD, 1)
			  .withResourceCost(ResourceType::GLASS, 1),
		CardEffect().withScienceSymbol(ScientificSymbol::QUILL_INKWELL)
					.withVictoryPoints(2),
		"Scriptorium",
		std::nullopt
	));

	ageIIDeck.push_back(std::make_unique<Card>(
		"Dispensary", CardColor::GREEN, 2,
		Cost().withResourceCost(ResourceType::CLAY, 2)
			  .withResourceCost(ResourceType::STONE, 1),
		CardEffect().withScienceSymbol(ScientificSymbol::MORTAR_PESTLE)
					.withVictoryPoints(2),
		"Pharmacist",
		std::nullopt
	));

	ageIIDeck.push_back(std::make_unique<Card>(
		"School", CardColor::GREEN, 2,
		Cost().withResourceCost(ResourceType::WOOD, 1)
			  .withResourceCost(ResourceType::PAPYRUS, 2),
		CardEffect().withScienceSymbol(ScientificSymbol::WHEEL)
					.withVictoryPoints(1),
		std::nullopt,
		"University"
	));

	ageIIDeck.push_back(std::make_unique<Card>(
		"Laboratory", CardColor::GREEN, 2,
		Cost().withResourceCost(ResourceType::WOOD, 1)
			  .withResourceCost(ResourceType::GLASS, 2),
		CardEffect().withScienceSymbol(ScientificSymbol::DRAFTING_COMPASS)
					.withVictoryPoints(1),
		std::nullopt,
		"Observatory"
	));

	ageIIDeck.push_back(std::make_unique<Card>(
		"Statue", CardColor::BLUE, 2,
		Cost().withResourceCost(ResourceType::CLAY, 2),
		CardEffect().withVictoryPoints(4),
		"Theater",
		"Gardens"
	));

	ageIIDeck.push_back(std::make_unique<Card>(
		"Temple", CardColor::BLUE, 2,
		Cost().withResourceCost(ResourceType::WOOD, 1)
			  .withResourceCost(ResourceType::PAPYRUS, 1),
		CardEffect().withVictoryPoints(4),
		"Altar",
		"Pantheon"
	));

	ageIIDeck.push_back(std::make_unique<Card>(
		"Aqueduct", CardColor::BLUE, 2,
		Cost().withResourceCost(ResourceType::STONE, 3),
		CardEffect().withVictoryPoints(5),
		"Baths",
		std::nullopt
	));

	ageIIDeck.push_back(std::make_unique<Card>(
		"Rostrum", CardColor::BLUE, 2,
		Cost().withResourceCost(ResourceType::STONE, 1)
			  .withResourceCost(ResourceType::WOOD, 1),
		CardEffect().withVictoryPoints(4),
		std::nullopt,
		"Senate"
	));

	ageIIDeck.push_back(std::make_unique<Card>(
		"Brewery", CardColor::YELLOW, 2,
		Cost(),
		CardEffect().withBaseCoins(6),
		std::nullopt,
		"Arena"
	));


	return ageIIDeck;
}
