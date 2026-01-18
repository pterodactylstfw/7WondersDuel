#include <gtest/gtest.h>
#include "CardEffect.h"
#include "Constants.h"

TEST(CardEffectTest, WithMultipleEffects_AllSetCorrectly) {
	CardEffect effect;
	effect.withVictoryPoints(3)
	      .withShields(2)
	      .withBaseCoins(6);
	
	EXPECT_FALSE(effect.isEmpty());
	EXPECT_EQ(effect.getVictoryPointsPerCard().value(), 3);
	EXPECT_EQ(effect.getShields().value(), 2);
	EXPECT_EQ(effect.getBaseCoins().value(), 6);
}

TEST(CardEffectTest, GetDescription_WithEffects) {
	CardEffect effect;
	effect.withVictoryPoints(5).withShields(2);
	
	EXPECT_FALSE(effect.getDescription().empty());
	EXPECT_NE(effect.getDescription().find("5 VP"), std::string::npos);
	EXPECT_NE(effect.getDescription().find("2 shields"), std::string::npos);
}

TEST(CardEffectTest, GrantsPlayAgain_Works) {
	CardEffect effect;
	
	EXPECT_FALSE(effect.getGrantsPlayAgain());
	
	effect.grantsPlayAgain();
	EXPECT_TRUE(effect.getGrantsPlayAgain());
}
