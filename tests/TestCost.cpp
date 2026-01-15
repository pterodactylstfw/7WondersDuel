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