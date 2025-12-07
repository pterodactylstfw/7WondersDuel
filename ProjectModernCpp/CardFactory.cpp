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

std::vector<std::unique_ptr<Card>> CardFactory::createAgeIIIDeck()
{
	std::vector<std::unique_ptr<Card>> ageIIIDeck;
	ageIIIDeck.reserve(20);

	ageIIIDeck.push_back(std::make_unique<Card>(
		"Arsenal", CardColor::RED, 3,
		Cost().withResourceCost(ResourceType::CLAY, 3)
			  .withResourceCost(ResourceType::WOOD, 2),
		CardEffect().withShields(3)
	));

	ageIIIDeck.push_back(std::make_unique<Card>(
		"Courthouse", CardColor::RED, 3,
		Cost().withCoinCost(8),
		CardEffect().withShields(3)
	));

	ageIIIDeck.push_back(std::make_unique<Card>(
		"Academy", CardColor::GREEN, 3,
		Cost().withResourceCost(ResourceType::STONE, 1)
			  .withResourceCost(ResourceType::WOOD, 1)
			  .withResourceCost(ResourceType::GLASS, 2),
		CardEffect().withScienceSymbol(ScientificSymbol::SUNDIAL)
					.withVictoryPoints(3)
	));

	ageIIIDeck.push_back(std::make_unique<Card>(
		"Study", CardColor::GREEN, 3,
		Cost().withResourceCost(ResourceType::WOOD, 2)
			  .withResourceCost(ResourceType::GLASS, 1)
			  .withResourceCost(ResourceType::PAPYRUS, 1),
		CardEffect().withScienceSymbol(ScientificSymbol::SUNDIAL)
					.withVictoryPoints(3)
	));

	ageIIIDeck.push_back(std::make_unique<Card>(
		"Chamber of Commerce", CardColor::YELLOW, 3,
		Cost().withResourceCost(ResourceType::PAPYRUS, 2),
		CardEffect().withCoinsPerCardType(CardColor::GREY, 3)
					.withVictoryPoints(3)
	));

	ageIIIDeck.push_back(std::make_unique<Card>(
		"Port", CardColor::YELLOW, 3,
		Cost().withResourceCost(ResourceType::WOOD, 1)
			  .withResourceCost(ResourceType::GLASS, 1)
			  .withResourceCost(ResourceType::PAPYRUS, 1),
		CardEffect().withCoinsPerCardType(CardColor::BROWN, 2)
					.withVictoryPoints(3)
	));

	ageIIIDeck.push_back(std::make_unique<Card>(
		"Armory", CardColor::YELLOW, 3,
		Cost().withResourceCost(ResourceType::STONE, 2)
			  .withResourceCost(ResourceType::GLASS, 1),
		CardEffect().withCoinsPerCardType(CardColor::RED, 1)
					.withVictoryPoints(3)
	));

	ageIIIDeck.push_back(std::make_unique<Card>(
		"Palace", CardColor::BLUE, 3,
		Cost().withResourceCost(ResourceType::CLAY, 1)
			  .withResourceCost(ResourceType::STONE, 1)
			  .withResourceCost(ResourceType::WOOD, 1)
			  .withResourceCost(ResourceType::GLASS, 2),
		CardEffect().withVictoryPoints(7)
	));

	ageIIIDeck.push_back(std::make_unique<Card>(
		"Town Hall", CardColor::BLUE, 3,
		Cost().withResourceCost(ResourceType::STONE, 3)
			  .withResourceCost(ResourceType::WOOD, 2),
		CardEffect().withVictoryPoints(7)
	));

	ageIIIDeck.push_back(std::make_unique<Card>(
		"Obelisk", CardColor::BLUE, 3,
		Cost().withResourceCost(ResourceType::STONE, 2)
			  .withResourceCost(ResourceType::GLASS, 1),
		CardEffect().withVictoryPoints(5)
	));

	ageIIIDeck.push_back(std::make_unique<Card>(
		"Fortifications", CardColor::RED, 3,
		Cost().withResourceCost(ResourceType::STONE, 2)
			  .withResourceCost(ResourceType::CLAY, 1)
			  .withResourceCost(ResourceType::PAPYRUS, 1),
		CardEffect().withShields(2),
		"Palisade",
		std::nullopt
	));

	ageIIIDeck.push_back(std::make_unique<Card>(
		"Siege Workshop", CardColor::RED, 3,
		Cost().withResourceCost(ResourceType::WOOD, 3)
			  .withResourceCost(ResourceType::GLASS, 1),
		CardEffect().withShields(2),
		"Archery Range",
		std::nullopt
	));

	ageIIIDeck.push_back(std::make_unique<Card>(
		"Circus", CardColor::RED, 3,
		Cost().withResourceCost(ResourceType::CLAY, 2)
			  .withResourceCost(ResourceType::STONE, 2),
		CardEffect().withShields(2),
		"Parade Ground",
		std::nullopt
	));

	ageIIIDeck.push_back(std::make_unique<Card>(
		"University", CardColor::GREEN, 3,
		Cost().withResourceCost(ResourceType::CLAY, 1)
			  .withResourceCost(ResourceType::GLASS, 1)
			  .withResourceCost(ResourceType::PAPYRUS, 1),
		CardEffect().withScienceSymbol(ScientificSymbol::ASTROLABE)
					.withVictoryPoints(2),
		"School",
		std::nullopt
	));

	ageIIIDeck.push_back(std::make_unique<Card>(
		"Observatory", CardColor::GREEN, 3,
		Cost().withResourceCost(ResourceType::STONE, 1)
			  .withResourceCost(ResourceType::PAPYRUS, 2),
		CardEffect().withScienceSymbol(ScientificSymbol::ASTROLABE)
					.withVictoryPoints(2),
		"Laboratory",
		std::nullopt
	));

	ageIIIDeck.push_back(std::make_unique<Card>(
		"Gardens", CardColor::BLUE, 3,
		Cost().withResourceCost(ResourceType::CLAY, 2)
			  .withResourceCost(ResourceType::WOOD, 2),
		CardEffect().withVictoryPoints(6),
		"Statue",
		std::nullopt
	));

	ageIIIDeck.push_back(std::make_unique<Card>(
		"Pantheon", CardColor::BLUE, 3,
		Cost().withResourceCost(ResourceType::CLAY, 1)
			  .withResourceCost(ResourceType::WOOD, 1)
			  .withResourceCost(ResourceType::PAPYRUS, 2),
		CardEffect().withVictoryPoints(6),
		"Temple",
		std::nullopt
	));

	ageIIIDeck.push_back(std::make_unique<Card>(
		"Senate", CardColor::BLUE, 3,
		Cost().withResourceCost(ResourceType::CLAY, 2)
			  .withResourceCost(ResourceType::STONE, 1)
			  .withResourceCost(ResourceType::PAPYRUS, 1),
		CardEffect().withVictoryPoints(5),
		"Rostrum",
		std::nullopt
	));

	ageIIIDeck.push_back(std::make_unique<Card>(
		"Lighthouse", CardColor::YELLOW, 3,
		Cost().withResourceCost(ResourceType::CLAY, 2)
			  .withResourceCost(ResourceType::GLASS, 1),
		CardEffect().withCoinsPerCardType(CardColor::YELLOW, 1)
					.withVictoryPoints(3),
		"Tavern",
		std::nullopt
	));

	ageIIIDeck.push_back(std::make_unique<Card>(
		"Arena", CardColor::YELLOW, 3,
		Cost().withResourceCost(ResourceType::CLAY, 1)
			  .withResourceCost(ResourceType::STONE, 1)
			  .withResourceCost(ResourceType::WOOD, 1),
		CardEffect().withCoinsPerWonder(2)
					.withVictoryPoints(3),
		"Brewery",
		std::nullopt
	));

	return ageIIIDeck;
}

