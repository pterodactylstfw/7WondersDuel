#include "GameController.h"

GameController::GameController(IGameView& view) : m_view(view) {}

const GameState& GameController::getGameState() const {
	return *m_gameState;
}

bool GameController::hasGameStarted() const
{
	return m_gameState != nullptr;
}

bool GameController::isGameOver() const {
	if (!m_gameState) return false;
	return m_gameState->isGameOver();
}

void GameController::startNewGame(const std::string& p1, const std::string& p2) {
	m_gameState = std::make_unique<GameState>(p1, p2);

	prepareWonders();
	prepareProgressTokens();

	m_gameState->setCurrentPhase(GamePhase::DRAFTING);
	prepareDraftRound();

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

void GameController::prepareDraftRound()
{
	m_gameState->clearDraftedWonders();
	auto& deck = m_gameState->getAllWonders();

	for (int i = 0; i < 4; ++i) {
		if (!deck.empty()) {
			m_gameState->addWonderToDraft(std::move(deck.back()));
			deck.pop_back();
		}
	}
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

void GameController::handleCivilianVictory()
{
	Player& player1 = m_gameState->getCurrentPlayer();
	Player& player2 = m_gameState->getOpponent();

	int score1 = player1.getFinalScore(player2);
	int score2 = player2.getFinalScore(player1);

	if (score1 > score2)
		m_gameState->setWinner(m_gameState->getCurrentPlayerIndex());
	else if (score1 < score2)
		m_gameState->setWinner(1 - m_gameState->getCurrentPlayerIndex());
	else
	{
		int civilianPoints1 = player1.getCivilianVictoryPoints(player2);
		int civilianPoints2 = player2.getCivilianVictoryPoints(player1);

		if (civilianPoints1 > civilianPoints2)
			m_gameState->setWinner(m_gameState->getCurrentPlayerIndex());
		else if (civilianPoints1 < civilianPoints2)
			m_gameState->setWinner(1 - m_gameState->getCurrentPlayerIndex());
		else
		{
			m_gameState->setWinner(std::nullopt);

		}
	}
	m_gameState->setGameOver(true);
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
	{
		ScientificSymbol sym = effect.getScienceSymbol().value();
		if (player.addScientificSymbol(sym)) {
			m_gameState->setPendingScientificReward(true);
			m_view.get().onMessage("--- SCIENCE PAIR! You earned a Progress Token! ---");
		}
	}

	if (m_gameState->hasPendingScientificReward())
	{
		const auto& availableTokens = m_gameState->getAvailableTokens();
		if (!availableTokens.empty())
		{
			int chosenIndex = -1;

			if (player.isAI())
			{
				chosenIndex = 0;
				m_view.get().onMessage(player.getName() + " (AI) chose a Progress Token");
			}
			else
			{
				chosenIndex = m_view.get().askTokenSelection(availableTokens, "Scientific pair reward: Choose a Progress Token");
			}

			if (chosenIndex >= 0 && chosenIndex < (int)availableTokens.size())
			{
				auto token = m_gameState->extractAvailableToken(chosenIndex); // scoate de pe masa
				if (token)
				{
					player.addProgressToken(std::move(token));
					applyProgressTokenEffect(
						player,
						m_gameState->getOpponent(),
						*player.getProgressTokens().back()
					);
				}
			}
		}

		m_gameState->setPendingScientificReward(false); // sau clearPendingScientificReward()
	}


	if (!effect.getDiscounts().empty())
	{
		for (const auto& discount : effect.getDiscounts())
			player.addTradeDiscount(discount.first, discount.second);
	}

	if (!effect.getProduction().isEmpty())
		player.addResourceProduction(effect.getProduction());

	if (!effect.getCoinsPerCardType().empty()) {
		if (effect.getGrantsGuildCopy()) {
			for (const auto& pair : effect.getCoinsPerCardType()) {
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
		if (!discardedTokens.empty()) {
			int chosenIndex = -1;

			if (player.isAI()) {
				chosenIndex = 0;
				m_view.get().onMessage(player.getName() + "AI chose a Progress Token");
			}
			else {
				chosenIndex = m_view.get().askTokenSelection(discardedTokens, "Special effect: Choose a discarded Progress Token");
			}

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
				int chosenIndex = -1;
				if (m_gameState->getCurrentPlayer().isAI()) {
					chosenIndex = 0;
					m_view.get().onMessage("AI discarded one of your cards");
				}
				else {
					chosenIndex = m_view.get().askCardSelectionFromList(
						cardsOfColor,
						"Select a card to remove from opponent:"
					);
				}
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
				for (const auto& cPtr : discardedCards) {
					cardOptions.push_back(std::cref(*cPtr));
				}

				int chosenIndex = -1;
				if (player.isAI()) {
					chosenIndex = 0;
					m_view.get().onMessage("AI retrieved a card from discarded cards");
				}
				else {
					chosenIndex = m_view.get().askCardSelectionFromList(
						cardOptions,
						"Select a card to retrieve from the discarded pile and play for FREE:"
					);
				}

				if (chosenIndex >= 0 && chosenIndex < discardedCards.size()) {
					auto cardFromDiscard = m_gameState->extractDiscardedCard(chosenIndex);
					if (cardFromDiscard)
						grantCardToPlayer(player, std::move(cardFromDiscard));
				}
			}
		}
	}

	if (m_gameState->hasPendingScientificReward())
	{
		const auto& availableTokens = m_gameState->getAvailableTokens();
		if (!availableTokens.empty())
		{
			int chosenIndex = -1;

			if (player.isAI())
			{
				chosenIndex = 0;
				m_view.get().onMessage(player.getName() + " (AI) chose a Bonus Progress Token");
			}
			else
			{
				chosenIndex = m_view.get().askTokenSelection(
					availableTokens,
					"Bonus Reward: You formed a pair! Choose another Progress Token"
				);
			}

			if (chosenIndex >= 0 && chosenIndex < (int)availableTokens.size())
			{
				auto token = m_gameState->extractAvailableToken(chosenIndex);
				if (token)
				{
					player.addProgressToken(std::move(token));
					applyProgressTokenEffect(
						player,
						m_gameState->getOpponent(),
						*player.getProgressTokens().back()
					);
				}
			}
		}

		m_gameState->setPendingScientificReward(false);

		// cand construim o minune trebuie sa vedem daca avem o carte care ne ofera banuti 
		// la finalul jocului trebuie sa vefiricam daca jucatorii au carti care le ofera vp pe carte/minune/set de banuti
	}
}

	void GameController::grantCardToPlayer(Player & player, std::unique_ptr<Card> card)
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
				for (const auto& it : resourceCosts)
					if (it.second > 0) options.push_back(it.first);

				if (options.empty()) break;
				ResourceType chosen = ResourceType::NONE;

				if (currentPlayer.isAI()) {
					chosen = options[0];
				}
				else {
					chosen = m_view.get().askResourceSelection(
						options,
						"Masonry Effect: Choose a resource to reduce cost (-1):"
					);
				}

				if (resourceCosts.count(chosen)) {
					resourceCosts[chosen]--;
					if (resourceCosts[chosen] <= 0)
						resourceCosts.erase(chosen);
				}
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

		if (tradeCost > 0 && opponent.hasProgressToken(ProgressTokenType::ECONOMY)) {
			opponent.addCoins(tradeCost);
		}

		// 4. Mutam cartea si aplicam efectele
		int oldShields = currentPlayer.getMilitaryShields();

		std::unique_ptr<Card> takenCard = m_gameState->takeCard(cardIndex);
		if (!takenCard) return false;

		grantCardToPlayer(currentPlayer, std::move(takenCard));
		if (buildForFreeByChain) {
			m_view.get().onMessage(getGameState().getCurrentPlayer().getName() + ": Built " + card.getName() + " for FREE (Chain)!");
		}
		else {
			m_view.get().onMessage(getGameState().getCurrentPlayer().getName() + ": Built " + card.getName() + ".");
		}

		if (buildForFreeByChain &&
			currentPlayer.hasProgressToken(ProgressTokenType::URBANISM))
		{
			currentPlayer.addCoins(4);
			m_view.get().onMessage("+4 Coins (Urbanism Bonus)");
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
		m_view.get().onMessage(getGameState().getCurrentPlayer().getName() + ": Discarded card for " + std::to_string(coinsGained) + " coins.");

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

				if (options.empty()) break;

				// 2. Cerem UI-ului fara input output
				ResourceType chosen = ResourceType::NONE;
				if (currentPlayer.isAI()) {
					chosen = options[0];
				}
				else {
					chosen = m_view.get().askResourceSelection(
						options,
						"Architecture Effect: Choose a resource to reduce cost (-1):"
					);
				}
				// 3. Aplicam reducerea
				if (resourceCosts.count(chosen)) {
					resourceCosts[chosen]--;
					if (resourceCosts[chosen] <= 0)
						resourceCosts.erase(chosen);
				}
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
		m_view.get().onMessage(getGameState().getCurrentPlayer().getName() + ": Wonder Constructed: " + builtWonder->getName() + "!");

		bool hasTheology = currentPlayer.hasProgressToken(ProgressTokenType::THEOLOGY);
		bool wonderReplay = builtWonder->getEffect().getGrantsPlayAgain();

		if (hasTheology || wonderReplay) {
			outPlayAgain = true;
			m_view.get().onMessage("Play Again granted!");
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

	void GameController::prepareWonders()
	{
		WonderFactory wf;
		auto& allWonders = m_gameState->getAllWonders();
		allWonders = wf.createWonders();

		std::random_device rd;
		std::mt19937 g(rd());
		std::shuffle(allWonders.begin(), allWonders.end(), g);
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
		return false;
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

		default: break;
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
			handleCivilianVictory();
		}
	}

	void GameController::saveGame(const std::string & filename) const {
		m_gameState->saveGame(std::string(filename));
	}

	void GameController::loadGame(const std::string & filename) {
		if (!m_gameState) {
			m_gameState = std::make_unique<GameState>(); // init daca e null,
			//fix crash load la cold start
		}

		if (!m_gameState->loadGame(std::string(filename))) {

			m_gameState.reset(); // sterg daca load esueaza

			throw std::runtime_error("Failed to load game file.");
		}
	}

	void GameController::applyProgressTokenEffect(Player & player, Player & opponent, ProgressToken & token)
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
			if (player.addScientificSymbol(ScientificSymbol::SCALES)) {
				m_gameState->setPendingScientificReward(true);
				m_view.get().onMessage("LAW TOKEN PAIR! You earned another Progress Token!");
			}
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
		default: break;
    }
	}

	bool GameController::pickWonder(int wonderIndex)
	{
		if (m_gameState->getCurrentPhase() != GamePhase::DRAFTING) return false;
		std::unique_ptr<Wonder> selectedWonder = m_gameState->extractWonderFromDraft(wonderIndex);

		if (!selectedWonder) return false;

		int slot = -1;
		auto& currentP = m_gameState->getCurrentPlayer();
		auto& wondersArr = currentP.getWonders();

		// caut slot liber
		for (int i = 0; i < 4; ++i) {
			if (wondersArr[i] == nullptr) {
				slot = i;
				break;
			}
		}

		if (slot != -1) {
			currentP.addWonder(std::move(selectedWonder), slot);
		}
		else {
			return false;
		}

		// cate minuni au ramas in draft
		int remaining = m_gameState->getDraftedWonders().size();

		if (remaining == 3) {
			// a ales primul jucator prima minune -> schimbam tura
			m_gameState->switchPlayer();
		}
		else if (remaining == 2) {
			// a ales al doilea jucator prima sa minune -> NU schimbam tura(mai alege una)
		}
		else if (remaining == 1) {
			// a ales al doilea jucator a doua sa minune -> schimbam tura
			m_gameState->switchPlayer();
		}
		else if (remaining == 0) {
			int p1Count = 0;
			const auto& p1Wonders = m_gameState->getPlayers()[0]->getWonders();
			for (const auto& w : p1Wonders)
				if (w) p1Count++;

			if (p1Count == 2) {
				prepareDraftRound();
				m_gameState->switchPlayer();
			}
			else {
				m_gameState->setCurrentPhase(GamePhase::AGE_I);
				prepareAge(1);

				m_gameState->switchPlayer();
			}
		}

		m_view.get().onStateUpdated();
		return true;
	}

	void GameController::debugTriggerVictory() {
		if (!m_gameState) return;
		Player& current = m_gameState->getCurrentPlayer();
		const Player& opponent = m_gameState->getOpponent();

		int currentDiff = current.getMilitaryShields() - opponent.getMilitaryShields();
		int needed = GameConstants::MILITARY_SUPREMACY_DISTANCE - currentDiff;

		if (needed > 0) {
			current.addMilitaryShields(needed);

			checkInstantVictory();

			m_view.get().onStateUpdated();
			m_view.get().onMessage("DEBUG: Military Victory Triggered!");
		}
	}

	void GameController::reset() {
		m_gameState.reset();
	}
