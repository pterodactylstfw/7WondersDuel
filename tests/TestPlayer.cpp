#include <gtest/gtest.h>
#include "Player.h"
#include "Constants.h"
#include "GameController.h"
#include "MockGameView.h"

TEST(PlayerTest, InitialState) {
	std::string name = "TestUser";
	Player p(name);

	EXPECT_EQ(p.getName(), name);

	EXPECT_EQ(p.getCoins(), GameConstants::STARTING_COINS);

	EXPECT_EQ(p.getMilitaryShields(), 0);
}

TEST(PlayerTest, AddCoins) {
	Player p("Player1");
	int initial = p.getCoins();

	p.addCoins(5);
	EXPECT_EQ(p.getCoins(), initial + 5);
}

TEST(PlayerTest, RemoveCoinsProtection) {
	Player p("Player1");
	p.addCoins(100);

	p.removeCoins(1000);

	EXPECT_EQ(p.getCoins(), 0);
}

TEST(PlayerTest, CardSorting) {
	Player p("Sorter");
	Cost cost; CardEffect effect;

	Card brown("Lumber", CardColor::BROWN, 1, cost, effect, "");
	Card grey("Glass", CardColor::GREY, 1, cost, effect, "");
	Card blue("Altar", CardColor::BLUE, 1, cost, effect, "");

	p.addCard(std::make_unique<Card>(brown));
	p.addCard(std::make_unique<Card>(grey));
	p.addCard(std::make_unique<Card>(blue));

	auto brownCards = p.getCardsOfType(CardColor::BROWN);
	EXPECT_EQ(brownCards.size(), 1);
	EXPECT_EQ(brownCards[0].get().getName(), "Lumber");

	auto redCards = p.getCardsOfType(CardColor::RED);
	EXPECT_TRUE(redCards.empty());
}

TEST(PlayerTests, ScientificVictory_Duplicates) {
	Player p("ScienceFail");

	p.addScientificSymbol(ScientificSymbol::WHEEL);
	p.addScientificSymbol(ScientificSymbol::ASTROLABE);
	p.addScientificSymbol(ScientificSymbol::MORTAR_PESTLE);
	p.addScientificSymbol(ScientificSymbol::SUNDIAL);
	p.addScientificSymbol(ScientificSymbol::SCALES);

	EXPECT_FALSE(p.hasScientificVictory());

	p.addScientificSymbol(ScientificSymbol::WHEEL);

	EXPECT_FALSE(p.hasScientificVictory());

	p.addScientificSymbol(ScientificSymbol::QUILL_INKWELL);
	EXPECT_TRUE(p.hasScientificVictory());
}

TEST(PlayerTests, TradingCostCalculation) {
	Player me("Me");
	Player opponent("Opponent");

	std::map<ResourceType, uint8_t> req;
	req[ResourceType::WOOD] = 1;
	Cost woodCost(0, req);

	EXPECT_EQ(me.calculateTradeCost(woodCost, opponent), 2);

	ResourceProduction prod;
	prod.addFixedResource(ResourceType::WOOD, 1);

	opponent.addResourceProduction(prod);
	opponent.addResourceProduction(prod);

	EXPECT_EQ(me.calculateTradeCost(woodCost, opponent), 4);
}

TEST(PlayerTests, CanAfford_WithTrading) {
	Player p("Buyer");
	Player opponent("Seller");

	std::map<ResourceType, uint8_t> req;
	req[ResourceType::WOOD] = 1;
	Cost costOfCard(0, req);

	p.addCoins(100);

	EXPECT_TRUE(p.canAfford(costOfCard, opponent));

	p.removeCoins(p.getCoins());

	EXPECT_FALSE(p.canAfford(costOfCard, opponent));
}