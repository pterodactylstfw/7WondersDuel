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

	displayMilitaryTrack();

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

	std::cout << "\n   [ID]  CARD NAME         CARD COLOR            COST                 EFFECT\n";
	std::cout << "   ----------------------------------------------------------------------------\n";

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
				<< std::left << std::setw(20) << colorToString(card->getColor()) // de refacut cu cardCategory
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

void ConsoleUI::displayMilitaryTrack() const
{
	const auto& state = m_game.getGameState();
	const auto& me = state.getCurrentPlayer();
	const auto& opp = state.getOpponent();

	int balance = me.getMilitaryShields() - opp.getMilitaryShields();

	if (balance > 9) balance = 9;
	if (balance < -9) balance = -9;

	std::cout << "\n   >>> MILITARY TRACK <<<\n";

	std::cout << "   " << me.getName() << " (LOSE) ";

	for (int i = -9; i <= 9; ++i)
	{
		if (i == balance) {
			std::cout << "[x]";
		}
		else if (i == 0) {
			std::cout << "[0]";
		}
		else {
			std::cout << "[ ]";
		}
	}

	std::cout << " " << opp.getName() << " (WIN) \n";

	std::cout << "   Current Balance: " << balance << "\n";
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

void ConsoleUI::handlePlayCard()
{
	
	int cardIndex = Utils::getIntInput("\n>>> Enter card index from pyramid (-1 to cancel): ");
	if (cardIndex == -1) return;

	cardIndex--; // din motive de 0-based index

	// carte accessibila?
	if (!m_game.getGameState().isCardAccessible(cardIndex)) {
		std::cout << "[!] Error: Card is not accessible (blocked or face down).\n";
		return;
	}

	const Card* card = m_game.getGameState().getCardPtr(cardIndex);
	if (!card) return;

	int actionChoice = showCardActionMenu(card);

	if (actionChoice == 0) {
		std::cout << "Action cancelled.\n";
		return;
	}


	bool success = false;

	if (actionChoice == 1) {
		success = m_game.executeAction(cardIndex, PlayerAction::CONSTRUCT_BUILDING);
	}
	else if (actionChoice == 2) {
		success = m_game.executeAction(cardIndex, PlayerAction::DISCARD_FOR_COINS);
	}
	else if (actionChoice == 3) {
		displayWonderBoard();
		int wonderIndex = Utils::getIntInput("Choose Wonder index (1-4) or 0 to cancel: ");
		if (wonderIndex == 0) return;

		success = m_game.executeAction(cardIndex, PlayerAction::CONSTRUCT_WONDER, wonderIndex - 1);
	}

	if (success) {
		std::cout << "\n>>> SUCCESS! Action completed. <<<\n";
	}
	else {
		std::cout << "\n[!] FAILED: Insufficient resources or invalid move.\n";
	}
}

void ConsoleUI::displayCityDetails() {
	const auto& player = m_game.getGameState().getCurrentPlayer();

	std::cout << "\n================ [ CITY DETAILS: " << player.getName() << " ] ================\n";

	// sumar resurse
	std::cout << ">>> RESOURCES:\n";
	
	std::string_view resDesc = player.getResourceDescription();
	if (resDesc.empty()) {
		std::cout << "None";
	}
	else {
		std::cout << resDesc;
	}

	std::cout << "\n";

	std::cout << "   Coins: " << player.getCoins() << "\n";

	// minuni constr
	std::cout << "\n>>> CONSTRUCTED WONDERS:\n";
	const auto& builtWonders = player.getConstructedWonders();
	if (builtWonders.empty()) std::cout << "   (None)\n";
	for (const auto& w : builtWonders)
		{
		std::cout << "   - " << w->getName() << "\n";
	}

	// carti construite pe categ
	std::cout << "\n>>> CONSTRUCTED BUILDINGS:\n";

	// lambda pt afisare carti pe culoare
	auto printColor = [&](CardColor color, std::string label) {
		auto cards = player.getCardsOfType(color);
		if (!cards.empty()) {
			std::cout << "   " << label << ": ";
			for (const auto* c : cards) {
				std::cout << c->getName() << ", ";
			}
			std::cout << "\n";
		}
		};

	printColor(CardColor::BROWN, "Raw Materials (Brown)");
	printColor(CardColor::GREY, "Manufactured (Grey)");
	printColor(CardColor::BLUE, "Civilian (Blue)");
	printColor(CardColor::GREEN, "Scientific (Green)");
	printColor(CardColor::YELLOW, "Commercial (Yellow)");
	printColor(CardColor::RED, "Military (Red)");
	printColor(CardColor::PURPLE, "Guilds (Purple)");

	// progress tokens
	std::cout << "\n>>> PROGRESS TOKENS:\n ...";
	if (player.getProgressTokens().empty()) std::cout << "   (None)\n";
	for(const auto& token : player.getProgressTokens())
	{
		std::cout << "   - " << token->getName() << "\n";
	}

	std::cout << "\n================================================================\n";
	Utils::getStringInput("Press Enter to return to map...");
}

int ConsoleUI::showHighLevelMenu() {
	std::cout << "\n[ YOUR TURN, "<< m_game.getGameState().getCurrentPlayer().getName()<<" ] What do you want to do ? \n";
	std::cout << "1. Pick a card to play\n";
	std::cout << "2. View my city details (Cards, Tokens)\n";
	std::cout << "3. Save & Exit to Main Menu\n";

	return Utils::getIntRange(1, 3, "Select option: ");
}

int ConsoleUI::showCardActionMenu(const Card* card)
{
	std::cout << "\n=======================================\n";
	std::cout << "   SELECTED CARD: " << card->getName() << "\n";
	std::cout << "=======================================\n";
	std::cout << "Cost: " << card->getCost().toString() << "\n";
	std::cout << "Effect: " << card->getEffect().getDescription() << "\n";

	const auto& player = m_game.getGameState().getCurrentPlayer();
	if (player.hasChainForCard(*card)) {
		std::cout << ">> FREE CONSTRUCTION (Chain Link) <<\n";
	}

	std::cout << "\nWhat do you want to do?\n";
	std::cout << "1. Construct Building\n";
	std::cout << "2. Discard for Coins\n";
	std::cout << "3. Construct Wonder\n";
	std::cout << "0. CANCEL (Go back)\n";

	return Utils::getIntRange(0, 3, "Choose action: ");
}

void ConsoleUI::run()
{
	while (true)
	{
		int startOption = showMainMenu();

		if (startOption == 0) {
			std::cout << "Goodbye!\n";
			return;
		}

		if (startOption == 1) {
			std::cout << "Enter player 1 name: ";
			std::string player1;
			std::getline(std::cin, player1);
			if (player1.empty()) std::getline(std::cin, player1);

			std::cout << "Enter player 2 name: ";
			std::string player2;
			std::getline(std::cin, player2);

			m_game.startNewGame(player1, player2);
		}
		else if (startOption == 2) {
			std::cout << "Save filename: (ex: savegame.json): ";
			std::string filename;
			std::getline(std::cin, filename);
			try {
				m_game.loadGame(filename);
				std::cout << ">>> Game successfully loaded! <<<\n";
			}
			catch (...) {
				std::cout << ">>> ERROR: Game couldn't be loaded.\n";
				continue;
			}
		}

		bool gameIsRunning = true;

		while (gameIsRunning && !m_game.isGameOver())
		{
			displayGameState();

			int choice = showHighLevelMenu();

			switch (choice) {
			case 1:
				handlePlayCard();
				break;

			case 2:
				displayCityDetails();
				break;

			case 3:
				std::cout << "Do you want to save before exiting? (1=Yes, 0=No): ";
				int save = Utils::getIntInput("");
				if (save == 1) {
					std::cout << "Save file name: ";
					std::string saveFile;
					std::getline(std::cin, saveFile);
					m_game.saveGame(saveFile);
					std::cout << ">>> Game Saved! <<<\n";
				}
				gameIsRunning = false;
				break;
			}
		}


		if (m_game.isGameOver()) {
			// Logica ta de victorie (neschimbată)
			const auto& state = m_game.getGameState();
			const auto& player1 = state.getCurrentPlayer(); // Atenție la logica de winner
			// ... (restul codului tău pentru winner) ...
			// showVictoryScreen(winnerName);
		}
	}
}