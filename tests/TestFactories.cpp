#include <gtest/gtest.h>
#include "CardFactory.h"
#include "Card.h"
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