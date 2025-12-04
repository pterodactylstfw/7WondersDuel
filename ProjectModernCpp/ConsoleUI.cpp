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
	
	// Adversarul
	std::cout << ">>> OPPONENT: "<<opponent.getName()<<"\n";
	displayPlayer(opponent);

	// Piramida
	std::cout << "\n            --- CARDS PYRAMID ---\n";
	displayPyramid();
	std::cout << "---------------------------------------\n";

	// Jucatorul curent
	std::cout << ">>> YOU (" << currentPlayer.getName() << ") <<<\n";
	displayPlayer(currentPlayer);
}

void ConsoleUI::displayPlayer(const Player& player) const
{
	std::cout << "   Name: " << player.getName();
	std::cout << " | Coins: " << player.getCoins();
	std::cout << " | Shields: " << player.getMilitaryShields();
	std::cout << " | Victory Points: " << player.getVictoryPoints();

	// afis resurse
	auto resources = player.getTotalResources();
	std::cout << "\n   Resources: ";
	if (resources.empty()) std::cout << "None";
	for (const auto& [type, qty] : resources)
		std::cout << resourceToString(type) << ":" << qty << "  ";
	
	std::cout << "\n   Wonders:\n";

	// minuni neconstruite
	const auto& unbuiltWonders = player.getWonders();
	for (const auto& wonder : unbuiltWonders)
	{
		if (wonder) { // verif daca minunea exista
			std::cout << "     [ ] " << wonder->getName()
				<< " (Cost: " << wonder->getCost().toShortString() << ")\n";
		}
	}

	// afis minuni construite trimise in vectorul de minuni constr
	const auto& builtWonders = player.getConstructedWonders();
	for (const auto& wonder : builtWonders)
	{
		if (wonder) {
			std::cout << "     [X] " << wonder->getName() << " [BUILT]\n";
		}
	}

	std::cout << "\n";
}

