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
		//+ opponent pierde 3 monede
	));

	Wonders.push_back(std::make_unique<Wonder>(
		"THE CIRCUS MAXIMUS",
		Cost().withResourceCost(ResourceType::GLASS, 1)
		.withResourceCost(ResourceType::WOOD, 1)
		.withResourceCost(ResourceType::STONE, 2),
		CardEffect().withShields(1)
		.withVictoryPoints(3)
		//+ Place in the discard pile a grey card of your choice constructed by your opponent.
	));

	Wonders.push_back(std::make_unique<Wonder>(
		"THE COLOSSUS",
		Cost().withResourceCost(ResourceType::GLASS, 1)
		.withResourceCost(ResourceType::CLAY, 3),
		CardEffect().withShields(2)
		.withVictoryPoints(3)
		
	));

	Wonders.push_back(std::make_unique<Wonder>(
		"THE GREAT LIBRARY",
		Cost().withResourceCost(ResourceType::PAPYRUS, 1)
		.withResourceCost(ResourceType::GLASS, 1)
		.withResourceCost(ResourceType::WOOD, 3),
		CardEffect().withVictoryPoints(4)
		//+ Randomly draw 3 Progress tokens from among those discarded at the beginning of the game.Choose one, play it, 
		// and return the other 2 to the box.
	));

	Wonders.push_back(std::make_unique<Wonder>(
		"THE GREAT LIGHTHOUSE",
		Cost().withResourceCost(ResourceType::PAPYRUS, 2)
		.withResourceCost(ResourceType::STONE, 1)
		.withResourceCost(ResourceType::WOOD, 1),
		CardEffect().withProduction(ResourceProduction().addChoice(
			{
			ResourceType::STONE,
			ResourceType::CLAY,
			ResourceType::WOOD
			}
		))
		.withVictoryPoints(4)
	));

	Wonders.push_back(std::make_unique<Wonder>(
		"THE HANGING GARDENS",
		Cost().withResourceCost(ResourceType::PAPYRUS, 1)
		.withResourceCost(ResourceType::GLASS, 1)
		.withResourceCost(ResourceType::WOOD, 2),
		CardEffect().withBaseCoins(3)
		.grantsPlayAgain()
		.withVictoryPoints(3)
	));

	Wonders.push_back(std::make_unique<Wonder>(
		"THE MAUSOLEUM",
		Cost().withResourceCost(ResourceType::PAPYRUS, 1)
		.withResourceCost(ResourceType::GLASS, 2)
		.withResourceCost(ResourceType::CLAY, 2),
		CardEffect().withVictoryPoints(2)
		//+Take all of the cards which have been discarded since the beginning
		//of the game and immediately construct one of your choice for free.
	));

	Wonders.push_back(std::make_unique<Wonder>(
		"THE PIRAEUS",
		Cost().withResourceCost(ResourceType::CLAY, 1)
		.withResourceCost(ResourceType::STONE, 1)
		.withResourceCost(ResourceType::WOOD, 2),
		CardEffect().withProduction(ResourceProduction().addChoice(
			{
			ResourceType::PAPYRUS,
			ResourceType::GLASS
			}
		))
		.grantsPlayAgain()
		.withVictoryPoints(2)
	));

	Wonders.push_back(std::make_unique<Wonder>(
		"THE PYRAMIDS",
		Cost().withResourceCost(ResourceType::PAPYRUS, 1)
		.withResourceCost(ResourceType::STONE, 3),
		CardEffect().withVictoryPoints(9)
	));

	Wonders.push_back(std::make_unique<Wonder>(
		"THE SPHINX",
		Cost().withResourceCost(ResourceType::GLASS, 2)
		.withResourceCost(ResourceType::CLAY, 1)
		.withResourceCost(ResourceType::STONE, 1),
		CardEffect().withVictoryPoints(6)
		.grantsPlayAgain()
	));

	Wonders.push_back(std::make_unique<Wonder>(
		"THE STATUE OF ZEUS",
		Cost().withResourceCost(ResourceType::PAPYRUS, 1)
		.withResourceCost(ResourceType::STONE, 3),
		CardEffect().withVictoryPoints(9)
	));

	Wonders.push_back(std::make_unique<Wonder>(
		"THE TEMPLE OF ARTEMIS",
		Cost().withResourceCost(ResourceType::PAPYRUS, 1)
		.withResourceCost(ResourceType::GLASS, 1)
		.withResourceCost(ResourceType::STONE, 1)
		.withResourceCost(ResourceType::WOOD, 1),
		CardEffect().withBaseCoins(12)
		.grantsPlayAgain()
	));

	return Wonders;
}
