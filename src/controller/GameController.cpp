#include "GameController.h"


GameController::GameController(IGameView &view): m_view(view) {}

const GameState& GameController::getGameState() const {
	return *m_gameState;
}

bool GameController::isGameOver() const {
	return m_gameState->isGameOver();
}

void GameController::startNewGame(const std::string& p1, const std::string& p2) {
	m_gameState = std::make_unique<GameState>(p1, p2);
	draftWondersAuto(); // am pus asa momentan pentru interfata grafica
	prepareProgressTokens();
	prepareAge(1);

	m_view.get().onStateUpdated(); // notifica ui ca s-a inceput un joc nou
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
	//displayWondersForSelection(first4Wonders);

	Player& player1 = m_gameState->getCurrentPlayer();
	int player1Wonders = 0;
	Player& player2 = m_gameState->getOpponent();
	int player2Wonders = 0;

	// int choice = Utils::getIntRange(1, static_cast<int>(first4Wonders.size()), player1.getName() + ", select a Wonder: ");
	// player1.addWonder(std::move(first4Wonders[choice - 1]), player1Wonders++);
	// first4Wonders.erase(first4Wonders.begin() + (choice - 1));

	int choice = m_view.get().askWonderSelection(first4Wonders, player1.getName());
	player1.addWonder(std::move(first4Wonders[choice]), player1Wonders++);
	first4Wonders.erase(first4Wonders.begin() + choice);


	for (int i = 0; i < 2; i++)
	{
		choice = m_view.get().askWonderSelection(first4Wonders, player2.getName());
		player2.addWonder(std::move(first4Wonders[choice]), player2Wonders++);
		first4Wonders.erase(first4Wonders.begin() + choice);
	}

	m_view.get().onMessage(player1.getName() + " gets the last wonder: " + first4Wonders[0]->getName());
	player1.addWonder(std::move(first4Wonders[0]), player1Wonders++);
	first4Wonders.erase(first4Wonders.begin());

	for (int i = 0; i < 4; ++i) {
		if (!allWonders.empty()) {
			first4Wonders.push_back(std::move(allWonders.back()));
			allWonders.pop_back();
		}
	}
	//displayWondersForSelection(first4Wonders);

	choice = m_view.get().askWonderSelection(first4Wonders, player2.getName());
	player1.addWonder(std::move(first4Wonders[choice]), player2Wonders++);
	first4Wonders.erase(first4Wonders.begin() + choice);

	for (int i = 0; i < 2; i++)
	{
		choice = m_view.get().askWonderSelection(first4Wonders, player1.getName());
		player2.addWonder(std::move(first4Wonders[choice]), player1Wonders++);
		first4Wonders.erase(first4Wonders.begin() + choice);
	}


	m_view.get().onMessage(player2.getName() + " gets the last wonder: " + first4Wonders[0]->getName());
	player1.addWonder(std::move(first4Wonders[0]), player2Wonders++);
	first4Wonders.erase(first4Wonders.begin());

}


