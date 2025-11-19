#include "GameController.h"

const GameState& GameController::getGameState() const {
	return *m_gameState;
}

bool GameController::isGameOver() const {
	return m_gameState->isGameOver();
}

void GameController::startNewGame(const std::string& p1, const std::string& p2) {
	m_gameState = std::make_unique<GameState>();
	// de pus setare nume playeri folosind setterii din player

	draftWondersAuto();
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

	if (action == PlayerAction::CONSTRUCT_BUILDING) {
		int cost = 0; // logica cost
		bool freeChain = currentPlayer.hasChainForCard(*cardPtr);

		if (!freeChain) {
			cost = currentPlayer.calculateResourceCost(cardPtr->getCost(), opponent);
		}

		if (freeChain || currentPlayer.getCoins() >= cost) {
			currentPlayer.removeCoins(cost);


			auto cardObj = m_gameState->takeCard(cardIndex); // mut din storage -> inventar player
			// cardObj->getEffect().apply(currentPlayer); // de implementat aplicare efect daca e nevoie
			currentPlayer.addCard(std::move(cardObj));

			m_gameState->removeCardFromPyramid(cardIndex);
			success = true;
		}
	}
	else if (action == PlayerAction::DISCARD_FOR_COINS) {
		int coins = 2 + static_cast<int>(currentPlayer.getCardsOfType(CardColor::YELLOW).size()); // 2 monede + nr carti galbene
		currentPlayer.addCoins(coins);

		// Eliminăm cartea definitiv (o scoatem din storage și o lăsăm să moară)
		m_gameState->takeCard(cardIndex);
		m_gameState->removeCardFromPyramid(cardIndex);
		success = true;
	}
	// de pus logica wonder aici???

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
			opponent.removeCard(*chosen);
			//+ sa punem cartea in decartate
			break;
		}
	}
}
