#include <gtest/gtest.h>
#include "GameState.h"
#include "Card.h"
#include "Cost.h"
#include "CardEffect.h"
#include "Wonder.h"
#include "ProgressToken.h"

std::unique_ptr<Card> createDummyCard(std::string name) {
    return std::make_unique<Card>(name, CardColor::BROWN, 1, Cost(), CardEffect(), "img.png");
}

TEST(GameStateTest, Initialization_Defaults) {
    GameState state("Raul", "Cristian");

    EXPECT_EQ(state.getCurrentPlayer().getName(), "Raul");
    EXPECT_EQ(state.getOpponent().getName(), "Cristian");
    EXPECT_EQ(state.getCurrentAge(), 1);
    EXPECT_FALSE(state.isGameOver());
    EXPECT_EQ(state.getMilitaryPosition(), 0);
}

TEST(GameStateTest, PlayerSwitching) {
    GameState state("Raul", "Cristian");
    EXPECT_EQ(state.getCurrentPlayer().getName(), "Raul");
    state.switchPlayer();
    EXPECT_EQ(state.getCurrentPlayer().getName(), "Cristian");
    state.switchPlayer();
    EXPECT_EQ(state.getCurrentPlayer().getName(), "Raul");
}

TEST(GameStateTest, Pyramid_Mechanics_Age1) {
    GameState state;

    std::vector<std::unique_ptr<Card>> deck;
    for (int i = 0; i < 20; ++i)
        deck.push_back(createDummyCard("Card " + std::to_string(i)));

    state.initializeAge(1, deck);
    const auto& pyramid = state.getPyramid();
    EXPECT_TRUE(pyramid[0].m_isFaceUp);
    EXPECT_TRUE(state.isCardAccessible(0));
    EXPECT_TRUE(state.isCardAccessible(1));
    EXPECT_FALSE(pyramid[6].m_isFaceUp);
    EXPECT_FALSE(state.isCardAccessible(6));

    state.removeCardFromPyramid(0);
    EXPECT_FALSE(state.isCardAccessible(6));
    EXPECT_FALSE(state.getPyramid()[6].m_isFaceUp);
    state.removeCardFromPyramid(1);
    EXPECT_TRUE(state.getPyramid()[6].m_isFaceUp);
    EXPECT_TRUE(state.isCardAccessible(6));
}

TEST(GameStateTest, Military_Calculation) {
    GameState state;
    state.getPlayers()[0]->addMilitaryShields(2);
    state.getPlayers()[1]->addMilitaryShields(1);

    EXPECT_EQ(state.getMilitaryPosition(), 1);
}

TEST(GameStateTest, Drafting_Wonders) {
    GameState state;
    state.addWonderToDraft(std::make_unique<Wonder>("W1", Cost(), "img1.png"));
    state.addWonderToDraft(std::make_unique<Wonder>("W2", Cost(), "img2.png"));

    EXPECT_EQ(state.getDraftedWonders().size(), 2);
    auto w = state.extractWonderFromDraft(0);
    EXPECT_EQ(w->getName(), "W1");
    EXPECT_EQ(state.getDraftedWonders().size(), 1);
}

TEST(GameStateTest, Tokens_Management) {
    GameState state;
    CardEffect dummyEffect;
    auto token = std::make_unique<ProgressToken>(
        ProgressTokenType::LAW,
        "Law Token",
        dummyEffect,
        "law.png"
    );

    state.addToAvailableTokens(std::move(token));

    EXPECT_EQ(state.getAvailableTokens().size(), 1);
    auto extracted = state.removeAvailableTokens(0);
    EXPECT_EQ(extracted->getType(), ProgressTokenType::LAW);
    EXPECT_EQ(state.getAvailableTokens().size(), 0);
}
