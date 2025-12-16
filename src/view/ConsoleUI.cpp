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


		auto cardView = state.getCardView(node.m_index);
		if (cardView.has_value()) {
			const Card& card = cardView.value().get();
			std::cout << "   [" << std::right << std::setw(2) << node.m_index + 1 << "]  "
				<< std::left << std::setw(20) << card.getName()
				<< std::left << std::setw(20) << colorToString(card.getColor()) // de refacut cu cardCategory
				<< std::left << std::setw(20) << card.getCost().toShortString()
				<< card.getEffect().getDescription()
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
			auto cardView = m_game.getGameState().getCardView(node.m_index);
			if (cardView.has_value()) {
				const Card& card = cardView.value().get();
				std::cout << " - [" << node.m_index << "] " << card.displayCardInfo() << "\n";
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

    cardIndex--; // ajustare index 0-based

    // carte accesibila?
    if (!m_game.getGameState().isCardAccessible(cardIndex)) {
        std::cout << "[!] Error: Card is not accessible (blocked or face down).\n";
        return;
    }

    auto cardView = m_game.getGameState().getCardView(cardIndex);
    if (!cardView.has_value()) return;
    const Card& card = cardView.value().get();

	// Vom calcula aici costurile pentru a informa playerul - detasare de GameController
    const Player& me = m_game.getGameState().getCurrentPlayer();
    const Player& opp = m_game.getGameState().getOpponent();

    int totalCost = 0;
    int tradeCost = 0;
    bool canAfford = false;

    if (me.hasChainForCard(card)) {
        // E gratis prin lant
        canAfford = true;
        // totalCost ramane 0
    } else {
        totalCost = me.calculateTotalCost(card.getCost(), opp);
        tradeCost = me.calculateTradeCost(card.getCost(), opp);
        canAfford = me.canAfford(totalCost, opp);
    }

    int actionChoice = showCardActionMenu(card);

    if (actionChoice == 0) {
        std::cout << "Action cancelled.\n";
        return;
    }

    bool success = false;

    if (actionChoice == 1) { // CONSTRUCT BUILDING
        if (!canAfford) {
            std::cout << "\n[!] You do not have enough coins (" << me.getCoins() << ") to pay the cost (" << totalCost << ").\n";
            return;
        }

        // Intrebam playerul daca vrea sa plateasca tradeul
        if (confirmPurchaseInteraction(totalCost, tradeCost)) {
            success = m_game.executeAction(cardIndex, PlayerAction::CONSTRUCT_BUILDING);
        }
    }
    else if (actionChoice == 2) { // DISCARD
        success = m_game.executeAction(cardIndex, PlayerAction::DISCARD_FOR_COINS);
    }
    else if (actionChoice == 3) { // CONSTRUCT WONDER
        displayWonderBoard();
        int wonderIndex = Utils::getIntInput("Choose Wonder index (1-4) or 0 to cancel: ");
        if (wonderIndex == 0) return;
        wonderIndex--; // 0-based

        // Verificam minunea
        const auto& wonders = me.getWonders();
        if (wonderIndex >= 0 && wonderIndex < 4 && wonders[wonderIndex] && !wonders[wonderIndex]->isBuilt()) {

            // Calculam cost minune
            const auto& w = wonders[wonderIndex];
            int wTotalCost = me.calculateTotalCost(w->getCost(), opp);
            int wTradeCost = me.calculateTradeCost(w->getCost(), opp);

            if (me.getCoins() < wTotalCost) {
                 std::cout << "\n[!] Not enough coins for this Wonder.\n";
                 return;
            }

            if (confirmPurchaseInteraction(wTotalCost, wTradeCost)) {
                success = m_game.executeAction(cardIndex, PlayerAction::CONSTRUCT_WONDER, wonderIndex);
            }
        } else {
            std::cout << "[!] Invalid wonder selection.\n";
        }
    }

    if (success) {
        if (m_game.isGameOver())
        {
            const auto& state = m_game.getGameState();
            const auto& winner = state.getWinnerIndex();
            if (state.getCurrentPlayerIndex() == winner.value())
            {
                showVictoryScreen(state.getCurrentPlayer().getName());
            }
            else
            {
                showVictoryScreen(state.getOpponent().getName());
            }
        }
        std::cout << "\n>>> SUCCESS! Action completed. <<<\n";
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
			for (const auto& c : cards) {
				std::cout << c.get().getName() << ", ";
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

int ConsoleUI::showCardActionMenu(const Card& card)
{
	std::cout << "\n=======================================\n";
	std::cout << "   SELECTED CARD: " << card.getName() << "\n";
	std::cout << "=======================================\n";
	std::cout << "Cost: " << card.getCost().toString() << "\n";
	std::cout << "Effect: " << card.getEffect().getDescription() << "\n";

	const auto& player = m_game.getGameState().getCurrentPlayer();
	if (player.hasChainForCard(card)) {
		std::cout << ">> FREE CONSTRUCTION (Chain Link) <<\n";
	}

	std::cout << "\nWhat do you want to do?\n";
	std::cout << "1. Construct Building\n";
	std::cout << "2. Discard for Coins\n";
	std::cout << "3. Construct Wonder\n";
	std::cout << "0. CANCEL (Go back)\n";

	return Utils::getIntRange(0, 3, "Choose action: ");
}

bool ConsoleUI::confirmPurchaseInteraction(int totalCost, int tradeCost) {
	if (tradeCost == 0)
		return true;

	std::cout << "\n[!] Missing resources. You need to trade with the bank.\n";
	std::cout << "   -> Total Cost: " << totalCost << " coins\n";
	std::cout << "   -> (Includes Trade Cost: " << tradeCost << " coins)\n";

	std::cout << "Do you want to proceed?\n";
	std::cout << "1. Pay " << totalCost << " coins and Construct\n";
	std::cout << "2. Cancel (Go back)\n";

	int choice = Utils::getIntRange(1, 2, "Choose option: ");
	return (choice == 1);
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

ConsoleUI::ConsoleUI() : m_game(*this) {}

void ConsoleUI::onMessage(const std::string& message) {
    std::cout << message << "\n";
}

void ConsoleUI::onError(const std::string& error) {
    std::cerr << "[!] " << error << "\n";
}

void ConsoleUI::onStateUpdated() {
    // aici poate fi gol sau un simplu mesaj
}

int ConsoleUI::askInt(int min, int max, const std::string& prompt) {
    return Utils::getIntRange(min, max, prompt);
}

ResourceType ConsoleUI::askResourceSelection(const std::vector<ResourceType>& options, const std::string& prompt) {
    std::cout << prompt << "\n";
    for (size_t i = 0; i < options.size(); ++i) {
        std::cout << i + 1 << ". " << resourceToString(options[i]) << "\n";
    }
    int choice = Utils::getIntRange(1, static_cast<int>(options.size()), "Choose resource: ");
    return options[choice - 1];
}

int ConsoleUI::askWonderSelection(const std::vector<std::unique_ptr<Wonder>>& wonders, const std::string& playerName) {
    std::cout << "\n" << playerName << ", select a Wonder:\n";
    for (size_t i = 0; i < wonders.size(); ++i) {
        std::cout << i + 1 << ". " << wonders[i]->getName() << "\n";
    }
    int choice = Utils::getIntRange(1, static_cast<int>(wonders.size()), "Select Wonder: ");
    return choice - 1; // 0-based index
}

int ConsoleUI::askTokenSelection(const std::vector<std::unique_ptr<ProgressToken>>& tokens, const std::string& prompt) {
    std::cout << "\n" << prompt << "\n";
    for (size_t i = 0; i < tokens.size(); ++i) {
        std::cout << i + 1 << ". " << tokens[i]->getName() << "\n";
    }
    int choice = Utils::getIntRange(1, static_cast<int>(tokens.size()), "Select Token: ");
    return choice - 1;
}

int ConsoleUI::askCardSelectionFromList(const std::vector<std::reference_wrapper<const Card>>& cards, const std::string& prompt) {
    std::cout << "\n" << prompt << "\n";
    for (size_t i = 0; i < cards.size(); ++i) {
        std::cout << i + 1 << ". " << cards[i].get().getName() << "\n";
    }
    int choice = Utils::getIntRange(1, static_cast<int>(cards.size()), "Select Card: ");
    return choice - 1;
}
