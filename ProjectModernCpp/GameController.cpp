#include "GameController.h"

const GameState& GameController::getGameState() const {
	return *m_gameState;
}

bool GameController::isGameOver() const {
	return m_gameState->isGameOver();
}

void GameController::startNewGame(const std::string& p1, const std::string& p2) {
	m_gameState = std::make_unique<GameState>(p1, p2);

	selectWondersManual();
	prepareAge(1);
}

void GameController::draftWondersAuto() {
	WonderFactory wf;

	auto allWonders = wf.createWonders();

	std::random_device rd;
	std::mt19937 g(rd());
	std::shuffle(allWonders.begin(), allWonders.end(), g);

	// impartim cate 4 minuni
	for (int i = 0; i < 4; ++i) {
		if (!allWonders.empty()) {
			m_gameState->getCurrentPlayer().addWonder(std::move(allWonders.back()), i);
			allWonders.pop_back();
		}
	}
	m_gameState->switchPlayer();
	for (int i = 0; i < 4; ++i) {
		if (!allWonders.empty()) {
			m_gameState->getCurrentPlayer().addWonder(std::move(allWonders.back()), i);
			allWonders.pop_back();
		}
	}
	m_gameState->switchPlayer();
}

void GameController::selectWondersManual()
{
	WonderFactory wf;

	auto allWonders = wf.createWonders();

	std::random_device rd;
	std::mt19937 g(rd());
	std::shuffle(allWonders.begin(), allWonders.end(), g);

	std::vector<std::unique_ptr<Wonder>> first4Wonders;
	for (int i = 0; i < 4; ++i) {
		if (!allWonders.empty()) {
			first4Wonders.push_back(std::move(allWonders.back()));
			allWonders.pop_back();
		}
	}
	displayWondersForSelection(first4Wonders);

	Player& player1 = m_gameState->getCurrentPlayer();
	int player1Wonders = 0;
	Player& player2 = m_gameState->getOpponent();
	int player2Wonders = 0;

	int choice = Utils::getIntInput(player1.getName() + ", select a Wonder: ");
	player1.addWonder(std::move(first4Wonders[choice - 1]), player1Wonders++);
	first4Wonders.erase(first4Wonders.begin() + (choice - 1));

	for (int i = 0; i < 2; i++)
	{
		displayWondersForSelection(first4Wonders);
		choice = Utils::getIntInput(player2.getName() + ", select a Wonder: ");
		player2.addWonder(std::move(first4Wonders[choice - 1]), player2Wonders++);
		first4Wonders.erase(first4Wonders.begin() + (choice - 1));
	}

	std::cout << player1.getName() << ", you get the last wonder";
	player1.addWonder(std::move(first4Wonders[0]), player1Wonders++);
	first4Wonders.erase(first4Wonders.begin());

	for (int i = 0; i < 4; ++i) {
		if (!allWonders.empty()) {
			first4Wonders.push_back(std::move(allWonders.back()));
			allWonders.pop_back();
		}
	}
	displayWondersForSelection(first4Wonders);

	choice = Utils::getIntInput(player2.getName() + ", select a Wonder: ");
	player2.addWonder(std::move(first4Wonders[choice - 1]), player2Wonders++);
	first4Wonders.erase(first4Wonders.begin() + (choice - 1));

	for (int i = 0; i < 2; i++)
	{
		displayWondersForSelection(first4Wonders);
		choice = Utils::getIntInput(player1.getName() + ", select a Wonder: ");
		player1.addWonder(std::move(first4Wonders[choice - 1]), player1Wonders++);
		first4Wonders.erase(first4Wonders.begin() + (choice - 1));
	}

	std::cout << player2.getName() << ", you get the last wonder";
	player2.addWonder(std::move(first4Wonders[0]), player2Wonders++);
	first4Wonders.erase(first4Wonders.begin());

}

void GameController::displayWondersForSelection(const std::vector<std::unique_ptr<Wonder>>& wonders) const
{
	std::cout << "\n--- AVAILABLE WONDERS ---\n";
	for (size_t i = 0; i < wonders.size(); ++i)
	{
		std::cout << "[" << i + 1 << "] " << wonders[i]->toString() << "\n";
	}
	std::cout << "-------------------------\n";
}

