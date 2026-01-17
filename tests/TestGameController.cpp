#include <gtest/gtest.h>
#include <fstream>
#include "GameController.h"
#include "GameState.h"
#include "MockGameView.h"
#include "Constants.h"

TEST(GameControllerTest, Initialization_And_StartNewGame) {
	MockGameView mockView;
	GameController controller(mockView);

	EXPECT_FALSE(controller.hasGameStarted());
	EXPECT_FALSE(controller.isGameOver());

	controller.startNewGame("Player1", "Player2");

	EXPECT_TRUE(controller.hasGameStarted());
	EXPECT_FALSE(controller.isGameOver());

	const GameState& state = controller.getGameState();
	EXPECT_EQ(state.getCurrentPhase(), GamePhase::DRAFTING);
	EXPECT_EQ(state.getPlayers()[0]->getName(), "Player1");
	EXPECT_EQ(state.getPlayers()[1]->getName(), "Player2");
}

TEST(GameControllerTest, Reset_Functionality) {
	MockGameView mockView;
	GameController controller(mockView);

	controller.startNewGame("Denisa", "Valentina");
	EXPECT_TRUE(controller.hasGameStarted());

	controller.reset();
	EXPECT_FALSE(controller.hasGameStarted());

	controller.startNewGame("Matei", "Iustin");
	EXPECT_TRUE(controller.hasGameStarted());
	EXPECT_EQ(controller.getGameState().getPlayers()[0]->getName(), "Matei");
	EXPECT_EQ(controller.getGameState().getPlayers()[1]->getName(), "Iustin");
}

TEST(GameControllerTest, StartNewGame_InitializesPlayersCorrectly) {
	MockGameView mockView;
	GameController controller(mockView);
	controller.startNewGame("Denisa", "Valentina");

	const GameState& state = controller.getGameState();
	const auto& players = state.getPlayers();;

	EXPECT_EQ(players[0]->getName(), "Denisa");
	EXPECT_EQ(players[1]->getName(), "Valentina");

	EXPECT_EQ(players[0]->getCoins(), 7);
	EXPECT_EQ(players[1]->getCoins(), 7);
	EXPECT_EQ(players[0]->getMilitaryShields(), 0);
	EXPECT_EQ(players[1]->getMilitaryShields(), 0);
	EXPECT_EQ(players[0]->getVictoryPoints(), 0);
	EXPECT_EQ(players[1]->getVictoryPoints(), 0);
}

TEST(GameControllerTest, SaveGame_CreatesFile) {
	MockGameView mockView;
	GameController controller(mockView);
	controller.startNewGame("Denisa", "Valentina");

	std::string filename = "test_save_game.json";

	controller.saveGame(filename);

	std::ifstream file(filename);
	EXPECT_TRUE(file.good());
	file.close();

	std::remove(filename.c_str());
}

TEST(GameControllerTest, LoadGame_RestoresState) {
	MockGameView mockView;
	GameController controller(mockView);
	controller.startNewGame("SavePlayer1", "SavePlayer2");

	std::string filename = "test_load_game.json";
	controller.saveGame(filename);

	GameController controller2(mockView);
	controller2.loadGame(filename);

	EXPECT_TRUE(controller2.hasGameStarted());
	EXPECT_EQ(controller2.getGameState().getPlayers()[0]->getName(), "SavePlayer1");
	EXPECT_EQ(controller2.getGameState().getPlayers()[1]->getName(), "SavePlayer2");
	EXPECT_EQ(controller2.getGameState().getCurrentPhase(), GamePhase::DRAFTING);

	std::remove(filename.c_str());
}