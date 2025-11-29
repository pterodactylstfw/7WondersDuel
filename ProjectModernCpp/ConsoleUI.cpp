#include "ConsoleUI.h"
#include "Utils.h"

void ConsoleUI::displayHeader() const
{
	system("cls");
	std::cout << "\n=======================================\n";
	std::cout << "       7 WONDERS DUEL - CONSOLE\n";
	std::cout << "=======================================\n";
}

void ConsoleUI::displayGameState() const
{
	const auto& state = m_game.getGameState();
	const auto& currentPlayer = state.getCurrentPlayer();
	const auto& opponent = state.getOpponent();

	std::cout << "\n\n";
	std::cout << "---------------- [ AGE " << (int)state.getCurrentAge() << " ] ----------------\n";

	std::cout << ">>> OPPONENT <<<\n";
	displayPlayer(opponent);

	// 2. Piramida (mijloc)
	/*std::cout << "\n            --- CARDS PYRAMID ---\n";
	displayPyramid();
	std::cout << "---------------------------------------\n";*/

	// 3. Jucătorul curent (partea de jos)
	std::cout << ">>> YOU (" << currentPlayer.getName() << ") <<<\n";
	displayPlayer(currentPlayer);
}

void ConsoleUI::displayPlayer(const Player& player) const
{
	std::cout << "   Name: " << player.getName();
	std::cout << " | Coins: " << player.getCoins();
	std::cout << " | Shields: " << player.getMilitaryShields();
	std::cout << " | Victory Points: " << player.getVictoryPoints();

	// Afișăm resursele (sumar)
	auto resources = player.getTotalResources();
	std::cout << "\n   Resources: ";
	if (resources.empty()) std::cout << "None";
	for (const auto& [type, qty] : resources)
		std::cout << resourceToString(type) << ":" << qty << "  ";
	
	/*
	std::cout << "\n   Wonders: ";

	const auto& wonders = player.getWonders();
	for (const auto& wonder : wonders)
	{
		if (wonder) {
			std::cout << wonder->getName();
			if (wonder->isBuilt()) {
				std::cout << " [BUILT] ";
			}
			else {
				std::cout << " (Cost: " << wonder->getCost().getCoinCost() << ") ";
			}
			std::cout << " | ";
		}
	}
	std::cout << "\n";
	*/

}

void ConsoleUI::displayAccesibleCards() const
{
	const auto& pyramid = m_game.getGameState().getPyramid();
	std::cout << "Pyramid Cards (Accessible): \n";
	for (const auto& node : pyramid)
	{
		if (!node.m_isRemoved && node.m_isFaceUp)
		{
			const auto* card = m_game.getGameState().getCardPtr(node.m_index);
			if (card)
			{
				std::cout << " - [" << node.m_index << "] " << card->displayCardInfo() << "\n";
			}
		}
	}
}

int ConsoleUI::showMainMenu()
{
	std::cout << "\n=======================================\n";
	std::cout << "               MAIN MENU\n";
	std::cout << "=======================================\n";

	std::cout << "1. Start new game\n";
	std::cout << "2. Load game\n";

	return Utils::getIntInput("Select an option: ");
}

int ConsoleUI::showActionMenu()
{
	std::cout << "1. Construct Building\n";
	std::cout << "2. Discard to obtain coins\n";
	std::cout << "3. Construct Wonder\n";

	return Utils::getIntInput("Select an option: ");
}

void ConsoleUI::showVictoryScreen(const std::string& winnerName)
{
	system("cls");
	std::cout << "\n=======================================\n";
	std::cout << "           GAME OVER\n";
	std::cout << "=======================================\n";
	std::cout << ">>> The winner is: " << winnerName << " <<<\n";
	std::cout << "Congratulations!\n";
	std::cout << "=======================================\n";
}

void ConsoleUI::run()
{
	int startOption = showMainMenu();
	switch (startOption)
	{
	case 1:
	{
		std::cout << "Enter player 1 name: ";
		std::string player1;
		std::cin >> player1;
		std::cin.ignore();

		std::cout << "Enter player 2 name: ";
		std::string player2;
		std::cin >> player2;
		std::cin.ignore();

		m_game.startNewGame(player1, player2);
		break;
	}

	case 2:
	{
		m_game.loadGame("test_savegame.json");
		break;
	}

	default:
		break;

	}
	// to continue
}
