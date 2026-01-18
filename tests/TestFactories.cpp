#include <gtest/gtest.h>
#include "CardFactory.h"
#include "ProgressTokenFactory.h"
#include "WonderFactory.h"
#include "Constants.h"

TEST(CardFactoryTest, CreateAgeIDeck_CreatesCorrectCards) {
	CardFactory factory;
	auto ageIDeck = factory.createAgeIDeck();

	EXPECT_EQ(ageIDeck.size(), 23);

	for (const auto& card : ageIDeck) {
		ASSERT_NE(card, nullptr);
		EXPECT_EQ(card->getAge(), 1);
	}

	bool foundGuardTower = false;
	for (const auto& card : ageIDeck) {
		if (card->getName() == "Guard Tower") foundGuardTower = true;
	}
	EXPECT_TRUE(foundGuardTower);
}

TEST(CardFactoryTest, CreateAgeIIDeck_CreatesCorrectCards) {
	CardFactory factory;
	auto ageIIDeck = factory.createAgeIIDeck();

	EXPECT_EQ(ageIIDeck.size(), 23);

	for (const auto& card : ageIIDeck) {
		ASSERT_NE(card, nullptr);
		EXPECT_EQ(card->getAge(), 2);
	}

	bool foundHorseBreeders = false;
	for (const auto& card : ageIIDeck) {
		if (card->getName() == "Horse Breeders") {
			foundHorseBreeders = true;
			EXPECT_TRUE(card->getFreeChainFrom().has_value());
			EXPECT_EQ(card->getFreeChainFrom().value(), "Stable");
		}
	}
	EXPECT_TRUE(foundHorseBreeders);
}

TEST(CardFactoryTest, CreateAgeIIIDeck_CreatesCorrectCards) {
	CardFactory factory;
	auto ageIIIDeck = factory.createAgeIIIDeck();

	EXPECT_EQ(ageIIIDeck.size(), 20);

	for (const auto& card : ageIIIDeck) {
		ASSERT_NE(card, nullptr);
		EXPECT_EQ(card->getAge(), 3);
	}
}

TEST(CardFactoryTest, CreateGuildDeck_CreatesCorrectCards) {
	CardFactory factory;
	auto guildDeck = factory.createGuildDeck();

	EXPECT_EQ(guildDeck.size(), 7);

	for (const auto& card : guildDeck) {
		ASSERT_NE(card, nullptr);
		EXPECT_EQ(card->getColor(), CardColor::PURPLE);
		EXPECT_EQ(card->getAge(), 3);
	}

	bool foundMerchantsGuild = false;
	for (const auto& card : guildDeck) {
		if (card->getName() == "Merchants Guild") {
			foundMerchantsGuild = true;
		}
	}
	EXPECT_TRUE(foundMerchantsGuild);
}

TEST(ProgressTokenFactoryTest, CreateAllTokens_CreatesCorrectTokens) {
	ProgressTokenFactory factory;
	auto tokens = factory.createAllTokens();

	EXPECT_EQ(tokens.size(), 10);

	for (const auto& token : tokens) {
		ASSERT_NE(token, nullptr);
		EXPECT_FALSE(token->getName().empty());
		EXPECT_FALSE(token->getDescription().empty());
		EXPECT_FALSE(token->isActive());
	}

	bool foundAgriculture = false;
	bool foundPhilosophy = false;
	for (const auto& token : tokens) {
		if (token->getType() == ProgressTokenType::AGRICULTURE) {
			foundAgriculture = true;
			EXPECT_EQ(token->getName(), "Agriculture");
			EXPECT_EQ(token->getVictoryPoints(), 4);
		}
		if (token->getType() == ProgressTokenType::PHILOSOPHY) {
			foundPhilosophy = true;
			EXPECT_EQ(token->getName(), "Philosophy");
			EXPECT_EQ(token->getVictoryPoints(), 7);
		}
	}

	EXPECT_TRUE(foundAgriculture);
	EXPECT_TRUE(foundPhilosophy);
}

TEST(WonderFactoryTest, CreateWonders_CreatesCorrectWonders) {
	WonderFactory factory;
	auto wonders = factory.createWonders();

	EXPECT_EQ(wonders.size(), 12);

	for (const auto& wonder : wonders) {
		ASSERT_NE(wonder, nullptr);
		EXPECT_FALSE(wonder->getName().empty());
		EXPECT_FALSE(wonder->getDescription().empty());
		EXPECT_FALSE(wonder->isBuilt());
	}

	bool foundPyramids = false;
	bool foundColossus = false;
	for (const auto& wonder : wonders) {
		if (wonder->getName() == "THE PYRAMIDS") {
			foundPyramids = true;
			EXPECT_EQ(wonder->getEffect().getVictoryPointsPerCard().value(), 9);
		}
		if (wonder->getName() == "THE COLOSSUS") {
			foundColossus = true;
			EXPECT_EQ(wonder->getEffect().getShields().value(), 2);
			EXPECT_EQ(wonder->getEffect().getVictoryPointsPerCard().value(), 3);
		}
	}

	EXPECT_TRUE(foundPyramids);
	EXPECT_TRUE(foundColossus);
}