#include<gtest/gtest.h>
#include "Cost.h"
#include "Constants.h"

TEST(CostTest, Initializaton_And_Getters) {
	Cost emptyCost;
	EXPECT_TRUE(emptyCost.isFree());
	EXPECT_EQ(emptyCost.getCoinCost(), 0);
	EXPECT_EQ(emptyCost.getTotalResourceCount(), 0);

	Cost moneyCost(5);
	EXPECT_EQ(moneyCost.getCoinCost(), 5);
	EXPECT_FALSE(moneyCost.isFree());

	Cost complexCost;
	complexCost.withCoinCost(2)
		.withResourceCost(ResourceType::WOOD, 1)
		.withResourceCost(ResourceType::STONE, 2);

	EXPECT_EQ(complexCost.getCoinCost(), 2);
	EXPECT_EQ(complexCost.getResourceCost(ResourceType::WOOD), 1);
	EXPECT_EQ(complexCost.getResourceCost(ResourceType::STONE), 2);
	EXPECT_EQ(complexCost.getResourceCost(ResourceType::GLASS), 0);
}
TEST(CostTest, CanAfford_Logic) {
	Cost cost;
	cost.withCoinCost(3).withResourceCost(ResourceType::CLAY, 1);

	std::map<ResourceType, int> myResources;
	myResources[ResourceType::CLAY] = 1;
	int myCoins = 5;
	EXPECT_TRUE(cost.canAfford(myCoins, myResources));
	EXPECT_FALSE(cost.canAfford(2, myResources));

	myResources[ResourceType::CLAY] = 0;
	EXPECT_FALSE(cost.canAfford(10, myResources));
}
TEST(CostTest, TradingCost_Calculation) {
	Cost cost;
	cost.withResourceCost(ResourceType::WOOD, 2);
	std::map<ResourceType, int> myResources;
	std::map<ResourceType, int> opponentProduction;

	opponentProduction[ResourceType::WOOD] = 0;
	EXPECT_EQ(cost.calculateTradingCost(myResources, opponentProduction), 4);
	opponentProduction[ResourceType::WOOD] = 1;
	EXPECT_EQ(cost.calculateTradingCost(myResources, opponentProduction), 6);
}

TEST(CostTest, ApplyDiscounts) {
	Cost original;
	original.withResourceCost(ResourceType::STONE, 3);
	Cost discounted = original.applyDiscount(ResourceType::STONE, 2);

	EXPECT_EQ(discounted.getResourceCost(ResourceType::STONE), 1);
}

TEST(CostTest, Operators_Work) {
	Cost c1; c1.withCoinCost(2);
	Cost c2; c2.withCoinCost(2);
	Cost c3; c3.withCoinCost(5);

	EXPECT_TRUE(c1 == c2);
	EXPECT_FALSE(c1 == c3);

	Cost sum = c1 + c3;
	EXPECT_EQ(sum.getCoinCost(), 7);
}