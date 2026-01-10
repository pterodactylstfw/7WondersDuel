#include <gtest/gtest.h>
#include "Player.h"
#include "Constants.h"
#include "GameController.h"
#include "MockGameView.h"

// Test?m clasa Player
TEST(PlayerTest, InitialState) {
	std::string name = "TestUser";
	Player p(name);

	// Verific?m numele
	EXPECT_EQ(p.getName(), name);

	// Verific?m banii de start (ar trebui s? fie 7)
	EXPECT_EQ(p.getCoins(), GameConstants::STARTING_COINS);

	// Verific?m scuturile militare
	EXPECT_EQ(p.getMilitaryShields(), 0);
}

TEST(PlayerTest, AddCoins) {
	Player p("Player1");
	int initial = p.getCoins();

	p.addCoins(5);
	EXPECT_EQ(p.getCoins(), initial + 5);
}

TEST(PlayerTest, RemoveCoinsProtection) {
	Player p("Player1");
	p.addCoins(100);

	// Încerc?m s? scoatem mai mult decât are
	p.removeCoins(1000);

	// Ar trebui s? r?mân? la 0, nu s? se duc? pe minus
	EXPECT_EQ(p.getCoins(), 0);
}

// Test?m GameController
TEST(GameControllerTest, StartGame) {
	MockGameView mockView;
	GameController controller(mockView);

	EXPECT_FALSE(controller.hasGameStarted());

	controller.startNewGame("Alice", "Bob");

	EXPECT_TRUE(controller.hasGameStarted());
}