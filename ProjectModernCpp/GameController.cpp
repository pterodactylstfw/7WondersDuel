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

void GameController::checkInstantVictory()
{
	const Player& current = m_gameState->getCurrentPlayer();
	const Player& opponent = m_gameState->getOpponent();

	int diff = current.getMilitaryShields() - opponent.getMilitaryShields();
	if (std::abs(diff) >= GameConstants::MILITARY_SUPREMACY_DISTANCE) {
		m_gameState->setGameOver(true);
		std::cout << "Military Victory! " << (diff > 0 ? current.getName() : opponent.getName()) << " wins!\n";
		return;
	}
	if (current.hasScientificVictory()) {
		m_gameState->setGameOver(true);
		std::cout << "Scientific Victory! " << current.getName() << " wins!\n";
	}
}

bool GameController::handleConstructBuilding(int cardIndex)
{
	Player& currentPlayer = m_gameState->getCurrentPlayer();
	Player& opponent = m_gameState->getOpponent();

	const Card* cardPtr = m_gameState->getCardPtr(cardIndex);
	if (!cardPtr) return false;

	int costToPay = 0;
	if (currentPlayer.hasChainForCard(*cardPtr))
		costToPay = 0;

	else {
		costToPay = currentPlayer.calculateResourceCost(cardPtr->getCost(), opponent);
		if (currentPlayer.getCoins() < costToPay)
			return false; // fonduri insuficiente
	}

	if (costToPay)
		currentPlayer.removeCoins(costToPay);

	std::unique_ptr<Card> takenCard = m_gameState->takeCard(cardIndex);
	// de impl: verif perechi stiintifice(efecte imediate)
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

void GameController::prepareProgressTokens()
{
	ProgressTokenFactory ptf;
	auto allTokens = ptf.createAllTokens();

	std::random_device rd;
	std::mt19937 g(rd());
	std::shuffle(allTokens.begin(), allTokens.end(), g);

	for (int i = 0; i < 5; i++) 
	{
		m_gameState->addToAvailableTokens(std::move(allTokens[i]));
	}

	for (int i = 5; i < 10; i++) 
	{
		m_gameState->addToDiscardTokens(std::move(allTokens[i]));
	}
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
			player.addVictoryPoints(3);
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
			index--;

			const Card* chosen = opponentCards[index];
			auto removedCard = opponent.removeCard(*chosen);

			m_gameState->addToDiscardCards(std::move(removedCard));
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
		case WonderType::THE_HANGING_GARDENS:
		{
			player.addCoins(6);
			//+ second turn
			player.addVictoryPoints(3);
			break;
		}
		case WonderType::THE_MAUSOLEUM:
		{
			const auto& discarded_cards = m_gameState->getDiscardedCards();
			if (discarded_cards.empty())
			{
				std::cout << "The discard pile is empty\n";
				break;
			}

			std::cout << "Choose a card to build for free\n";
			int index = 1;
			for (const auto& card : discarded_cards)
			{
				std::cout << index << "." << card->getName() << "\n";
				index++;
			}

			std::cout << "Choose a card to build: \n";
			std::cin >> index;

			while (index < 1 || index > discarded_cards.size())
			{
				std::cout << "Invalid choice. Try again: \n";
				std::cin >> index;
			}
			index--;

			std::unique_ptr<Card> cardToBuild = m_gameState->extractDiscardedCard(index);
			if (cardToBuild)
			{
				player.addCard(std::move(cardToBuild));
			}
			break;
		}
		case WonderType::PIRAEUS:
		{
			player.addResourceChoice({
				ResourceType::PAPYRUS,
				ResourceType::GLASS
				});
			//+second turn
			player.addVictoryPoints(2);
			break;
		}
		case WonderType::THE_PYRAMIDS:
		{
			player.addVictoryPoints(9);
			break;
		}
		case WonderType::THE_SPHINX:
		{
			//+second turn
			player.addVictoryPoints(6);
			break;
		}
		case WonderType::THE_STATUE_OF_ZEUS:
		{
			player.addMilitaryShields(1);
			player.addVictoryPoints(3);
			auto opponentCards = opponent.getCardsOfType(CardColor::BROWN);

			if (opponentCards.empty()) {
				std::cout << "Opponent has no brown cards to destroy.\n";
				break;
			}

			std::cout << "Your opponent's brown cards:\n";
			int index = 1;
			for (const Card* card : opponentCards)
			{
				std::cout << index << "." << card->getName() << "\n";
				index++;
			}

			std::cout << "Choose a card to destroy: \n";
			std::cin >> index;

			while (index < 1 || index > opponentCards.size())
			{
				std::cout << "Invalid choice. Try again: \n";
				std::cin >> index;
			}
			index--;

			const Card* chosen = opponentCards[index];
			auto removedCard = opponent.removeCard(*chosen);

			m_gameState->addToDiscardCards(std::move(removedCard));
			break;
		}
	}
}
