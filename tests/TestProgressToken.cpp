#include <gtest/gtest.h>
#include "ProgressToken.h"
#include "CardEffect.h"
#include "Constants.h"

TEST(ProgressTokenTest, Initialization_And_Getters) {
	CardEffect effect;
	effect.withBaseCoins(6)
		.withVictoryPoints(4);
	ProgressToken token(ProgressTokenType::AGRICULTURE, "Agriculture", effect, "agriculture.png");

	EXPECT_EQ(token.getType(), ProgressTokenType::AGRICULTURE);
	EXPECT_EQ(token.getName(), "Agriculture");
	EXPECT_EQ(token.getVictoryPoints(), 4);
	EXPECT_EQ(token.getEffect().getBaseCoins(), 6);
	EXPECT_FALSE(token.isActive());
	EXPECT_EQ(token.getImagePath(), ":/assets/tokens/agriculture.png");
}

TEST(ProgressTokenTest, SetActive_ChangesState) {
	CardEffect effect;
	ProgressToken token(ProgressTokenType::STRATEGY, "Strategy", effect, "strategy.png");

	EXPECT_FALSE(token.isActive());

	token.setActive(true);
	EXPECT_TRUE(token.isActive());

	token.setActive(false);
	EXPECT_FALSE(token.isActive());
}