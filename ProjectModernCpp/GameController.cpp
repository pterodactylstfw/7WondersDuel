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
	prepareProgressTokens();
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

	int choice = Utils::getIntRange(1, static_cast<int>(first4Wonders.size()), player1.getName() + ", select a Wonder: ");
	player1.addWonder(std::move(first4Wonders[choice - 1]), player1Wonders++);
	first4Wonders.erase(first4Wonders.begin() + (choice - 1));

	for (int i = 0; i < 2; i++)
	{
		displayWondersForSelection(first4Wonders);
		choice = Utils::getIntRange(1, static_cast<int>(first4Wonders.size()), player2.getName() + ", select a Wonder: ");
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

	choice = Utils::getIntRange(1, static_cast<int>(first4Wonders.size()), player2.getName() + ", select a Wonder: ");
	player2.addWonder(std::move(first4Wonders[choice - 1]), player2Wonders++);
	first4Wonders.erase(first4Wonders.begin() + (choice - 1));

	for (int i = 0; i < 2; i++)
	{
		displayWondersForSelection(first4Wonders);
		choice = Utils::getIntRange(1, static_cast<int>(first4Wonders.size()), player1.getName() + ", select a Wonder: ");
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

void GameController::checkMilitaryLooting(int previousShields, int currentShields)
{
	const int ZONE_1 = GameConstants::MILITARY_ZONE_1;
	const int ZONE_2 = GameConstants::MILITARY_ZONE_2;

	Player& opponent = m_gameState->getOpponent();

	if (previousShields < ZONE_1 && currentShields >= ZONE_1) {
		if (m_gameState->removeMilitaryToken(0)) {
			opponent.removeCoins(2);
			std::cout << "--- MILITARY ATTACK! Opponent lost 2 coins (Zone 1) ---\n";
		}
	}
	if (previousShields < ZONE_2 && currentShields >= ZONE_2) {
		if (m_gameState->removeMilitaryToken(1)) {
			opponent.removeCoins(5);
			std::cout << "--- MILITARY ATTACK! Opponent lost 5 coins (Zone 2) ---\n";
		}
	}
}

void GameController::checkInstantVictory()
{
	const Player& current = m_gameState->getCurrentPlayer();
	const Player& opponent = m_gameState->getOpponent();

	int diff = current.getMilitaryShields() - opponent.getMilitaryShields();
	if (std::abs(diff) >= GameConstants::MILITARY_SUPREMACY_DISTANCE) {
		m_gameState->setGameOver(true);
		if (diff > 0) m_gameState->setWinner(m_gameState->getCurrentPlayerIndex());
		else m_gameState->setWinner(1 - m_gameState->getCurrentPlayerIndex());

		std::cout << "\n--- MILITARY VICTORY ---\n";
		return;
	}
	if (current.hasScientificVictory()) {
		m_gameState->setGameOver(true);
		m_gameState->setWinner(m_gameState->getCurrentPlayerIndex());
		std::cout << "\n--- SCIENTIFIC VICTORY ---\n";
		return;
	}
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
		if (currentPlayer.getCoins() < costToPay) {
			std::cout << "[!] Insufficient coins to build this card.\n";
			return false;
		}
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
  
	int oldShields = currentPlayer.getMilitaryShields();
	std::unique_ptr<Card> takenCard = m_gameState->takeCard(cardIndex);

	if (takenCard->getColor() == CardColor::RED) {
		if (currentPlayer.hasProgressToken(ProgressTokenType::STRATEGY)) {
			currentPlayer.addMilitaryShields(1);
			std::cout << "Strategy Token active: +1 Extra Shield!\n";
		}
	}
	if (takenCard->getEffect().getScienceSymbol().has_value()) {
		ScientificSymbol sym = takenCard->getEffect().getScienceSymbol().value();
		if (currentPlayer.addScientificSymbol(sym)) {
			m_gameState->setPendingScientificReward(true);
			std::cout << "--- SCIENCE PAIR! You earned a Progress Token! ---\n";
		}
	}

	currentPlayer.addCard(std::move(takenCard));
	int newShields = currentPlayer.getMilitaryShields();
	if (newShields > oldShields) {
		checkMilitaryLooting(oldShields, newShields);
	}

	return true;
}

bool GameController::handleDiscardCard(int cardIndex)
{
	Player& currentPlayer = m_gameState->getCurrentPlayer();

	int yellowCards = static_cast<int>(currentPlayer.getCardsOfType(CardColor::YELLOW).size());
	int coinsGained = GameConstants::DISCARD_COINS_BASE + yellowCards;

	std::unique_ptr<Card> discardedCard = m_gameState->takeCard(cardIndex);
	m_gameState->addToDiscardCards(std::move(discardedCard));

	currentPlayer.addCoins(coinsGained);
	std::cout << "Discarded card for " << coinsGained << " coins.\n";

	return true;
}

bool GameController::handleConstructWonders(int cardIndex, int wonderIndex, bool& outPlayAgain)
{
	Player& currentPlayer = m_gameState->getCurrentPlayer();
	Player& opponent = m_gameState->getOpponent();

	auto& wonders = currentPlayer.getWonders();
	if (wonderIndex < 0 || wonderIndex >= 4 || !wonders[wonderIndex]) return false;

	Wonder& targetWonder = *wonders[wonderIndex];

	if (targetWonder.isBuilt()) {
		std::cout << "Wonder already built!\n";
		return false;
	}

	if (m_gameState->getAllConstructedWondersCount() >= GameConstants::MAX_WONDERS_TOTAL) {
		std::cout << "Max 7 wonders built globally!\n";
		return false;
	}

	int costToPay = currentPlayer.calculateResourceCost(targetWonder.getCost(), opponent);
	if (currentPlayer.getCoins() < costToPay) {
		std::cout << "[!] Insufficient coins for Wonder.\n";
		return false;
	}

	if (costToPay > 0) currentPlayer.removeCoins(costToPay);

	std::unique_ptr<Card> sacrificedCard = m_gameState->takeCard(cardIndex);
	m_gameState->addToDiscardCards(std::move(sacrificedCard));

	currentPlayer.getConstructedWonders().push_back(std::move(wonders[wonderIndex]));
	// de mutat minunea in minuni realizate(daca facem) sau marcata ca realizata

	bool hasTheology = currentPlayer.hasProgressToken(ProgressTokenType::THEOLOGY);
	bool wonderReplay = targetWonder.getEffect().getDescription().find("Play a second turn") != std::string::npos;

	if (hasTheology || wonderReplay) {
		outPlayAgain = true;
		std::cout << ">>> Play Again granted!\n";
	}
	int oldShields = currentPlayer.getMilitaryShields();
	//applyWonderEffect(currentPlayer, opponent, targetWonder);
	int newShields = currentPlayer.getMilitaryShields();

	if (newShields > oldShields) {
		checkMilitaryLooting(oldShields, newShields);
	}

	return true;
}

void GameController::prepareAge(int age) {
	// generare deck carti
	CardFactory cf;
	std::vector<std::unique_ptr<Card>> deck;

	if (age == 1) deck = cf.createAgeIDeck();
	else if (age == 2) deck = cf.createAgeIIDeck();
	else if (age == 3) {
		deck = cf.createAgeIIIDeck();
		//in age 3 avem si guilds
		auto guilds = cf.createGuildDeck();
		std::shuffle(guilds.begin(), guilds.end(), std::mt19937{ std::random_device{}() });
		for (int i = 0; i < 3; ++i) deck.push_back(std::move(guilds[i]));
	}

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
		std::cout << "[!] Card is blocked or invalid.\n";
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
		checkInstantVictory();
		if (m_gameState->isGameOver()) return true;

		m_gameState->removeCardFromPyramid(cardIndex);

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
	if (!m_gameState) {
		m_gameState = std::make_unique<GameState>(); // init daca e null,
													//fix crash load la cold start
	}

	if (!m_gameState->loadGame(std::string(filename))) {

		m_gameState.reset(); // sterg daca load esueaza

		throw std::runtime_error("Failed to load game file.");
	}
}

