#include "ConsoleUI.h"
#include "Utils.h"
#include <print>

void ConsoleUI::displayHeader() const
{
	Utils::clearScreen();

	std::println("");
	std::println("=======================================");
	std::println("       7 WONDERS DUEL - CONSOLE");
	std::println("=======================================");
}

void ConsoleUI::displayGameState() const
{
	const auto& state = m_game.getGameState();

	if (state.getCurrentPhase() == GamePhase::DRAFTING) {
		displayDraftingPhase();
		return;
	}

	const auto& currentPlayer = state.getCurrentPlayer();
	const auto& opponent = state.getOpponent();

	displayHeader();

	std::println("\n");
	std::println("---------------- [ AGE {} ] ----------------", static_cast<int>(state.getCurrentAge()));

	// Adversarul
	std::println(">>> OPPONENT: {}", opponent.getName());
	displayPlayer(opponent);

	displayMilitaryTrack();

	// Piramida
	std::println("\n            --- CARDS PYRAMID ---");
	displayPyramid();
	std::println("---------------------------------------");

	// Jucatorul curent
	std::println(">>> YOU ({}) <<<", currentPlayer.getName());
	displayPlayer(currentPlayer);
}

void ConsoleUI::displayPlayer(const Player& player) const
{
	std::print("   Name: {} | ", player.getName());
	std::print("Coins: {} | ", player.getCoins());
	std::print("Shields: {} | ", player.getMilitaryShields());
	std::println("Victory Points: {}", player.getVictoryPoints());

	// afis resurse
	auto resources = player.getTotalResources();
	std::string resString;
	if (resources.empty()) resString = "None";
	for (const auto& [type, qty] : resources)
		resString += std::format("{}:{}  ", resourceToString(type), qty);

	std::println("   Resources: {}", resString);

	std::println("\n   Wonders:");

	// minuni neconstruite
	const auto& unbuiltWonders = player.getWonders();
	for (const auto& wonder : unbuiltWonders)
	{
		if (wonder) { // verif daca minunea exista
			std::println("     [ ] {} (Cost: {})", wonder->getName(), wonder->getCost().toShortString());
		}
	}

	// afis minuni construite trimise in vectorul de minuni constr
	const auto& builtWonders = player.getConstructedWonders();
	for (const auto& wonder : builtWonders)
	{
		if (wonder) {
			std::println("     [X] {} [BUILT]", wonder->getName());
		}
	}

	std::println("");
}

void ConsoleUI::displayPyramid() const
{
	const auto& state = m_game.getGameState();
	const auto& pyramidNodes = state.getPyramid();

	std::println("\n   [ID]  CARD NAME           COST                 EFFECT");
	std::println("   ----------------------------------------------------------------------------");

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

			std::string colorCode = Colors::RESET;
			switch(card.getColor()) {
				case CardColor::RED: colorCode = Colors::RED; break;
				case CardColor::GREEN: colorCode = Colors::GREEN; break;
				case CardColor::BLUE: colorCode = Colors::BLUE; break;
				case CardColor::YELLOW: colorCode = Colors::YELLOW; break;
				case CardColor::BROWN: colorCode = Colors::BROWN; break;
				case CardColor::GREY: colorCode = Colors::GREY; break;
				case CardColor::PURPLE: colorCode = Colors::MAGENTA; break;
				default: break;
			}

			std::cout << "   [" << std::right << std::setw(2) << node.m_index + 1 << "]  "
				<< colorCode << std::left << std::setw(20) << card.getName()<< Colors::RESET
				<< std::left << std::setw(20) << card.getCost().toShortString()
				<< card.getEffect().getDescription()
				<< "\n";
			foundAny = true;
		}
	}

	if (!foundAny) {
		std::println("   (No accessible cards right now.)");
	}
	std::println("");
}

void ConsoleUI::displayMilitaryTrack() const
{
	const auto& state = m_game.getGameState();
	const auto& me = state.getCurrentPlayer();
	const auto& opp = state.getOpponent();

	int balance = me.getMilitaryShields() - opp.getMilitaryShields();

	if (balance > 9) balance = 9;
	if (balance < -9) balance = -9;

	std::println("\n   >>> MILITARY TRACK <<<");

	std::print("   {} (LOSE) ", me.getName());

	for (int i = -9; i <= 9; ++i)
	{
		if (i == balance) std::print("[x]");
		else if (i == 0) std::print("[0]");
		else std::print("[ ]");
	}

	std::println(" {} (WIN) ", opp.getName());
	std::println("   Current Balance: {}", balance);
}

