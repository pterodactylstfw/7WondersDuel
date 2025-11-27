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
