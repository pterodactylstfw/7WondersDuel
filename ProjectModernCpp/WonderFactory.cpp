	#include "WonderFactory.h"

std::vector<std::unique_ptr<Wonder>> WonderFactory::createWonders()
{
	std::vector<std::unique_ptr<Wonder>> Wonders;
	Wonders.reserve(12);

	Wonders.push_back(std::make_unique<Wonder>(
		WonderType::THE_APPIAN_WAY,
		Cost().withResourceCost(ResourceType::PAPYRUS, 1)
		.withResourceCost(ResourceType::CLAY, 2)
		.withResourceCost(ResourceType::STONE, 2),
		3,
		CardEffect().withBaseCoins(3)
		.grantsPlayAgain()
		.withCustomDescription("You take 3 coins from the bank. Your opponent loses 3 coins.\n"
			                   "Immediately play a second turn.\n" 
			                   "This Wonder is worth 3 victory points\n")
		//+ opponent pierde 3 monede
	));

	Wonders.push_back(std::make_unique<Wonder>(
		WonderType::CIRCUS_MAXIMUS,
		Cost().withResourceCost(ResourceType::GLASS, 1)
		.withResourceCost(ResourceType::WOOD, 1)
		.withResourceCost(ResourceType::STONE, 2),
		3,
		CardEffect().withShields(1)
		.withCustomDescription("Place in the discard pile a grey card of your choice constructed by your opponent.\n"
								"This Wonder is worth 1 Shield.\n"
								"This Wonder is worth 3 victory points.\n")
		//+ Place in the discard pile a grey card of your choice constructed by your opponent.
	));

	Wonders.push_back(std::make_unique<Wonder>(
		WonderType::THE_COLOSSUS,
		Cost().withResourceCost(ResourceType::GLASS, 1)
		.withResourceCost(ResourceType::CLAY, 3),
		3,
		CardEffect().withShields(2)
		.withCustomDescription("This Wonder is worth 2 Shields.\n"
		                       "This Wonder is worth 3 victory points.\n")
	));

	Wonders.push_back(std::make_unique<Wonder>(
		WonderType::THE_GREAT_LIBRARY,
		Cost().withResourceCost(ResourceType::PAPYRUS, 1)
		.withResourceCost(ResourceType::GLASS, 1)
		.withResourceCost(ResourceType::WOOD, 3),
		4,
		CardEffect(). withCustomDescription("Randomly draw 3 Progress tokens from those discarded. Choose one, play it, and return the other\n"
		                                    "This Wonder is worth 4 victory points.\n")
		//+ Randomly draw 3 Progress tokens from among those discarded at the beginning of the game.Choose one, play it, 
		// and return the other 2 to the box.
	));

	Wonders.push_back(std::make_unique<Wonder>(
		WonderType::THE_GREAT_LIGHTHOUSE,
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
		.withCustomDescription("This Wonder produces one unit of: Stone, Clay, or Wood.\n"
		                       "This Wonder is worth 4 victory points.\n")
	));

	Wonders.push_back(std::make_unique<Wonder>(
		WonderType::THE_HANGING_GARDENS,
		Cost().withResourceCost(ResourceType::PAPYRUS, 1)
		.withResourceCost(ResourceType::GLASS, 1)
		.withResourceCost(ResourceType::WOOD, 2),
		3,
		CardEffect().withBaseCoins(3)
		.grantsPlayAgain()
		.withCustomDescription("You take 6 coins from the bank.\n"
							   "Immediately play a second turn.\n"
			                   "This Wonder is worth 3 victory points.\n")
	));

	Wonders.push_back(std::make_unique<Wonder>(
		WonderType::THE_MAUSOLEUM,
		Cost().withResourceCost(ResourceType::PAPYRUS, 1)
		.withResourceCost(ResourceType::GLASS, 2)
		.withResourceCost(ResourceType::CLAY, 2),
		2,
		CardEffect().withCustomDescription("Take all of the cards which have been discarded and construct one for free.\n"
		                                   "This Wonder is worth 2 victory points.\n")
		//+Take all of the cards which have been discarded since the beginning
		//of the game and immediately construct one of your choice for free.
	));

	Wonders.push_back(std::make_unique<Wonder>(
		WonderType::PIRAEUS,
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
		.withCustomDescription("This Wonder produces one unit of: Papyrus or Glass.\n"
		                       "Immediately play a second turn.\n"
							   "This Wonder is worth 2 victory points.\n")
	));

	Wonders.push_back(std::make_unique<Wonder>(
		WonderType::THE_PYRAMIDS,
		Cost().withResourceCost(ResourceType::PAPYRUS, 1)
		.withResourceCost(ResourceType::STONE, 3),
		9,
		CardEffect().withCustomDescription("This Wonder is worth 9 victory points.\n")
	));

	Wonders.push_back(std::make_unique<Wonder>(
		WonderType::THE_SPHINX,
		Cost().withResourceCost(ResourceType::GLASS, 2)
		.withResourceCost(ResourceType::CLAY, 1)
		.withResourceCost(ResourceType::STONE, 1),
		6,
		CardEffect().grantsPlayAgain()
		.withCustomDescription("Immediately play a second turn.\n"
		                       "This Wonder is worth 6 victory points.\n")
	));

	Wonders.push_back(std::make_unique<Wonder>(
		WonderType::THE_STATUE_OF_ZEUS,
		Cost().withResourceCost(ResourceType::PAPYRUS, 1)
		.withResourceCost(ResourceType::STONE, 3),
		9,
		CardEffect().withCustomDescription("Place in the discard pile a brown card of your choice constructed by your opponent.\n"
		                                   "This Wonder is worth 1 Shield.\n"
		                                   "This Wonder is worth 3 victory points.\n")
	));

	Wonders.push_back(std::make_unique<Wonder>(
		WonderType::THE_TEMPLE_OF_ARTEMIS,
		Cost().withResourceCost(ResourceType::PAPYRUS, 1)
		.withResourceCost(ResourceType::GLASS, 1)
		.withResourceCost(ResourceType::STONE, 1)
		.withResourceCost(ResourceType::WOOD, 1),
		0,
		CardEffect().withBaseCoins(12)
		.grantsPlayAgain()
		.withCustomDescription("Immediately take 12 coins from the Bank.\n"
		                       "Immediately play a second turn.\n")
	));

	return Wonders;
}