void ConsoleUI::displayPyramid() const
{
	const auto& state = m_game.getGameState();
	const auto& pyramidNodes = state.getPyramid();

	std::cout << "\n   [ID]  CARD NAME            COST                 EFFECT\n";
	std::cout << "   ----------------------------------------------------------------------\n";

	bool foundAny = false;

	for (const auto& node : pyramidNodes)
	{

		if (node.m_isRemoved) {
			continue;
		}


		if (!state.isCardAccessible(node.m_index)) {
			continue;
		}


		const Card* card = state.getCardPtr(node.m_index);
		if (card) {

			std::cout << "   [" << std::right << std::setw(2) << node.m_index + 1 << "]  "
				<< std::left << std::setw(20) << card->getName()
				<< std::left << std::setw(20) << card->getCost().toShortString()
				<< card->getEffect().getDescription()
				<< "\n";
			foundAny = true;
		}
	}

	if (!foundAny) {
		std::cout << "   (No accessible cards right now.)\n";
	}
	std::cout << "\n";
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

void ConsoleUI::displayWonderBoard() const
{
	const auto& player = m_game.getGameState().getCurrentPlayer();
	const auto& wonders = player.getWonders();

	std::cout << "\n--- CHOOSE A WONDER TO BUILD ---\n";

	for (size_t i = 0; i < wonders.size(); ++i)
	{
		// afis doar minunile neconstruite si disponibile
		if (wonders[i] && !wonders[i]->isBuilt())
		{
			std::cout << "[" << i + 1 << "] " << wonders[i]->getName()
				<< " -- Cost: " << wonders[i]->getCost().toString() << "\n";
		}
	}
	std::cout << "----------------------------------------\n";
}


int ConsoleUI::showMainMenu()
{
	std::cout << "\n=======================================\n";
	std::cout << "               MAIN MENU\n";
	std::cout << "=======================================\n";

	std::cout << "0. Exit\n";
	std::cout << "1. Start new game\n";
	std::cout << "2. Load game\n";

	return Utils::getIntInput("Select an option: ");
}

int ConsoleUI::showActionMenu()
{
	std::cout << "0. Exit to Main Menu\n";
	std::cout << "1. Construct Building\n";
	std::cout << "2. Discard to obtain coins\n";
	std::cout << "3. Construct Wonder\n";
	std::cout << "4. Save Game\n";

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
	while (true)
	{
		// main menu
		int startOption = showMainMenu();

		if (startOption == 0) { // Ieșire (trebuie să adaugi opțiunea 0 în showMainMenu dacă nu există)
			std::cout << "Goodbye!\n";
			return;
		}

		if (startOption == 1)
		{
			
			std::cout << "Enter player 1 name: ";
			std::string player1;
			std::getline(std::cin, player1);
			if (player1.empty()) std::getline(std::cin, player1); //buffer posibil ramas 

			std::cout << "Enter player 2 name: ";
			std::string player2;
			std::getline(std::cin, player2);

			m_game.startNewGame(player1, player2);
		}
		else if (startOption == 2)
		{
			// Load Game
			std::cout << "Save filename: (ex: savegame.json): ";
			std::string filename;
			std::getline(std::cin, filename);
			try {
				m_game.loadGame(filename);
				std::cout << ">>> Game successfully loaded! <<<\n";
			}
			catch (...) {
				std::cout << ">>> ERROR: Game couldn't be loaded.\n";
				continue; // Revino la meniu
			}
		}

		// game loop
		bool gameIsRunning = true;

		while (gameIsRunning && !m_game.isGameOver())
		{
			// afis stare curenta
			displayGameState();

			// meniu actiuni
			int actionChoice = showActionMenu();

			if (actionChoice == 0) { // exit to main menu
				gameIsRunning = false;
				break;
			}
			if (actionChoice == 4) { // Save
				std::cout << "Save file name: ";
				std::string saveFile;
				std::getline(std::cin, saveFile);
				m_game.saveGame(saveFile);
				std::cout << ">>> Saved game! <<<\n";
				continue; // Nu schimbăm tura, reluăm bucla
			}

			// C. Input specific acțiunii
			int cardIndex = Utils::getIntInput("Enter card index from pyramid: ");
			cardIndex--; // pentru ca am afisarea cu index+1
			bool success = false;

			if (actionChoice == 1) { // constr building
				success = m_game.executeAction(cardIndex, PlayerAction::CONSTRUCT_BUILDING);
			}
			else if (actionChoice == 2) { // discard
				success = m_game.executeAction(cardIndex, PlayerAction::DISCARD_FOR_COINS);
			}
			else if (actionChoice == 3) { // constr wonder
				if (!m_game.getGameState().isCardAccessible(cardIndex)) {
					std::cout << "[!] ERROR: Selected card isn't available, therefore you can't build a Wonder with it.\n";
					continue; // Sărim peste restul
				}
				displayWonderBoard(); // minuni disponibile
				int wonderIndex = Utils::getIntInput("Choose a Wonder (1-4): ");
				success = m_game.executeAction(cardIndex, PlayerAction::CONSTRUCT_WONDER, wonderIndex);
			}

			// raspuns
			if (success) {
				std::cout << "\n>>> Successfully done action! <<<\n";
				// system("pause"); // feature optional - sa astept enter inainte de a reafisa starea jocului
			}
			else {
				std::cout << "\n[!] ERROR: Invalid move (Insufficient resources / Blocked card) \n";
			}
		}

		// verif final joc
		if (m_game.isGameOver()) {
			// afisare castigator
			const auto& state = m_game.getGameState();
			const auto& player1 = state.getCurrentPlayer();
			const auto& player2 = state.getOpponent();
			std::string winnerName;
			if (state.getWinnerIndex().has_value()) {
				winnerName = (state.getWinnerIndex().value() == 0) ? player1.getName() : player2.getName();
			}
			else {
				// egalitate - decidem dupa puncte
				int score1 = player1.getFinalScore(player2);
				int score2 = player2.getFinalScore(player1);
				if (score1 > score2) winnerName = player1.getName();
				else if (score2 > score1) winnerName = player2.getName();
				else winnerName = "No one! It's a tie!";
			}
			
		} // de revizuit
	}
}