void ConsoleUI::displayAccessibleCards() const
{
	const auto& pyramid = m_game.getGameState().getPyramid();
	std::println("Pyramid Cards (Accessible):");
	for (const auto& node : pyramid)
	{
		if (!node.m_isRemoved && node.m_isFaceUp)
		{
			auto cardView = m_game.getGameState().getCardView(node.m_index);
			if (cardView.has_value()) {
				const Card& card = cardView.value().get();
				std::println(" - [{}] {}", node.m_index + 1, card.displayCardInfo());
			}
		}
	}
}

void ConsoleUI::displayDraftingPhase() const
{
	displayHeader();
	const auto& state = m_game.getGameState();
	const auto& player = state.getCurrentPlayer();
	const auto& draftedWonders = state.getDraftedWonders();

	std::println("\n========== WONDER DRAFTING PHASE ==========");
	std::println("It is {}'s turn to pick a Wonder.", player.getName());
	std::println("-------------------------------------------");

	int idx = 1;
	for (const auto& w : draftedWonders) {
		if (w) {
			std::println("{}. {} (Cost: {})", idx, w->getName(), w->getCost().toShortString());
		}
		idx++;
	}
	std::println("-------------------------------------------");
}

void ConsoleUI::displayWonderBoard() const
{
	const auto& player = m_game.getGameState().getCurrentPlayer();
	const auto& wonders = player.getWonders();

	std::println("\n--- CHOOSE A WONDER TO BUILD ---");

	for (size_t i = 0; i < wonders.size(); ++i)
	{
		// afis doar minunile neconstruite si disponibile
		if (wonders[i] && !wonders[i]->isBuilt())
		{
			std::println("[{}] {} -- Cost: {}", i + 1,
				wonders[i]->getName(), wonders[i]->getCost().toString());
		}
	}
	std::println("----------------------------------------");
}


int ConsoleUI::showMainMenu()
{
	std::println("\n=======================================");
	std::println("               MAIN MENU");
	std::println("=======================================");

	std::println("0. Exit");
	std::println("1. Start new game");
	std::println("2. Load game");

	return Utils::getIntInput("Select an option: ");
}

int ConsoleUI::showActionMenu()
{
	std::println("0. Exit to Main Menu");
	std::println("1. Construct Building");
	std::println("2. Discard to obtain coins");
	std::println("3. Construct Wonder");
	std::println("4. Save Game");

	return Utils::getIntInput("Select an option: ");
}

void ConsoleUI::showVictoryScreen(const std::string& winnerName)
{
	const auto& state = m_game.getGameState();
	int winnerIndex = state.getWinnerIndex().value_or(-1);

	std::string victoryType = "GAME OVER";

	if (std::abs(state.getMilitaryPosition()) >= GameConstants::MILITARY_SUPREMACY_DISTANCE) {
		victoryType = "MILITARY VICTORY!";
	}
	else if (winnerIndex != -1 && state.getPlayers()[winnerIndex]->hasScientificVictory()) {
		victoryType = "SCIENTIFIC VICTORY!";
	}
	else if (winnerIndex != -1) {
		victoryType = "CIVILIAN VICTORY!";
	}
	else {
		victoryType = "GAME ENDED (DRAW)";
	}

	Utils::clearScreen();
	std::println("\n=======================================");
	std::println("           {}           ", victoryType);
	std::println("=======================================");
	std::println(">>> The winner is: {} <<<", winnerName);

	if (victoryType == "CIVILIAN VICTORY!" && winnerIndex != -1) {
		const auto& winner = state.getPlayers()[winnerIndex];
		const auto& loser = state.getPlayers()[1 - winnerIndex];
		int winnerScore = winner->getFinalScore(*loser);
		int loserScore = loser->getFinalScore(*winner);
		std::println("{}'s Score: {} points", winner->getName(), winnerScore);
		std::println("{}'s Score: {} points", loser->getName(), loserScore);
	}

	std::println("Congratulations!");
	std::println("=======================================");

	Utils::waitForEnter("Press Enter to return to Main Menu...");
}


