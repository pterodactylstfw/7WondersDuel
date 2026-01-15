#include<gtest/gtest.h>
#include "Cost.h"
#include "Constants.h"

TEST(CostTest, Initaialization_And_Getters) {
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