void GameController::checkMilitaryLooting(int previousShields, int currentShields)
{
	const int ZONE_1 = GameConstants::MILITARY_ZONE_1;
	const int ZONE_2 = GameConstants::MILITARY_ZONE_2;

	Player& opponent = m_gameState->getOpponent();

	if (previousShields < ZONE_1 && currentShields >= ZONE_1) {
		if (m_gameState->removeMilitaryToken(0)) {
			opponent.removeCoins(2);
			m_view.get().onMessage("--- MILITARY ATTACK! Opponent lost 2 coins (Zone 1) ---");
		}
	}
	if (previousShields < ZONE_2 && currentShields >= ZONE_2) {
		if (m_gameState->removeMilitaryToken(1)) {
			opponent.removeCoins(5);
			m_view.get().onMessage("--- MILITARY ATTACK! Opponent lost 5 coins (Zone 2) ---");
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

		m_view.get().onMessage("\n--- MILITARY VICTORY ---\n");
		return;
	}
	if (current.hasScientificVictory()) {
		m_gameState->setGameOver(true);
		m_gameState->setWinner(m_gameState->getCurrentPlayerIndex());
		m_view.get().onMessage("\n--- SCIENTIFIC VICTORY ---\n");
		return;
	}
}

void GameController::applyEffect(Player& player, const CardEffect& effect)
{
	Player& opponent = (player.getName() == m_gameState->getCurrentPlayer().getName())
		? m_gameState->getOpponent()
		: m_gameState->getCurrentPlayer();

	if (effect.getVictoryPointsPerCard().has_value())
		player.addVictoryPoints(effect.getVictoryPointsPerCard().value());

	if (effect.getShields().has_value())
		player.addMilitaryShields(effect.getShields().value());

	if (effect.getBaseCoins().has_value())
		player.addCoins(effect.getBaseCoins().value());

	if (effect.getScienceSymbol().has_value())
		player.addScientificSymbol(effect.getScienceSymbol().value());

	if (!effect.getDiscounts().empty())
	{
		for (const auto& discount : effect.getDiscounts())
			player.addTradeDiscount(discount.first, discount.second);
	}

	if (!effect.getProduction().isEmpty())
		player.addResourceProduction(effect.getProduction());
	
	if (!effect.getCoinsPerCardType().empty()) {
		if (effect.getGrantsGuildCopy()) {
			for (const auto& pair: effect.getCoinsPerCardType()) {
				Player& playerWithMostCards = m_gameState->getPlayerWithMostCardsPerColor(pair.first);
				int coinsToAdd = playerWithMostCards.getCardPerColor(pair.first) * pair.second;
				player.addCoins(coinsToAdd);
			}
		}
		else {
			for (const auto& pair : effect.getCoinsPerCardType()) {
				int coinsToAdd = player.getCardPerColor(pair.first) * pair.second;
				player.addCoins(coinsToAdd);
			}
		}
		// sunt 2 tipuri de aplicari: in functie de cate carti are jucatorul curent(carte normala) sau
		// in functie de cine are cele mai multe carti de acea culoare (guild)
		// cand schimb cu cardEffect pt a diferentia intre cele 2 cazuri folosesc acel membru guildCopy
	}

	if (effect.getGrantsProgressToken())
	{
		auto& discardedTokens = m_gameState->getDiscardedTokens();


		int chosenIndex = m_view.get().askTokenSelection(
			discardedTokens,
			"Select a Progress Token to claim:"
		);

		if (chosenIndex >= 0 && chosenIndex < discardedTokens.size()) {
			player.addProgressToken(std::move(discardedTokens[chosenIndex]));
			discardedTokens.erase(discardedTokens.begin() + chosenIndex);
			applyProgressTokenEffect(player, opponent, *player.getProgressTokens().back());
		}
	}

	if (effect.getOpponentLosesCoins().has_value())
	{
		opponent.removeCoins(effect.getOpponentLosesCoins().value());
	}

	if (effect.getOpponentLosesCard().has_value())
	{
		auto cardsOfColor = opponent.getCardsOfType(effect.getOpponentLosesCard().value());
		if (cardsOfColor.empty()) {
			m_view.get().onMessage("The opponent doesn't have a card of this color.");
		}
		else {

			int chosenIndex = m_view.get().askCardSelectionFromList(
				cardsOfColor,
				"Select a card to remove from opponent:"
			);

			if (chosenIndex >= 0 && chosenIndex < cardsOfColor.size()) {
				auto removedCard = opponent.removeCard(cardsOfColor[chosenIndex].get());
				if (removedCard != nullptr)
					m_gameState->addToDiscardCards(std::move(removedCard));
			}
		}
	}

	if (effect.getGrantsDiscardedCard())
	{
		auto& discardedCards = m_gameState->getDiscardedCards();
		if (discardedCards.empty()) {
			m_view.get().onMessage("The discarded pile is empty.");
		}
		else {
			// Trebuie să construim un vector de referințe pentru UI
			std::vector<std::reference_wrapper<const Card>> cardOptions;
			for(const auto& cPtr : discardedCards) {
				cardOptions.push_back(std::cref(*cPtr));
			}

			int chosenIndex = m_view.get().askCardSelectionFromList(
				cardOptions,
				"Select a card to retrieve from the discarded pile and play for FREE:"
			);

			if (chosenIndex >= 0 && chosenIndex < discardedCards.size()) {
				auto cardFromDiscard = m_gameState->extractDiscardedCard(chosenIndex);
				if (cardFromDiscard)
					grantCardToPlayer(player, std::move(cardFromDiscard));
			}
		}
	}

	// cand construim o minune trebuie sa vedem daca avem o carte care ne ofera banuti 
	// la finalul jocului trebuie sa vefiricam daca jucatorii au carti care le ofera vp pe carte/minune/set de banuti
}

void GameController::grantCardToPlayer(Player& player, std::unique_ptr<Card> card)
{
	CardEffect effect = card->getEffect();
	CardColor cardColor = card->getColor();
	std::optional<ScientificSymbol> scienceSymbol = effect.getScienceSymbol();

	player.addCard(std::move(card));

	applyEffect(player, effect);

	if (cardColor == CardColor::RED) {
		if (player.hasProgressToken(ProgressTokenType::STRATEGY)) {
			player.addMilitaryShields(1);
			m_view.get().onMessage("Strategy Token active: +1 Extra Shield!");
		}
	}
	if (scienceSymbol.has_value()) {
		ScientificSymbol sym =effect.getScienceSymbol().value();
		if (player.addScientificSymbol(sym)) {
			m_gameState->setPendingScientificReward(true);
			m_view.get().onMessage("--- SCIENCE PAIR! You earned a Progress Token! ---");
		}
	}

}

bool GameController::handleConstructBuilding(int cardIndex)
{
	Player& currentPlayer = m_gameState->getCurrentPlayer();
	Player& opponent = m_gameState->getOpponent();

	auto cardView = m_gameState->getCardView(cardIndex);
	if (!cardView.has_value()) return false;
	auto& card = cardView.value().get();

	bool buildForFreeByChain = currentPlayer.hasChainForCard(card);

	Cost baseCost = card.getCost();
	Cost finalCost = baseCost;

	int tradeCost = 0;
	int costToPay = 0;

	// 1. Calculam costul total (monede proprii + trade)
	if (!currentPlayer.hasChainForCard(card) && 
		 card.getColor() == CardColor::BLUE &&
		 currentPlayer.hasProgressToken(ProgressTokenType::MASONRY))
	{
		int discount = 2;

		auto resourceCosts = baseCost.getResourceCosts();

		while (discount > 0)
		{
			std::vector <ResourceType> options;
			for (const auto& it: resourceCosts)
				if (it.second > 0) options.push_back(it.first);

			if (options.empty()) break;
			ResourceType chosen = m_view.get().askResourceSelection(
	options,
	"Masonry Effect: Choose a resource to reduce cost (-1):"
);

			resourceCosts[chosen]--;

			if (resourceCosts[chosen] <= 0)
				resourceCosts.erase(chosen);

			discount--;
		}

		finalCost = Cost(baseCost.getCoinCost(), resourceCosts);
	}
	
	if (!buildForFreeByChain) {
		tradeCost = currentPlayer.calculateTradeCost(finalCost, opponent);
		costToPay = tradeCost + finalCost.getCoinCost();
	}

	// 2. Verificam daca jucatorul isi permite (UI-ul ar fi trebuit sa verifice deja, dar facem double-check)
	if (!currentPlayer.canAfford(costToPay, opponent)) {
		// Nu are bani suficienti. In GUI asta nu ar trebui sa se intample daca butonul e dezactivat,
		// dar e bine sa avem protectia aici.
		return false;
	}

	// 3. Executam plata
	if (costToPay > 0) {
		currentPlayer.removeCoins(costToPay);
	}

	if (tradeCost > 0 && opponent.hasProgressToken(ProgressTokenType::ECONOMY)){
		opponent.addCoins(tradeCost);
	}

	// 4. Mutam cartea si aplicam efectele
	int oldShields = currentPlayer.getMilitaryShields();

	std::unique_ptr<Card> takenCard = m_gameState->takeCard(cardIndex);
	if (!takenCard) return false;

	grantCardToPlayer(currentPlayer, std::move(takenCard));

	if (buildForFreeByChain &&
		currentPlayer.hasProgressToken(ProgressTokenType::URBANISM))
	{
		currentPlayer.addCoins(4);
	}

	// 5. Verificam efectele militare imediate
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
	m_view.get().onMessage("Discarded card for " + std::to_string(coinsGained) + " coins.");

	return true;
}

bool GameController::handleConstructWonders(int cardIndex, int wonderIndex, bool& outPlayAgain)
{
	Player& currentPlayer = m_gameState->getCurrentPlayer();
	Player& opponent = m_gameState->getOpponent();

	auto& wonders = currentPlayer.getWonders();
	if (wonderIndex < 0 || wonderIndex >= 4 || !wonders[wonderIndex]) return false;

	auto& targetWonderPtr = wonders[wonderIndex];

	if (targetWonderPtr->isBuilt()) return false;

	if (m_gameState->getAllConstructedWondersCount() >= GameConstants::MAX_WONDERS_TOTAL) return false;

	//int costToPay = currentPlayer.calculateTotalCost(targetWonderPtr->getCost(), opponent);
	Cost baseCost = targetWonderPtr->getCost();
	Cost finalCost = baseCost;
	int costToPay = 0;
	int tradeCost = 0;

	if (currentPlayer.hasProgressToken(ProgressTokenType::ARCHITECTURE))
	{
		int discount = 2;
		auto resourceCosts = baseCost.getResourceCosts();

		while (discount > 0)
		{
			// 1. Construim opțiunile
			std::vector <ResourceType> options;
			for (const auto& it : resourceCosts)
				if (it.second > 0) options.push_back(it.first);

			if(options.empty()) break;

			// 2. Cerem UI-ului fara input output
			ResourceType chosen = m_view.get().askResourceSelection(
				options,
				"Architecture Effect: Choose a resource to reduce cost (-1):"
			);

			// 3. Aplicam reducerea
			resourceCosts[chosen]--;
			if (resourceCosts[chosen] <= 0)
				resourceCosts.erase(chosen);

			discount--;
		}
		finalCost = Cost(baseCost.getCoinCost(), resourceCosts);
	}
	else {
		finalCost = baseCost;
	}

	tradeCost = currentPlayer.calculateTradeCost(finalCost, opponent);
	costToPay = tradeCost + finalCost.getCoinCost();

	if (currentPlayer.getCoins() < costToPay) return false;

	if (costToPay > 0) currentPlayer.removeCoins(costToPay);

	if (tradeCost > 0 && opponent.hasProgressToken(ProgressTokenType::ECONOMY)) {
		opponent.addCoins(tradeCost);
	}

	std::unique_ptr<Card> sacrificedCard = m_gameState->takeCard(cardIndex);

	currentPlayer.getConstructedWonders().push_back(std::move(targetWonderPtr));
	// de mutat minunea in minuni realizate(daca facem) sau marcata ca realizata

	auto& builtWonder = currentPlayer.getConstructedWonders().back();

	bool hasTheology = currentPlayer.hasProgressToken(ProgressTokenType::THEOLOGY);
	bool wonderReplay = builtWonder->getEffect().getGrantsPlayAgain();

	if (hasTheology || wonderReplay) {
		outPlayAgain = true;
	}

	int oldShields = currentPlayer.getMilitaryShields();

	const CardEffect& wonderEffect = builtWonder->getEffect();
	applyEffect(currentPlayer, builtWonder->getEffect());

	int newShields = currentPlayer.getMilitaryShields();

	if (newShields > oldShields)
		checkMilitaryLooting(oldShields, newShields);

	// + trebuie sa verificam daca avem o carte care ne ofera bani pentru minune
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
		m_view.get().onError("[!] Card is blocked or invalid.");
		return false; // carte blocata/inexistenta
	}

	Player& currentPlayer = m_gameState->getCurrentPlayer();
	const Player& opponent = m_gameState->getOpponent();
	auto cardView = m_gameState->getCardView(cardIndex);
	if (!cardView.has_value()) return false;

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
		if (m_gameState->isGameOver()) {
			m_view.get().onStateUpdated();
			return true;
		}

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
		m_view.get().onStateUpdated();
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

void GameController::applyProgressTokenEffect(Player& player, Player& opponent, ProgressToken& token)
{
	ProgressTokenType type = token.getType();

	switch (type)
	{
		case ProgressTokenType::AGRICULTURE:
		{
			player.addCoins(6);
			player.addVictoryPoints(4);
			break;
		}
		case ProgressTokenType::ARCHITECTURE:
		{
			token.getEffect().withArchitectureEffect();
			break;
		}
		case ProgressTokenType::ECONOMY:
		{
			token.getEffect().withEconomyEffect();
			break;
		}
		case ProgressTokenType::LAW:
		{
			player.addScientificSymbol(ScientificSymbol::SCALES);
			break;
		}
		case ProgressTokenType::MASONRY:
		{
			token.getEffect().withMasonryEffect();
			break;
		}
		case ProgressTokenType::MATHEMATICS:
		{
			token.getEffect().withMathematicsEffect();
			break;
		}
		case ProgressTokenType::PHILOSOPHY:
		{
			player.addVictoryPoints(7);
			break;
		}
		case ProgressTokenType::STRATEGY:
		{
			token.getEffect().withStrategyEffect();
			break;
		}
		case ProgressTokenType::THEOLOGY:
		{
			token.getEffect().withTheologyEffect();
			break;
		}
		case ProgressTokenType::URBANISM:
		{
			player.addCoins(6);
			token.getEffect().withUrbanismEffect();
			break;
		}
	}
}