void ConsoleUI::handlePlayCard()
{

    int cardIndex = Utils::getIntInput("\n>>> Enter card index from pyramid (-1 to cancel): ");
    if (cardIndex == -1) return;

    cardIndex--; // ajustare index 0-based

    // carte accesibila?
    if (!m_game.getGameState().isCardAccessible(cardIndex)) {
        std::println("[!] Error: Card is not accessible (blocked or face down).");
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
        std::println("Action cancelled.");
        return;
    }

    bool success = false;

    if (actionChoice == 1) { // CONSTRUCT BUILDING
        if (!canAfford) {
        	std::println("{}[!] You do not have enough coins ( {} << ) to pay the cost ( {} ).", '\n', me.getCoins(), totalCost);
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
                 std::println("{}[!] Not enough coins for this Wonder.", '\n');
                 return;
            }

            if (confirmPurchaseInteraction(wTotalCost, wTradeCost)) {
                success = m_game.executeAction(cardIndex, PlayerAction::CONSTRUCT_WONDER, wonderIndex);
            }
        } else {
            std::println("[!] Invalid wonder selection.");
        }
    }

    if (success) {
        std::println("{}>>> SUCCESS! Action completed. <<<", '\n');
    }
}

void ConsoleUI::displayCityDetails() {
	const auto& player = m_game.getGameState().getCurrentPlayer();

	std::println("{}================ [ CITY DETAILS:  {}  ] ================", '\n', player.getName());

	// sumar resurse
	std::println(">>> RESOURCES:");

	std::string resDesc = player.getResourceDescription();
	if (resDesc.empty()) {
		std::println("None");
	}
	else {
		std::println("{}", resDesc);
	}

	std::println();

	std::println("   Coins:   {} << ", player.getCoins());

	// minuni constr
	std::println("{}>>> CONSTRUCTED WONDERS:", '\n');
	const auto& builtWonders = player.getConstructedWonders();
	if (builtWonders.empty()) std::println("   (None)");
	for (const auto& w : builtWonders)
		{
		std::println("   - {}", w->getName());
	}

	// carti construite pe categ
	std::println("{}>>> CONSTRUCTED BUILDINGS:", '\n');

	// lambda pt afisare carti pe culoare
	auto printColor = [&](CardColor color, std::string label) {
		auto cards = player.getCardsOfType(color);
		if (!cards.empty()) {
			std::println("   {}: ", label);
			for (const auto& c : cards) {
				std::println("{} , ", c.get().getName());
			}
			std::println();
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
	std::println("\n>>> PROGRESS TOKENS:");
	if (player.getProgressTokens().empty()) std::println("   (None)");
	for(const auto& token : player.getProgressTokens())
	{
		std::println("   - {}", token->getName());
	}

	std::println("\n================================================================");
	Utils::waitForEnter("Press Enter to return to map...");
}

int ConsoleUI::showHighLevelMenu() {
	std::println("\n[ YOUR TURN, {} ] What do you want to do ?", m_game.getGameState().getCurrentPlayer().getName());
	std::println("1. Pick a card to play");
	std::println("2. View my city details (Cards, Tokens)");
	std::println("3. Save & Exit to Main Menu");

	return Utils::getIntRange(1, 3, "Select option: ");
}

int ConsoleUI::showCardActionMenu(const Card& card)
{
	std::println("\n=======================================");
	std::println("   SELECTED CARD: {}", card.getName());
	std::println("=======================================");
	std::println("Cost: {}", card.getCost().toString());
	std::println("Effect: {}", card.getEffect().getDescription());

	const auto& player = m_game.getGameState().getCurrentPlayer();
	if (player.hasChainForCard(card)) {
		std::println(">> FREE CONSTRUCTION (Chain Link) <<");
	}

	std::println("\nWhat do you want to do?");
	std::println("1. Construct Building");
	std::println("2. Discard for Coins");
	std::println("3. Construct Wonder");
	std::println("0. CANCEL (Go back)");

	return Utils::getIntRange(0, 3, "Choose action: ");
}

bool ConsoleUI::confirmPurchaseInteraction(int totalCost, int tradeCost) {
	if (tradeCost == 0)
		return true;

	std::println("\n[!] Missing resources. You need to trade with the bank.");
	std::println("   -> Total Cost: {} coins", totalCost);
	std::println("   -> (Includes Trade Cost: {} coins)", tradeCost);

	std::println("Do you want to proceed?");
	std::println("1. Pay {} coins and Construct", totalCost);
	std::println("2. Cancel (Go back)");

	int choice = Utils::getIntRange(1, 2, "Choose option: ");
	return (choice == 1);
}

ConsoleUI::ConsoleUI() : m_game(*this) {}

void ConsoleUI::run()
{
	while (true)
	{
		int startOption = showMainMenu();

		if (startOption == 0) {
			std::println("Goodbye!");
			return;
		}

		bool gameIsRunning = false;

		if (startOption == 1) {
			std::println("(Player names must be 3-15 characters, alphanumeric only)");

			std::string regexName = "^[a-zA-Z0-9 ]{3,15}$";
			std::string errorMsg = "Name must be 3-15 characters, alphanumeric only.";

			std::string player1 = Utils::getStringInput("Player 1 name: ", regexName, errorMsg);
			std::string player2 = Utils::getStringInput("Player 2 name: ", regexName, errorMsg);

			m_game.startNewGame(player1, player2);
			gameIsRunning = true;

		}
		else if (startOption == 2) {
			std::string regexFile = "^.+\\.json$";
			std::string errorFile = "Filename must end with .json";

			std::string filename = Utils::getStringInput("Save filename (e.g. save.json): "
				, regexFile, errorFile);
			try {
				m_game.loadGame(filename);
				std::println(">>> Game successfully loaded! <<<");
				gameIsRunning = true;
				Utils::waitForEnter();
			}
			catch (...) {
				std::println(">>> ERROR: Game couldn't be loaded.");
				continue;
			}
		}

		while (gameIsRunning && !m_game.isGameOver())
		{
			displayGameState();

			if (m_game.getGameState().getCurrentPhase() == GamePhase::DRAFTING)
			{
				int count = m_game.getGameState().getDraftedWonders().size();

				int choice = Utils::getIntRange(1, count, "Select a wonder index to keep: ");

				bool ok = m_game.pickWonder(choice - 1);

				if (!ok) {
					std::println("[!] Error picking wonder. Try again.");
					Utils::waitForEnter();
				}
			}

			else {

				int choice = showHighLevelMenu();

				switch (choice) {
				case 1:
					handlePlayCard();
					break;

				case 2:
					displayCityDetails();
					break;

				case 3:
					std::string regexFile = "^.+\\.json$";
					std::string errorFile = "Filename must end with .json";

					std::print("Do you want to save before exiting? (1=Yes, 0=No): ");
					int save = Utils::getIntInput("");

					if (save == 1) {
						std::string saveFile = Utils::getStringInput("Save file name: ",
							regexFile, errorFile);
						m_game.saveGame(saveFile);
						std::println(">>> Game Saved! <<<");
					}
					gameIsRunning = false;
					break;
				}
			}
		}

		if (m_game.isGameOver()) {
			// logica victorie
			const auto& state = m_game.getGameState();
			if (state.getWinnerIndex().has_value()) {
				int winnerIndex = state.getWinnerIndex().value();
				std::string winnerName = (winnerIndex == 0) ?
				state.getCurrentPlayer().getName() : state.getOpponent().getName();
				showVictoryScreen(winnerName);
			}

			else
				showVictoryScreen("Unknown...");
		}
	}
}


void ConsoleUI::onMessage(const std::string& message) {
	std::println("[INFO] {}", message);
}

void ConsoleUI::onError(const std::string& error) {
	std::cerr << "[ERROR] " << error << "\n";
	Utils::waitForEnter("Press Enter to continue...");
}

void ConsoleUI::onStateUpdated() {
    // ramane goala intentionat 
}

int ConsoleUI::askInt(int min, int max, const std::string& prompt) {
    return Utils::getIntRange(min, max, prompt);
}

ResourceType ConsoleUI::askResourceSelection(const std::vector<ResourceType>& options, const std::string& prompt) {
	int index = Utils::getUserSelection(options,
		prompt,
		[](const ResourceType& r) { return resourceToString(r); }
	);
	return options[index];
}

int ConsoleUI::askWonderSelection(const std::array<std::unique_ptr<Wonder>, GameConstants::WONDERS_PER_PLAYER>& wonders, const std::string& playerName) {
	std::vector<const Wonder*> validWonders;
	std::vector<int> originalIndices;

	for (size_t i = 0; i < wonders.size(); ++i) {
		if (wonders[i]) {
			validWonders.push_back(wonders[i].get());
			originalIndices.push_back(i);
		}
	}

	if (validWonders.empty()) {
		std::println("[!] No wonders available to select.");
		return -1;
	}

	int selection = Utils::getUserSelection(validWonders,
		std::format("{}, select a Wonder:", playerName),
		[](const Wonder* w) { return w->getName(); }
	);

	if (selection >= 0 && selection < originalIndices.size()) {
		return originalIndices[selection];
	}

	return -1;
}

int ConsoleUI::askTokenSelection(const std::vector<std::unique_ptr<ProgressToken>>& tokens, const std::string& prompt) {
	return Utils::getUserSelection(tokens,
		prompt,
		[](const auto& t) { return t->getName(); }
	);
}

int ConsoleUI::askCardSelectionFromList(const std::vector<std::reference_wrapper<const Card>>& cards, const std::string& prompt) {
	return Utils::getUserSelection(cards,
		prompt,
		[](const auto& c) { return c.get().getName(); }
	);
}
