#include "WonderFactory.h"

std::vector<std::unique_ptr<Wonder>> WonderFactory::createWonders()
{
	std::vector<std::unique_ptr<Wonder>> Wonders;
	Wonders.reserve(12);

	Wonders.push_back(std::make_unique<Wonder>(
		"The Appian Way",
		Cost().withResourceCost(ResourceType::PAPYRUS, 1)
		.withResourceCost(ResourceType::CLAY, 2)
		.withResourceCost(ResourceType::STONE, 2),
		3,
		CardEffect().withBaseCoins(3)
		.grantsPlayAgain()
		//+ opponent pierde 3 monede
	));

	Wonders.push_back(std::make_unique<Wonder>(
		"Circus Maximus",
		Cost().withResourceCost(ResourceType::GLASS, 1)
		.withResourceCost(ResourceType::WOOD, 1)
		.withResourceCost(ResourceType::STONE, 2),
		3,
		CardEffect().withShields(1)
		//+ Place in the discard pile a grey card of your choice constructed by your opponent.
	));

	Wonders.push_back(std::make_unique<Wonder>(
		"The Colossus",
		Cost().withResourceCost(ResourceType::GLASS, 1)
		.withResourceCost(ResourceType::CLAY, 3),
		3,
		CardEffect().withShields(2)
		
	));

	Wonders.push_back(std::make_unique<Wonder>(
		"The Great Library",
		Cost().withResourceCost(ResourceType::PAPYRUS, 1)
		.withResourceCost(ResourceType::GLASS, 1)
		.withResourceCost(ResourceType::WOOD, 3),
		4
		//+ Randomly draw 3 Progress tokens from among those discarded at the beginning of the game.Choose one, play it, 
		// and return the other 2 to the box.
	));

	Wonders.push_back(std::make_unique<Wonder>(
		"The Great Lighthouse",
		Cost().withResourceCost(ResourceType::PAPYRUS, 2)
		.withResourceCost(ResourceType::STONE, 1)
		.withResourceCost(ResourceType::WOOD, 1),
		4,
		CardEffect().withProduction(ResourceProduction().addChoice(
			{
			ResourceType::STONE,
			ResourceType::CLAY,
			ResourceType::WOOD
			}
		))
	));

	Wonders.push_back(std::make_unique<Wonder>(
		"The Hanging Gardens",
		Cost().withResourceCost(ResourceType::PAPYRUS, 1)
		.withResourceCost(ResourceType::GLASS, 1)
		.withResourceCost(ResourceType::WOOD, 2),
		3,
		CardEffect().withBaseCoins(3)
		.grantsPlayAgain()
	));

	Wonders.push_back(std::make_unique<Wonder>(
		"The Mausoleum",
		Cost().withResourceCost(ResourceType::PAPYRUS, 1)
		.withResourceCost(ResourceType::GLASS, 2)
		.withResourceCost(ResourceType::CLAY, 2),
		2
		//+Take all of the cards which have been discarded since the beginning
		//of the game and immediately construct one of your choice for free.
	));

	Wonders.push_back(std::make_unique<Wonder>(
		"Piraeus",
		Cost().withResourceCost(ResourceType::CLAY, 1)
		.withResourceCost(ResourceType::STONE, 1)
		.withResourceCost(ResourceType::WOOD, 2),
		2,
		CardEffect().withProduction(ResourceProduction().addChoice(
			{
			ResourceType::PAPYRUS,
			ResourceType::GLASS
			}
		))
		.grantsPlayAgain()
	));

	Wonders.push_back(std::make_unique<Wonder>(
		"The Pyramids",
		Cost().withResourceCost(ResourceType::PAPYRUS, 1)
		.withResourceCost(ResourceType::STONE, 3),
		9
	));

	Wonders.push_back(std::make_unique<Wonder>(
		"The Sphinx",
		Cost().withResourceCost(ResourceType::GLASS, 2)
		.withResourceCost(ResourceType::CLAY, 1)
		.withResourceCost(ResourceType::STONE, 1),
		6,
		CardEffect().grantsPlayAgain()
	));

	Wonders.push_back(std::make_unique<Wonder>(
		"The Statue of Zeus",
		Cost().withResourceCost(ResourceType::PAPYRUS, 1)
		.withResourceCost(ResourceType::STONE, 3),
		9
	));

	Wonders.push_back(std::make_unique<Wonder>(
		"The Temple of Artemis",
		Cost().withResourceCost(ResourceType::PAPYRUS, 1)
		.withResourceCost(ResourceType::GLASS, 1)
		.withResourceCost(ResourceType::STONE, 1)
		.withResourceCost(ResourceType::WOOD, 1),
		0,
		CardEffect().withBaseCoins(12)
		.grantsPlayAgain()
	));

	return Wonders;
}
