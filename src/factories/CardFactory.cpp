#include "CardFactory.h"

std::vector<std::unique_ptr<Card>> CardFactory::createAgeIDeck()
{
	std::vector<std::unique_ptr<Card>> ageIDeck;
	ageIDeck.reserve(23);

	ageIDeck.push_back(std::make_unique<Card>(
		"Lumber Yard", CardColor::BROWN, 1,
		Cost(),
		CardEffect().withProduction(
			ResourceProduction().addFixedResource(ResourceType::WOOD, 1)),
		"lumber_yard.png"
	));

	ageIDeck.push_back(std::make_unique<Card>(
		"Logging Camp", CardColor::BROWN, 1,
		Cost().withCoinCost(1),
		CardEffect().withProduction(
			ResourceProduction().addFixedResource(ResourceType::WOOD, 1)),
		"logging_camp.png"
	));

	ageIDeck.push_back(std::make_unique<Card>(
		"Clay Pool", CardColor::BROWN, 1,
		Cost(),
		CardEffect().withProduction(
			ResourceProduction().addFixedResource(ResourceType::CLAY, 1)),
		"clay_pool.png"
	));

	ageIDeck.push_back(std::make_unique<Card>(
		"Clay Pit", CardColor::BROWN, 1,
		Cost().withCoinCost(1),
		CardEffect().withProduction(
			ResourceProduction().addFixedResource(ResourceType::CLAY, 1)),
		"clay_pit.png"
	));

	ageIDeck.push_back(std::make_unique<Card>(
		"Quarry", CardColor::BROWN, 1,
		Cost(),
		CardEffect().withProduction(
			ResourceProduction().addFixedResource(ResourceType::STONE, 1)),
		"quarry.png"
	));

	ageIDeck.push_back(std::make_unique<Card>(
		"Stone Pit", CardColor::BROWN, 1,
		Cost().withCoinCost(1),
		CardEffect().withProduction(
			ResourceProduction().addFixedResource(ResourceType::STONE, 1)),
		"stone_pit.png"
	));

	ageIDeck.push_back(std::make_unique<Card>(
		"Glassworks", CardColor::GREY, 1,
		Cost().withCoinCost(1),
		CardEffect().withProduction(
			ResourceProduction().addFixedResource(ResourceType::GLASS, 1)),
		"glassworks.png"
	));

	ageIDeck.push_back(std::make_unique<Card>(
		"Press", CardColor::GREY, 1,
		Cost().withCoinCost(1),
		CardEffect().withProduction(
			ResourceProduction().addFixedResource(ResourceType::PAPYRUS, 1)),
		"press.png"
	));

	ageIDeck.push_back(std::make_unique<Card>(
		"Guard Tower", CardColor::RED, 1,
		Cost(),
		CardEffect().withShields(1),
		"guard_tower.png"
	));

	ageIDeck.push_back(std::make_unique<Card>(
		"Workshop", CardColor::GREEN, 1,
		Cost().withResourceCost(ResourceType::PAPYRUS, 1),
		CardEffect().withScienceSymbol(ScientificSymbol::DRAFTING_COMPASS)
					.withVictoryPoints(1),
		"workshop.png"
	));

	ageIDeck.push_back(std::make_unique<Card>(
		"Apothecary", CardColor::GREEN, 1,
		Cost().withResourceCost(ResourceType::GLASS, 1),
		CardEffect().withScienceSymbol(ScientificSymbol::WHEEL)
					.withVictoryPoints(1),
		"apothecary.png"
	));

	ageIDeck.push_back(std::make_unique<Card>(
		"Stone Reserve", CardColor::YELLOW, 1,
		Cost().withCoinCost(3),
		CardEffect().withDiscount(ResourceType::STONE, 1),
		"stone_reserve.png"
	));

	ageIDeck.push_back(std::make_unique<Card>(
		"Clay Reserve", CardColor::YELLOW, 1,
		Cost().withCoinCost(3),
		CardEffect().withDiscount(ResourceType::CLAY, 1),
		"clay_reserve.png"
	));

	ageIDeck.push_back(std::make_unique<Card>(
		"Wood Reserve", CardColor::YELLOW, 1,
		Cost().withCoinCost(3),
		CardEffect().withDiscount(ResourceType::WOOD, 1),
		"wood_reserve.png"
	));

	ageIDeck.push_back(std::make_unique<Card>(
		"Stable", CardColor::RED, 1,
		Cost().withResourceCost(ResourceType::WOOD, 1),
		CardEffect().withShields(1),
		"stable.png",
		std::nullopt,
		"Horse Breeders"
	));

	ageIDeck.push_back(std::make_unique<Card>(
		"Garrison", CardColor::RED, 1,
		Cost().withResourceCost(ResourceType::CLAY, 1),
		CardEffect().withShields(1),
		"garrison.png",
		std::nullopt,
		"Barracks"
	));

	ageIDeck.push_back(std::make_unique<Card>(
		"Palisade", CardColor::RED, 1,
		Cost().withCoinCost(2),
		CardEffect().withShields(1),
		"palisade.png",
		std::nullopt,
		"Fortifications"
	));

	ageIDeck.push_back(std::make_unique<Card>(
		"Scriptorium", CardColor::GREEN, 1,
		Cost().withCoinCost(2),
		CardEffect().withScienceSymbol(ScientificSymbol::QUILL_INKWELL),
		"scriptorium.png",
		std::nullopt,
		"Library"
	));

	ageIDeck.push_back(std::make_unique<Card>(
		"Pharmacist", CardColor::GREEN, 1,
		Cost().withCoinCost(2),
		CardEffect().withScienceSymbol(ScientificSymbol::MORTAR_PESTLE),
		"pharmacist.png",
		std::nullopt,
		"Dispensary"
	));

	ageIDeck.push_back(std::make_unique<Card>(
		"Theater", CardColor::BLUE, 1,
		Cost(),
		CardEffect().withVictoryPoints(3),
		"theater.png",
		std::nullopt,
		"Statue"
	));

	ageIDeck.push_back(std::make_unique<Card>(
		"Altar", CardColor::BLUE, 1,
		Cost(),
		CardEffect().withVictoryPoints(3),
		"altar.png",
		std::nullopt,
		"Temple"
	));

	ageIDeck.push_back(std::make_unique<Card>(
		"Baths", CardColor::BLUE, 1,
		Cost().withResourceCost(ResourceType::STONE, 1),
		CardEffect().withVictoryPoints(3),
		"baths.png",
		std::nullopt,
		"Aqueduct"
	));

	ageIDeck.push_back(std::make_unique<Card>(
		"Tavern", CardColor::YELLOW, 1,
		Cost(),
		CardEffect().withBaseCoins(4),
		"tavern.png",
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
			ResourceProduction().addFixedResource(ResourceType::WOOD, 2)),
		"sawmill.png"
	));

	ageIIDeck.push_back(std::make_unique<Card>(
		"Brickyard", CardColor::BROWN, 2,
		Cost().withCoinCost(2),
		CardEffect().withProduction(
			ResourceProduction().addFixedResource(ResourceType::CLAY, 2)),
		"brickyard.png"
	));

	ageIIDeck.push_back(std::make_unique<Card>(
		"Shelf Quarry", CardColor::BROWN, 2,
		Cost().withCoinCost(2),
		CardEffect().withProduction(
			ResourceProduction().addFixedResource(ResourceType::STONE, 2)),
		"shelf_quarry"
	));

	ageIIDeck.push_back(std::make_unique<Card>(
		"Glass Blower", CardColor::GREY, 2,
		Cost(),
		CardEffect().withProduction(
			ResourceProduction().addFixedResource(ResourceType::GLASS, 1)),
		"glass_blower.png"
	));

	ageIIDeck.push_back(std::make_unique<Card>(
		"Drying Room", CardColor::GREY, 2,
		Cost(),
		CardEffect().withProduction(
			ResourceProduction().addFixedResource(ResourceType::PAPYRUS, 1)),
		"drying_room.png"
	));

	ageIIDeck.push_back(std::make_unique<Card>(
		"Walls", CardColor::RED, 2,
		Cost().withResourceCost(ResourceType::STONE, 2),
		CardEffect().withShields(2),
		"walls.png"
	));

	ageIIDeck.push_back(std::make_unique<Card>(
		"Forum", CardColor::YELLOW, 2,
		Cost().withCoinCost(3)
			  .withResourceCost(ResourceType::CLAY, 1),
		CardEffect().withProduction(
			ResourceProduction().addChoice({ResourceType::GLASS, ResourceType::PAPYRUS})),
		"forum.png"
	));

	ageIIDeck.push_back(std::make_unique<Card>(
		"Caravansery", CardColor::YELLOW, 2,
		Cost().withCoinCost(2)
			  .withResourceCost(ResourceType::GLASS, 1)
		      .withResourceCost(ResourceType::PAPYRUS, 1),
		CardEffect().withProduction(
			ResourceProduction().addChoice({ ResourceType::WOOD, 
											 ResourceType::CLAY,
											 ResourceType::STONE })),
		"caravansery.png"
	));

	ageIIDeck.push_back(std::make_unique<Card>(
		"Customs House", CardColor::YELLOW, 2,
		Cost().withCoinCost(4),
		CardEffect().withDiscount(ResourceType::PAPYRUS, 1)
					.withDiscount(ResourceType::GLASS, 1),
		"customs_house.png"
	));

	ageIIDeck.push_back(std::make_unique<Card>(
		"Tribunal", CardColor::BLUE, 2,
		Cost().withResourceCost(ResourceType::WOOD, 2)
			  .withResourceCost(ResourceType::GLASS, 1),
		CardEffect().withVictoryPoints(5),
		"tribunal.png"
	));

	ageIIDeck.push_back(std::make_unique<Card>(
		"Horse Breeders", CardColor::RED, 2,
		Cost().withResourceCost(ResourceType::CLAY, 1)
			  .withResourceCost(ResourceType::WOOD, 1),
		CardEffect().withShields(1),
		"horse_breeders.png",
		"Stable",
		std::nullopt
	));

	ageIIDeck.push_back(std::make_unique<Card>(
		"Barracks", CardColor::RED, 2,
		Cost().withCoinCost(3),
		CardEffect().withShields(1),
		"barracks.png",
		"Garrison",
		std::nullopt
	));

	ageIIDeck.push_back(std::make_unique<Card>(
		"Archery Range", CardColor::RED, 2,
		Cost().withResourceCost(ResourceType::STONE, 1)
			  .withResourceCost(ResourceType::WOOD, 1)
			  .withResourceCost(ResourceType::PAPYRUS, 1),
		CardEffect().withShields(2),
		"archery_range.png",
		std::nullopt,
		"Siege Workshop"
	));

	ageIIDeck.push_back(std::make_unique<Card>(
		"Parade Ground", CardColor::RED, 2,
		Cost().withResourceCost(ResourceType::CLAY, 2)
			  .withResourceCost(ResourceType::GLASS, 1),
		CardEffect().withShields(2),
		"parade_ground.png",
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
		"library.png",
		"Scriptorium",
		std::nullopt
	));

	ageIIDeck.push_back(std::make_unique<Card>(
		"Dispensary", CardColor::GREEN, 2,
		Cost().withResourceCost(ResourceType::CLAY, 2)
			  .withResourceCost(ResourceType::STONE, 1),
		CardEffect().withScienceSymbol(ScientificSymbol::MORTAR_PESTLE)
					.withVictoryPoints(2),
		"dispensary.png",
		"Pharmacist",
		std::nullopt
	));

	ageIIDeck.push_back(std::make_unique<Card>(
		"School", CardColor::GREEN, 2,
		Cost().withResourceCost(ResourceType::WOOD, 1)
			  .withResourceCost(ResourceType::PAPYRUS, 2),
		CardEffect().withScienceSymbol(ScientificSymbol::WHEEL)
					.withVictoryPoints(1),
		"school.png",
		std::nullopt,
		"University"
	));

	ageIIDeck.push_back(std::make_unique<Card>(
		"Laboratory", CardColor::GREEN, 2,
		Cost().withResourceCost(ResourceType::WOOD, 1)
			  .withResourceCost(ResourceType::GLASS, 2),
		CardEffect().withScienceSymbol(ScientificSymbol::DRAFTING_COMPASS)
					.withVictoryPoints(1),
		"laboratory.png",
		std::nullopt,
		"Observatory"
	));

	ageIIDeck.push_back(std::make_unique<Card>(
		"Statue", CardColor::BLUE, 2,
		Cost().withResourceCost(ResourceType::CLAY, 2),
		CardEffect().withVictoryPoints(4),
		"statue.png",
		"Theater",
		"Gardens"
	));

	ageIIDeck.push_back(std::make_unique<Card>(
		"Temple", CardColor::BLUE, 2,
		Cost().withResourceCost(ResourceType::WOOD, 1)
			  .withResourceCost(ResourceType::PAPYRUS, 1),
		CardEffect().withVictoryPoints(4),
		"temple.png",
		"Altar",
		"Pantheon"
	));

	ageIIDeck.push_back(std::make_unique<Card>(
		"Aqueduct", CardColor::BLUE, 2,
		Cost().withResourceCost(ResourceType::STONE, 3),
		CardEffect().withVictoryPoints(5),
		"aqueduct.png",
		"Baths",
		std::nullopt
	));

	ageIIDeck.push_back(std::make_unique<Card>(
		"Rostrum", CardColor::BLUE, 2,
		Cost().withResourceCost(ResourceType::STONE, 1)
			  .withResourceCost(ResourceType::WOOD, 1),
		CardEffect().withVictoryPoints(4),
		"rostrum.png",
		std::nullopt,
		"Senate"
	));

	ageIIDeck.push_back(std::make_unique<Card>(
		"Brewery", CardColor::YELLOW, 2,
		Cost(),
		CardEffect().withBaseCoins(6),
		"brewery.png",
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
		CardEffect().withShields(3),
		"arsenal.png"
	));

	ageIIIDeck.push_back(std::make_unique<Card>(
		"Courthouse", CardColor::RED, 3,
		Cost().withCoinCost(8),
		CardEffect().withShields(3),
		"courthouse.png"
	));

	ageIIIDeck.push_back(std::make_unique<Card>(
		"Academy", CardColor::GREEN, 3,
		Cost().withResourceCost(ResourceType::STONE, 1)
			  .withResourceCost(ResourceType::WOOD, 1)
			  .withResourceCost(ResourceType::GLASS, 2),
		CardEffect().withScienceSymbol(ScientificSymbol::SUNDIAL)
					.withVictoryPoints(3),
		"academy.png"
	));

	ageIIIDeck.push_back(std::make_unique<Card>(
		"Study", CardColor::GREEN, 3,
		Cost().withResourceCost(ResourceType::WOOD, 2)
			  .withResourceCost(ResourceType::GLASS, 1)
			  .withResourceCost(ResourceType::PAPYRUS, 1),
		CardEffect().withScienceSymbol(ScientificSymbol::SUNDIAL)
					.withVictoryPoints(3),
		"study.png"
	));

	ageIIIDeck.push_back(std::make_unique<Card>(
		"Chamber of Commerce", CardColor::YELLOW, 3,
		Cost().withResourceCost(ResourceType::PAPYRUS, 2),
		CardEffect().withCoinsPerCardType(CardColor::GREY, 3)
					.withVictoryPoints(3),
		"chamber_of_commerce.png"
	));

	ageIIIDeck.push_back(std::make_unique<Card>(
		"Port", CardColor::YELLOW, 3,
		Cost().withResourceCost(ResourceType::WOOD, 1)
			  .withResourceCost(ResourceType::GLASS, 1)
			  .withResourceCost(ResourceType::PAPYRUS, 1),
		CardEffect().withCoinsPerCardType(CardColor::BROWN, 2)
					.withVictoryPoints(3),
		"port.png"
	));

	ageIIIDeck.push_back(std::make_unique<Card>(
		"Armory", CardColor::YELLOW, 3,
		Cost().withResourceCost(ResourceType::STONE, 2)
			  .withResourceCost(ResourceType::GLASS, 1),
		CardEffect().withCoinsPerCardType(CardColor::RED, 1)
					.withVictoryPoints(3),
		"armory.png"
	));

	ageIIIDeck.push_back(std::make_unique<Card>(
		"Palace", CardColor::BLUE, 3,
		Cost().withResourceCost(ResourceType::CLAY, 1)
			  .withResourceCost(ResourceType::STONE, 1)
			  .withResourceCost(ResourceType::WOOD, 1)
			  .withResourceCost(ResourceType::GLASS, 2),
		CardEffect().withVictoryPoints(7),
		"palace.png"
	));

	ageIIIDeck.push_back(std::make_unique<Card>(
		"Town Hall", CardColor::BLUE, 3,
		Cost().withResourceCost(ResourceType::STONE, 3)
			  .withResourceCost(ResourceType::WOOD, 2),
		CardEffect().withVictoryPoints(7),
		"town_hall.png"
	));

	ageIIIDeck.push_back(std::make_unique<Card>(
		"Obelisk", CardColor::BLUE, 3,
		Cost().withResourceCost(ResourceType::STONE, 2)
			  .withResourceCost(ResourceType::GLASS, 1),
		CardEffect().withVictoryPoints(5),
		"obelisk.png"
	));

	ageIIIDeck.push_back(std::make_unique<Card>(
		"Fortifications", CardColor::RED, 3,
		Cost().withResourceCost(ResourceType::STONE, 2)
			  .withResourceCost(ResourceType::CLAY, 1)
			  .withResourceCost(ResourceType::PAPYRUS, 1),
		CardEffect().withShields(2),
		"fortifications.png",
		"Palisade",
		std::nullopt
	));

	ageIIIDeck.push_back(std::make_unique<Card>(
		"Siege Workshop", CardColor::RED, 3,
		Cost().withResourceCost(ResourceType::WOOD, 3)
			  .withResourceCost(ResourceType::GLASS, 1),
		CardEffect().withShields(2),
		"siege_workshop.png",
		"Archery Range",
		std::nullopt
	));

	ageIIIDeck.push_back(std::make_unique<Card>(
		"Circus", CardColor::RED, 3,
		Cost().withResourceCost(ResourceType::CLAY, 2)
			  .withResourceCost(ResourceType::STONE, 2),
		CardEffect().withShields(2),
		"circus.png",
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
		"university.png",
		"School",
		std::nullopt
	));

	ageIIIDeck.push_back(std::make_unique<Card>(
		"Observatory", CardColor::GREEN, 3,
		Cost().withResourceCost(ResourceType::STONE, 1)
			  .withResourceCost(ResourceType::PAPYRUS, 2),
		CardEffect().withScienceSymbol(ScientificSymbol::ASTROLABE)
					.withVictoryPoints(2),
		"observatory.png",
		"Laboratory",
		std::nullopt
	));

	ageIIIDeck.push_back(std::make_unique<Card>(
		"Gardens", CardColor::BLUE, 3,
		Cost().withResourceCost(ResourceType::CLAY, 2)
			  .withResourceCost(ResourceType::WOOD, 2),
		CardEffect().withVictoryPoints(6),
		"gardens.png",
		"Statue",
		std::nullopt
	));

	ageIIIDeck.push_back(std::make_unique<Card>(
		"Pantheon", CardColor::BLUE, 3,
		Cost().withResourceCost(ResourceType::CLAY, 1)
			  .withResourceCost(ResourceType::WOOD, 1)
			  .withResourceCost(ResourceType::PAPYRUS, 2),
		CardEffect().withVictoryPoints(6),
		"pantheon.png",
		"Temple",
		std::nullopt
	));

	ageIIIDeck.push_back(std::make_unique<Card>(
		"Senate", CardColor::BLUE, 3,
		Cost().withResourceCost(ResourceType::CLAY, 2)
			  .withResourceCost(ResourceType::STONE, 1)
			  .withResourceCost(ResourceType::PAPYRUS, 1),
		CardEffect().withVictoryPoints(5),
		"senate.png",
		"Rostrum",
		std::nullopt
	));

	ageIIIDeck.push_back(std::make_unique<Card>(
		"Lighthouse", CardColor::YELLOW, 3,
		Cost().withResourceCost(ResourceType::CLAY, 2)
			  .withResourceCost(ResourceType::GLASS, 1),
		CardEffect().withCoinsPerCardType(CardColor::YELLOW, 1)
					.withVictoryPoints(3),
		"lighthouse.png",
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
		"arena.png",
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
					.grantsGuildCopy()
					.withCustomDescription("At the time it is constructed, the card grants you 1 coin for each yellow card in the city which has the most yellow cards.\n"
						"At the end of the game, this card is worth 1 victory point for each yellow card in the city which has the most yellow cards.\n"),
		"merchants_guild.png"
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
					.grantsGuildCopy()
					.withCustomDescription("At the time it is constructed, this card grants you 1 coin for each brown and each grey card in the city which has the most brown and grey cards.\n"
							"At the end of the game, this card is worth 1 victory point for each brown and grey card in the city which has the most brown and grey cards.\n"),
		"shipowners_guild.png"
	));

	guildDeck.push_back(std::make_unique<Card>(
		"Builders Guild", CardColor::PURPLE, 3,
		Cost().withResourceCost(ResourceType::STONE, 2)
			  .withResourceCost(ResourceType::CLAY, 1)
			  .withResourceCost(ResourceType::WOOD, 1)
			  .withResourceCost(ResourceType::GLASS, 1),
		CardEffect().withPointsPerWonder(2)
					.withCustomDescription("At the end of the game, this card is worth 2 victory points for each Wonder constructed in the city which has the most wonders.\n"),
		"builders_guild.png"
	));

	guildDeck.push_back(std::make_unique<Card>(
		"Magistrates Guild", CardColor::PURPLE, 3,
		Cost().withResourceCost(ResourceType::WOOD, 2)
			  .withResourceCost(ResourceType::CLAY, 1)
			  .withResourceCost(ResourceType::PAPYRUS, 1),
		CardEffect().withCoinsPerCardType(CardColor::BLUE, 1)
					.withPointsPerCardType(CardColor::BLUE, 1)
					.grantsGuildCopy()
					.withCustomDescription("At the time it is constructed, this card grants you 1 coin for each blue card in the city which has the most blue cards.\n"
						"At the end of the game, this card is worth 1 victory point for each blue card in the city which has the most blue cards.\n"),
		"magistrates_guild.png"
	));

	guildDeck.push_back(std::make_unique<Card>(
		"Scientists Guild", CardColor::PURPLE, 3,
		Cost().withResourceCost(ResourceType::CLAY, 2)
			  .withResourceCost(ResourceType::WOOD, 2),
		CardEffect().withCoinsPerCardType(CardColor::GREEN, 1)
					.withPointsPerCardType(CardColor::GREEN, 1)
					.grantsGuildCopy()
					.withCustomDescription("At the time it is constructed, this card earns you 1 coin for each green card in the city which has the most green cards.\n"
						"At the end of the game, this card is worth 1 victory point for each green card in the city which has the most green cards.\n"),
		"scientists_guild.png"
	));

	guildDeck.push_back(std::make_unique<Card>(
		"Moneylenders Guild", CardColor::PURPLE, 3,
		Cost().withResourceCost(ResourceType::STONE, 2)
			  .withResourceCost(ResourceType::WOOD, 2),
		CardEffect().withPointPerCoins(1, 3)
					.withCustomDescription("At the end of the game, this card is worth 1 victory point for each set of 3 coins in the richest city.\n"),
		"moneylenders_guild.png"
	));

	guildDeck.push_back(std::make_unique<Card>(
		"Tacticians Guild", CardColor::PURPLE, 3,
		Cost().withResourceCost(ResourceType::STONE, 2)
			  .withResourceCost(ResourceType::CLAY, 1)
			  .withResourceCost(ResourceType::PAPYRUS, 1),
		CardEffect().withCoinsPerCardType(CardColor::RED, 1)
					.withPointsPerCardType(CardColor::RED, 1)
					.grantsGuildCopy()
					.withCustomDescription("At the time it is constructed, this card grants you 1 coin for each red card in the city which has the most red cards.\n"
						"At the end of the game, this card is worth 1 victory point for each red card in the city which has the most red cards.\n"),
		"tacticians_guild.png"
	));

	return guildDeck;
}

