#include <gtest/gtest.h>
#include "Wonder.h"
#include "Cost.h"
#include "CardEffect.h"
#include "Constants.h"

TEST(WonderTest, Initialization_And_Getters) {
	Cost cost1;
	cost1.withResourceCost(ResourceType::GLASS, 1)
	.withResourceCost(ResourceType::CLAY, 3);
	CardEffect effect1;
	effect1.withShields(2)
	.withVictoryPoints(3);
	Wonder wonder1("THE_COLOSSUS", cost1, effect1, "the_colossus.png");

	EXPECT_EQ(wonder1.getName(), "THE_COLOSSUS");
	EXPECT_FALSE(wonder1.isBuilt());
	EXPECT_EQ(wonder1.getImagePath(), ":/assets/wonders/the_colossus.png");
	EXPECT_EQ(wonder1.getCost().getResourceCost(ResourceType::GLASS), 1);
	EXPECT_EQ(wonder1.getCost().getResourceCost(ResourceType::CLAY), 3);

	ASSERT_TRUE(wonder1.getEffect().getVictoryPointsPerCard().has_value());
	EXPECT_EQ(wonder1.getEffect().getVictoryPointsPerCard().value(), 3);
	ASSERT_TRUE(wonder1.getEffect().getShields().has_value());
	EXPECT_EQ(wonder1.getEffect().getShields().value(), 2);
}

TEST(WonderTest, GetImagePath_Logic) {
	Cost cost;

	Wonder wonder1("The Pyramids", cost, "the_pyramids.png");
	EXPECT_EQ(wonder1.getImagePath(), ":/assets/wonders/the_pyramids.png");

	Wonder wonder2("The Hanging Gardens", cost, "the_hanging_gardens.png");
	EXPECT_EQ(wonder2.getImagePath(), ":/assets/wonders/the_hanging_gardens.png");
}
