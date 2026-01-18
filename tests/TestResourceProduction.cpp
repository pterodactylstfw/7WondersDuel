#include <gtest/gtest.h>
#include "ResourceProduction.h"
#include "Constants.h"

TEST(ResourceProductionTest, AddFixedResource_AddsCorrectly) {
	ResourceProduction prod;

	prod.addFixedResource(ResourceType::WOOD, 2)
	.addFixedResource(ResourceType::STONE, 1);

	EXPECT_FALSE(prod.isEmpty());
	EXPECT_EQ(prod.getFixedResources().at(ResourceType::WOOD), 2);
	EXPECT_EQ(prod.getFixedResources().at(ResourceType::STONE), 1);
}

TEST(ResourceProductionTest, AddChoice_AddsCorrectly) {
	ResourceProduction prod;

	prod.addChoice({ ResourceType::WOOD, ResourceType::CLAY });

	EXPECT_TRUE(prod.hasChoices());
	EXPECT_EQ(prod.getChoices().size(), 1);
	EXPECT_EQ(prod.getChoices()[0].size(), 2);
}