bool GameController::handleConstructBuilding(int cardIndex)
{
	Player& currentPlayer = m_gameState->getCurrentPlayer();
	Player& opponent = m_gameState->getOpponent();

	const Card* cardPtr = m_gameState->getCardPtr(cardIndex);
	if (!cardPtr) return false;

	// plata cost carte
	int costToPay = 0;
	if (currentPlayer.hasChainForCard(*cardPtr))
		costToPay = 0;
	else {
		costToPay = currentPlayer.calculateResourceCost(cardPtr->getCost(), opponent);
		if (currentPlayer.getCoins() < costToPay)
			return false; // fonduri insuficiente
	}

	if (costToPay > 0)
		currentPlayer.removeCoins(costToPay);

	//extrag efect inainte de a muta cartea
	const CardEffect& effect = cardPtr->getEffect();

	// aplicare efecte

	
	const ResourceProduction& prod = effect.getProduction();
	if (!prod.isEmpty()) {
		for (const auto& [type, qty] : prod.getFixedResources()) {
			currentPlayer.addResource(type, qty);
		}
		if (prod.hasChoices()) {
			for (const auto& choice : prod.getChoices()) {
				std::vector<ResourceType> optionsCopy = choice;
				currentPlayer.addResourceChoice(optionsCopy);
			}
		}
	}

	
	if (auto shields = effect.getShields(); shields.has_value()) {
		currentPlayer.addMilitaryShields(shields.value());
		// de implementat verificare victorie militara
		// if (checkMilitaryVictory()) 
	}

	if (auto vp = effect.getVictoryPointsPerCard(); vp.has_value()) {
		currentPlayer.addVictoryPoints(vp.value());
	}

	if (auto symbol = effect.getScienceSymbol(); symbol.has_value()) {
		currentPlayer.addScientificSymbol(symbol.value());
		// de implementat verificare victorie stiintifica
	}

	if (auto coins = effect.getBaseCoins(); coins.has_value()) {
		currentPlayer.addCoins(coins.value());
	}

	std::unique_ptr<Card> takenCard = m_gameState->takeCard(cardIndex);
	currentPlayer.addCard(std::move(takenCard));

	return true;
}

bool GameController::handleDiscardCard(int cardIndex) 
{
	Player& currentPlayer = m_gameState->getCurrentPlayer();

	int yellowCards = static_cast<int>(currentPlayer.getCardsOfType(CardColor::YELLOW).size());
	int coinsGained = GameConstants::DISCARD_COINS_BASE + yellowCards;

	std::unique_ptr<Card> discardedCard = m_gameState->takeCard(cardIndex);
	// de impl : carti decartate, necesar si pt applywondereffect al denisei

	currentPlayer.addCoins(coinsGained);

	return true;
}

bool GameController::handleConstructWonders(int cardIndex, int wonderIndex, bool& outPlayAgain)
{
	Player& currentPlayer = m_gameState->getCurrentPlayer();
	Player& opponent = m_gameState->getOpponent();

	auto& wonders = currentPlayer.getWonders();
	if (wonderIndex < 0 || wonderIndex >= 4 || !wonders[wonderIndex]) return false;

	Wonder& targetWonder = *wonders[wonderIndex];

	if (targetWonder.isBuilt())
		return false; // e deja construita

	if (m_gameState->getAllConstructedWondersCount() >= GameConstants::MAX_WONDERS_TOTAL)
		return false; //sunt construite mai mult de 7 minuni global

	int costToPay = currentPlayer.calculateResourceCost(targetWonder.getCost(), opponent);
	if (currentPlayer.getCoins() < costToPay)
		return false;

	if (costToPay > 0) currentPlayer.removeCoins(costToPay);

	std::unique_ptr<Card> sacrificedCard = m_gameState->takeCard(cardIndex);

	currentPlayer.getConstructedWonders().push_back(std::move(wonders[wonderIndex]));
	// de mutat minunea in minuni realizate(daca facem) sau marcata ca realizata

	applyWonderEffect(currentPlayer, opponent, targetWonder);
	return true;
}

