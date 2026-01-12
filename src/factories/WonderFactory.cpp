#include "WonderFactory.h"

std::vector<std::unique_ptr<Wonder>> WonderFactory::createWonders()
{
	std::vector<std::unique_ptr<Wonder>> Wonders;
	Wonders.reserve(12);

	Wonders.push_back(std::make_unique<Wonder>(
		"THE APPIAN WAY",
		Cost().withResourceCost(ResourceType::PAPYRUS, 1)
			  .withResourceCost(ResourceType::CLAY, 2)
			  .withResourceCost(ResourceType::STONE, 2),
		CardEffect().withBaseCoins(3)
					.grantsPlayAgain()
					.withVictoryPoints(3)
					.opponentLosesCoins(3)
					.withCustomDescription("You take 3 coins from the bank. Your opponent loses 3 coins.\n"
			                   "Immediately play a second turn.\n" 
			                   "This Wonder is worth 3 victory points\n"),
		"the_appian_way.png"
	));

	Wonders.push_back(std::make_unique<Wonder>(
		"THE CIRCUS MAXIMUS",
		Cost().withResourceCost(ResourceType::GLASS, 1)
			  .withResourceCost(ResourceType::WOOD, 1)
			  .withResourceCost(ResourceType::STONE, 2),
		CardEffect().withShields(1)
					.withVictoryPoints(3)
					.opponentlosesCard(CardColor::GREY)
					.withCustomDescription("Place in the discard pile a grey card of your choice constructed by your opponent.\n"
								"This Wonder is worth 1 Shield.\n"
								"This Wonder is worth 3 victory points.\n"),
		"circus_maximus.png"
	));

	Wonders.push_back(std::make_unique<Wonder>(
		"THE COLOSSUS",
		Cost().withResourceCost(ResourceType::GLASS, 1)
		.withResourceCost(ResourceType::CLAY, 3),
		CardEffect().withShields(2)
					.withVictoryPoints(3)
					.withCustomDescription("This Wonder is worth 2 Shields\n"
						"This Wonder is worth 3 victory points\n"),
		"the_colossus.png"
	));

	Wonders.push_back(std::make_unique<Wonder>(
		"THE GREAT LIBRARY",
		Cost().withResourceCost(ResourceType::PAPYRUS, 1)
		.withResourceCost(ResourceType::GLASS, 1)
		.withResourceCost(ResourceType::WOOD, 3),
		CardEffect().withVictoryPoints(4)
					.grantsProgressToken()
					.withCustomDescription("Randomly draw 3 Progress tokens from those discarded. Choose one, play it, and return the other\n"
		                                    "This Wonder is worth 4 victory points.\n"),
		"the_great_library.png"
	));

	Wonders.push_back(std::make_unique<Wonder>(
		"THE GREAT LIGHTHOUSE",
		Cost().withResourceCost(ResourceType::PAPYRUS, 2)
		.withResourceCost(ResourceType::STONE, 1)
		.withResourceCost(ResourceType::WOOD, 1),
		CardEffect().withProduction(ResourceProduction().addChoice({
						ResourceType::STONE,
						ResourceType::CLAY,
						ResourceType::WOOD
			}))
		.withVictoryPoints(4)
		.withCustomDescription("This Wonder produces one unit of: Stone, Clay, or Wood\n"
			"This Wonder is worth 4 victory points\n"),
		"the_great_lighthouse.png"
	));

	Wonders.push_back(std::make_unique<Wonder>(
		"THE HANGING GARDENS",
		Cost().withResourceCost(ResourceType::PAPYRUS, 1)
		.withResourceCost(ResourceType::GLASS, 1)
		.withResourceCost(ResourceType::WOOD, 2),
		CardEffect().withBaseCoins(6)
					.grantsPlayAgain()
					.withVictoryPoints(3)
					.withCustomDescription("You take 6 coins from the bank.\n"
							   "Immediately play a second turn.\n"
			                   "This Wonder is worth 3 victory points.\n"),
		"the_hanging_gardens.png"
	));

	Wonders.push_back(std::make_unique<Wonder>(
		"THE MAUSOLEUM",
		Cost().withResourceCost(ResourceType::PAPYRUS, 1)
		.withResourceCost(ResourceType::GLASS, 2)
		.withResourceCost(ResourceType::CLAY, 2),
		CardEffect().withVictoryPoints(2)
					.grantsDiscardedCard()
					.withCustomDescription("Take all of the cards which have been discarded and construct one for free.\n"
		                                   "This Wonder is worth 2 victory points.\n"),
		"the_mausoleum.png"
	));

	Wonders.push_back(std::make_unique<Wonder>(
		"THE PIRAEUS",
		Cost().withResourceCost(ResourceType::CLAY, 1)
		.withResourceCost(ResourceType::STONE, 1)
		.withResourceCost(ResourceType::WOOD, 2),
		CardEffect().withProduction(ResourceProduction().addChoice({
						ResourceType::PAPYRUS,
						ResourceType::GLASS
			}))
		.grantsPlayAgain()
		.withVictoryPoints(2)
		.withCustomDescription("This Wonder produces one unit of: Papyrus or Glass\n"
			"Immediately play a second turn\n"
			"This Wonder is worth 2 victory points\n"),
		"the_piraeus.png"
	));

	Wonders.push_back(std::make_unique<Wonder>(
		"THE PYRAMIDS",
		Cost().withResourceCost(ResourceType::PAPYRUS, 1)
		.withResourceCost(ResourceType::STONE, 3),
		CardEffect().withVictoryPoints(9)
					.withCustomDescription("This Wonder is worth 9 victory points.\n"),
		"the_pyramids.png"
	));

	Wonders.push_back(std::make_unique<Wonder>(
		"THE SPHINX",
		Cost().withResourceCost(ResourceType::GLASS, 2)
		.withResourceCost(ResourceType::CLAY, 1)
		.withResourceCost(ResourceType::STONE, 1),
		CardEffect().withVictoryPoints(6)
					.grantsPlayAgain()
					.withCustomDescription("Immediately play a second turn.\n"
		                       "This Wonder is worth 6 victory points.\n"),
		"the_sphinx.png"
	));

	Wonders.push_back(std::make_unique<Wonder>(
		"THE STATUE OF ZEUS",
		Cost().withResourceCost(ResourceType::PAPYRUS, 2)
		.withResourceCost(ResourceType::STONE, 1)
		.withResourceCost(ResourceType::CLAY, 1)
		.withResourceCost(ResourceType::WOOD, 1),
		CardEffect().withShields(1)
					.withVictoryPoints(3)
					.opponentlosesCard(CardColor::BROWN)
					.withCustomDescription("Place in the discard pile a brown card of your choice constructed by your opponent.\n"
		                        "This Wonder is worth 1 Shield.\n"
		                        "This Wonder is worth 3 victory points.\n"),
		"the_statue_of_zeus.png"
	));

	Wonders.push_back(std::make_unique<Wonder>(
		"THE TEMPLE OF ARTEMIS",
		Cost().withResourceCost(ResourceType::PAPYRUS, 1)
		.withResourceCost(ResourceType::GLASS, 1)
		.withResourceCost(ResourceType::STONE, 1)
		.withResourceCost(ResourceType::WOOD, 1),
		CardEffect().withBaseCoins(12)
				.grantsPlayAgain()
				.withCustomDescription("Immediately take 12 coins from the Bank\n"
					"Immediately play a second turn\n"),
		"the_temple_of_artemis.png"
	));

	return Wonders;
}