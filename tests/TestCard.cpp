#include <gtest/gtest.h>
#include "Card.h"
#include "Cost.h"
#include "CardEffect.h"
#include "Constants.h"
#include <optional>

TEST(CardTest, Initialization_And_Getters) {
    Cost cost;
    CardEffect effect;
    effect.withVictoryPoints(3);
    Card card("Test Temple", CardColor::BLUE, 1, cost, effect, "temple.png");

    EXPECT_EQ(card.getName(), "Test Temple");
    EXPECT_EQ(card.getColor(), CardColor::BLUE);
    EXPECT_EQ(card.getAge(), 1);
    EXPECT_TRUE(card.getCost().isFree());
    EXPECT_EQ(card.getEffect().getVictoryPointsPerCard(), 3);
}

TEST(CardTest, Chaining_Logic) {
    Cost cost;
    CardEffect effect;
    Card scriptorium("Scriptorium", CardColor::GREEN, 1, cost, effect, "script.png", std::nullopt, "Library");
    Card library("Library", CardColor::GREEN, 2, cost, effect, "lib.png", "Scriptorium", std::nullopt);
    Card baths("Baths", CardColor::BLUE, 1, cost, effect, "baths.png");

    EXPECT_TRUE(library.canBeBuiltFreelyAfter(scriptorium));
    EXPECT_TRUE(scriptorium.canBuildFreeChainTo(library));
    EXPECT_FALSE(library.canBeBuiltFreelyAfter(baths));
}