std::vector<std::unique_ptr<Card>> CardFactory::createGuildDeck()
{
	std::vector<std::unique_ptr<Card>> guildDeck;
	guildDeck.reserve(7);

	guildDeck.push_back(std::make_unique<Card>(
		"Merchants Guild", CardColor::PURPLE, 3,
		Cost().withResourceCost(ResourceType::CLAY, 1)
			  .withResourceCost(ResourceType::WOOD, 1)
			  .withResourceCost(ResourceType::GLASS, 1)
			  .withResourceCost(ResourceType::PAPYRUS, 1),
		CardEffect().withCoinsPerCardType(CardColor::YELLOW, 1)
					.withPointsPerCardType(CardColor::YELLOW, 1)
					.withCustomDescription("At the time it is constructed, the card grants you 1 coin for each yellow card in the city which has the most yellow cards.\n"
						"At the end of the game, this card is worth 1 victory point for each yellow card in the city which has the most yellow cards.\n")
	));

	guildDeck.push_back(std::make_unique<Card>(
		"Shipowners Guild", CardColor::PURPLE, 3,
		Cost().withResourceCost(ResourceType::CLAY, 1)
			  .withResourceCost(ResourceType::STONE, 1)
			  .withResourceCost(ResourceType::GLASS, 1)
			  .withResourceCost(ResourceType::PAPYRUS, 1),
		CardEffect().withCoinsPerCardType(CardColor::BROWN, 1)
					.withCoinsPerCardType(CardColor::GREY, 1)
					.withPointsPerCardType(CardColor::BROWN, 1)
					.withPointsPerCardType(CardColor::GREY, 1)
					.withCustomDescription("At the time it is constructed, this card grants you 1 coin for each brown and each grey card in the city which has the most brown and grey cards.\n"
							"At the end of the game, this card is worth 1 victory point for each brown and grey card in the city which has the most brown and grey cards.\n")
	));

	guildDeck.push_back(std::make_unique<Card>(
		"Builders Guild", CardColor::PURPLE, 3,
		Cost().withResourceCost(ResourceType::STONE, 2)
			  .withResourceCost(ResourceType::CLAY, 1)
			  .withResourceCost(ResourceType::WOOD, 1)
			  .withResourceCost(ResourceType::GLASS, 1),
		CardEffect().withPointsPerWonder(2)
					.withCustomDescription("At the end of the game, this card is worth 2 victory points for each Wonder constructed in the city which has the most wonders.\n")
	));

	guildDeck.push_back(std::make_unique<Card>(
		"Magistrates Guild", CardColor::PURPLE, 3,
		Cost().withResourceCost(ResourceType::WOOD, 2)
			  .withResourceCost(ResourceType::CLAY, 1)
			  .withResourceCost(ResourceType::PAPYRUS, 1),
		CardEffect().withCoinsPerCardType(CardColor::BLUE, 1)
					.withPointsPerCardType(CardColor::BLUE, 1)
					.withCustomDescription("At the time it is constructed, this card grants you 1 coin for each blue card in the city which has the most blue cards.\n"
						"At the end of the game, this card is worth 1 victory point for each blue card in the city which has the most blue cards.\n")
	));

	guildDeck.push_back(std::make_unique<Card>(
		"Scientists Guild", CardColor::PURPLE, 3,
		Cost().withResourceCost(ResourceType::CLAY, 2)
			  .withResourceCost(ResourceType::WOOD, 2),
		CardEffect().withCoinsPerCardType(CardColor::GREEN, 1)
					.withPointsPerCardType(CardColor::GREEN, 1)
					.withCustomDescription("At the time it is constructed, this card earns you 1 coin for each green card in the city which has the most green cards.\n"
						"At the end of the game, this card is worth 1 victory point for each green card in the city which has the most green cards.\n")
	));

	guildDeck.push_back(std::make_unique<Card>(
		"Moneylenders Guild", CardColor::PURPLE, 3,
		Cost().withResourceCost(ResourceType::STONE, 2)
			  .withResourceCost(ResourceType::WOOD, 2),
		CardEffect().withPointPerCoins(1, 3)
					.withCustomDescription("At the end of the game, this card is worth 1 victory point for each set of 3 coins in the richest city.\n")
	));

	guildDeck.push_back(std::make_unique<Card>(
		"Tacticians Guild", CardColor::PURPLE, 3,
		Cost().withResourceCost(ResourceType::STONE, 2)
			  .withResourceCost(ResourceType::CLAY, 1)
			  .withResourceCost(ResourceType::PAPYRUS, 1),
		CardEffect().withCoinsPerCardType(CardColor::RED, 1)
					.withPointsPerCardType(CardColor::RED, 1)
					.withCustomDescription("At the time it is constructed, this card grants you 1 coin for each red card in the city which has the most red cards.\n"
						"At the end of the game, this card is worth 1 victory point for each red card in the city which has the most red cards.\n")
	));

	return guildDeck;
}