void GameController::prepareAge(int age) {
	// generare deck carti
	CardFactory cf;
	std::vector<std::unique_ptr<Card>> deck;

	if (age == 1) deck = cf.createAgeIDeck();
	else if (age == 2) deck = cf.createAgeIIDeck();
	else deck = cf.createAgeIIIDeck();

	std::random_device rd;
	std::mt19937 g(rd());
	std::shuffle(deck.begin(), deck.end(), g); // amestecare carti

	while (deck.size() > 20) {
		deck.pop_back(); // sterge carti pana raman cu 20/deck
	}

	
	m_gameState->initializeAge(age, deck); // trimite datele catre storage(state)
}

bool GameController::executeAction(int cardIndex, PlayerAction action, int wonderIndex) {
	if (!m_gameState->isCardAccessible(cardIndex)) {
		return false; // carte blocata/inexistenta
	}

	Player& currentPlayer = m_gameState->getCurrentPlayer();
	const Player& opponent = m_gameState->getOpponent();
	const Card* cardPtr = m_gameState->getCardPtr(cardIndex);

	if (!cardPtr) return false;

	bool success = false;
	bool playAgain = false;

	switch (action) {
	case PlayerAction::CONSTRUCT_BUILDING:
		success = handleConstructBuilding(cardIndex);
		break;

	case PlayerAction::DISCARD_FOR_COINS:
		success = handleDiscardCard(cardIndex);
		break;

	case PlayerAction::CONSTRUCT_WONDER:
		success = handleConstructWonders(cardIndex, wonderIndex, playAgain);
		break;
	}


	if (success) {
		bool anyCardLeft = false;
		for (const auto& node : m_gameState->getPyramid()) {
			if (!node.m_isRemoved) { anyCardLeft = true; break; }
		}

		if (!anyCardLeft) {
			checkEndAge();
		}
		else {
			if (!playAgain) {
				m_gameState->switchPlayer();
			}
		}
		return true;
	}

	return false;
}

void GameController::checkEndAge() {
	int currentAge = m_gameState->getCurrentAge();
	if (currentAge < 3) {
		prepareAge(currentAge + 1);
	}
	else {
		m_gameState->setGameOver(true);
	}
}

void GameController::saveGame(const std::string& filename) const {
	m_gameState->saveGame(std::string(filename));
}
void GameController::loadGame(const std::string& filename) {
	m_gameState->loadGame(std::string(filename));
}

void GameController::applyWonderEffect(Player& player, Player& opponent, const Wonder& wonder)
{
	WonderType type = wonder.getWonderType();

	switch (type)
	{
		case WonderType::THE_APPIAN_WAY:
		{
			player.addCoins(3);
			opponent.removeCoins(3);
			//+ play again player
			break;
		}
		case WonderType::CIRCUS_MAXIMUS:
		{
			player.addMilitaryShields(1);
			auto opponentCards = opponent.getCardsOfType(CardColor::GREY);

			if (opponentCards.empty()) {
				std::cout << "Opponent has no grey cards to destroy.\n";
				break;
			}

			std::cout << "Your opponent's grey cards:\n";
			int index = 1;
			for (const Card* card : opponentCards)
			{
				std::cout << index<<"."<< card->getName() << "\n";
				index++;
			}

			std::cout << "Choose a card to destroy: \n";
			std::cin >> index;

			while (index < 1 || index > opponentCards.size())
			{
				std::cout << "Invalid choice. Try again: \n";
				std::cin >> index;
			}

			const Card* chosen = opponentCards[index - 1];
			auto removedCard = opponent.removeCard(*chosen);

			m_gameState->addToDiscardCards(std::move(removedCard)); // am sters param GameState pentru ca 
															// poate fi accesat cu pointer direct
		
			break;
		}
		case WonderType::THE_COLOSSUS:
		{
			player.addMilitaryShields(2);
			player.addVictoryPoints(3);
			break;
		}
		case WonderType::THE_GREAT_LIBRARY:
		{
			player.addVictoryPoints(4);
			//+Randomly draw 3 Progress tokens from among those discarded at
			//the beginning of the game.Choose one, play it, and return the other
			//2 to the box.
			break;
		}
		case WonderType::THE_GREAT_LIGHTHOUSE:
		{
			player.addVictoryPoints(4);
			player.addResourceChoice({
				ResourceType::WOOD,
				ResourceType::STONE,
				ResourceType::CLAY
				});
			break;
		}
	}
}
