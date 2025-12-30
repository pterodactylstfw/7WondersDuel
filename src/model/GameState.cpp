#include "GameState.h"
#include <fstream>

void GameState::buildPyramidStructure(int age) {
    m_pyramid.clear();
    m_pyramid.resize(20);

    for (int i = 0; i < 20; ++i) {
        m_pyramid[i].m_index = i;
        m_pyramid[i].m_isRemoved = false;
        m_pyramid[i].m_blockedBy.clear();
    }

    if (age == 1) {
        // age I 
        for (int i = 0; i <= 5; i++) m_pyramid[i].m_isFaceUp = true;
        for (int i = 6; i <= 10; i++) m_pyramid[i].m_isFaceUp = false;
        for (int i = 11; i <= 14; i++) m_pyramid[i].m_isFaceUp = true;
        for (int i = 15; i <= 17; i++) m_pyramid[i].m_isFaceUp = false;
        for (int i = 18; i <= 19; i++) m_pyramid[i].m_isFaceUp = true;

        // Dependinte carti age I
        m_pyramid[6].m_blockedBy = { 0, 1 };
        m_pyramid[7].m_blockedBy = { 1, 2 };
        m_pyramid[8].m_blockedBy = { 2, 3 };
        m_pyramid[9].m_blockedBy = { 3, 4 };
        m_pyramid[10].m_blockedBy = { 4, 5 };

        m_pyramid[11].m_blockedBy = { 6, 7 };
        m_pyramid[12].m_blockedBy = { 7, 8 };
        m_pyramid[13].m_blockedBy = { 8, 9 };
        m_pyramid[14].m_blockedBy = { 9, 10 };

        m_pyramid[15].m_blockedBy = { 11, 12 };
        m_pyramid[16].m_blockedBy = { 12, 13 };
        m_pyramid[17].m_blockedBy = { 13, 14 };

        m_pyramid[18].m_blockedBy = { 15, 16 };
        m_pyramid[19].m_blockedBy = { 16, 17 };
    }
    
    if (age == 2) {
        // age II
        for (int i = 0; i <= 1; i++) m_pyramid[i].m_isFaceUp = true;
        for (int i = 2; i <= 4; i++) m_pyramid[i].m_isFaceUp = false;
        for (int i = 5; i <= 8; i++) m_pyramid[i].m_isFaceUp = true;
        for (int i = 9; i <= 13; i++) m_pyramid[i].m_isFaceUp = false;
        for (int i = 14; i <= 19; i++) m_pyramid[i].m_isFaceUp = true;

        // Dependinte carti age II
        m_pyramid[2].m_blockedBy = { 0 };
        m_pyramid[3].m_blockedBy = { 0, 1 };
        m_pyramid[4].m_blockedBy = { 1 };

        m_pyramid[5].m_blockedBy = { 2 };
        m_pyramid[6].m_blockedBy = { 2, 3 };
        m_pyramid[7].m_blockedBy = { 3, 4 };
        m_pyramid[8].m_blockedBy = { 4 };

        m_pyramid[9].m_blockedBy = { 5 };
        m_pyramid[10].m_blockedBy = { 5, 6 };
        m_pyramid[11].m_blockedBy = { 6, 7 };
        m_pyramid[12].m_blockedBy = { 7, 8 };
        m_pyramid[13].m_blockedBy = { 8 };

		m_pyramid[14].m_blockedBy = { 9 };
		m_pyramid[15].m_blockedBy = { 9, 10 };
        m_pyramid[16].m_blockedBy = { 10, 11 };
        m_pyramid[17].m_blockedBy = { 11, 12 };
        m_pyramid[18].m_blockedBy = { 12, 13 };
		m_pyramid[19].m_blockedBy = { 13 };
    }

    if (age == 3) {
        // age III
        for (int i = 0; i <= 1; i++) m_pyramid[i].m_isFaceUp = true;
        for (int i = 2; i <= 4; i++) m_pyramid[i].m_isFaceUp = false;
        for (int i = 5; i <= 8; i++) m_pyramid[i].m_isFaceUp = true;
        for (int i = 9; i <= 10; i++) m_pyramid[i].m_isFaceUp = false;
        for (int i = 11; i <= 14; i++) m_pyramid[i].m_isFaceUp = true;
		for (int i = 15; i <= 17; i++) m_pyramid[i].m_isFaceUp = false;
		for (int i = 18; i <= 19; i++) m_pyramid[i].m_isFaceUp = true;

        // Dependinte carti age III
		m_pyramid[2].m_blockedBy = { 0 };
		m_pyramid[3].m_blockedBy = { 0, 1 };
        m_pyramid[4].m_blockedBy = { 1 };

        m_pyramid[5].m_blockedBy = { 2 };
        m_pyramid[6].m_blockedBy = { 2, 3 };
        m_pyramid[7].m_blockedBy = { 3, 4 };
        m_pyramid[8].m_blockedBy = { 4 };

        m_pyramid[9].m_blockedBy = { 5, 6 };
        m_pyramid[10].m_blockedBy = { 7, 8 };

        m_pyramid[11].m_blockedBy = { 9 };
        m_pyramid[12].m_blockedBy = { 9 };
        m_pyramid[13].m_blockedBy = { 10 };
        m_pyramid[14].m_blockedBy = { 10 };

        m_pyramid[15].m_blockedBy = { 11, 12 };
        m_pyramid[16].m_blockedBy = { 12, 13 };
        m_pyramid[17].m_blockedBy = { 13, 14 };

        m_pyramid[18].m_blockedBy = { 15, 16 };
		m_pyramid[19].m_blockedBy = { 16, 17 };
    }
}

GameState::GameState():m_currentPlayerIndex(0),
m_currentTurn(1),
m_winnerIndex(std::nullopt),
m_currentAge(1),
m_gameOver(false)
{
    m_players[0] = std::make_unique<Player>("Player 1");
    m_players[1] = std::make_unique<Player>("Player 2");
}

GameState::GameState(const std::string& p1Name, const std::string& p2Name)
    : m_currentPlayerIndex(0),
    m_currentTurn(1),
    m_winnerIndex(std::nullopt),
    m_currentAge(1),
    m_gameOver(false)
{

    m_players[0] = std::make_unique<Player>(p1Name);
    m_players[1] = std::make_unique<Player>(p2Name);
}

void GameState::switchPlayer()
{ 
	m_currentPlayerIndex = 1 - m_currentPlayerIndex;
}

std::array<std::unique_ptr<Player>, GameConstants::NUMBER_OF_PLAYERS>& GameState::getPlayers()
{
	return m_players;
}

const Player& GameState::getCurrentPlayer() const
{
	return *m_players[m_currentPlayerIndex];
}

const Player& GameState::getOpponent() const
{
	return *m_players[1 - m_currentPlayerIndex];
}

Player& GameState::getCurrentPlayer()
{
    return *m_players[m_currentPlayerIndex];
}

Player& GameState::getOpponent()
{
    return *m_players[1 - m_currentPlayerIndex];
}

uint8_t GameState::getCurrentAge() const
{
	return m_currentAge;
}

bool GameState::isGameOver() const
{
	return m_gameOver;
}

void GameState::setGameOver(bool over) { m_gameOver = over; }

std::vector<std::unique_ptr<Wonder>>& GameState::getAllWonders()
{
	return m_allWonders;
}

int GameState::getAllConstructedWondersCount() const
{
    int total = 0;
    for (const auto& player : m_players) {
        if (player) {
            total += player->getConstructedWondersCount();
        }
    }
    return total;
}

GamePhase GameState::getCurrentPhase() const
{
	return m_currentPhase;
}

void GameState::setCurrentPhase(GamePhase phase)
{
	m_currentPhase = phase;
}

const std::vector<std::unique_ptr<Wonder>>& GameState::getDraftedWonders() const
{
	return m_draftedWonders;
}

void GameState::clearDraftedWonders()
{
	m_draftedWonders.clear();
}

void GameState::addWonderToDraft(std::unique_ptr<Wonder> wonder)
{
    if(wonder) m_draftedWonders.push_back(std::move(wonder));
}

std::unique_ptr<Wonder> GameState::extractWonderFromDraft(int index)
{
    if (index < 0 || index >= m_draftedWonders.size()) return nullptr;

    std::unique_ptr<Wonder> w = std::move(m_draftedWonders[index]);
    m_draftedWonders.erase(m_draftedWonders.begin() + index);

    return w;
}

const std::vector<CardNode>& GameState::getPyramid() const
{
    return m_pyramid;
}

const std::optional<uint8_t>& GameState::getWinnerIndex() const
{
	return m_winnerIndex;
}

std::optional<std::reference_wrapper<const Card>> GameState::getCardView(int index) const
{
    if (index >= 0 && index < m_currentAgeCards.size() && m_currentAgeCards[index]) {
        return std::cref(*m_currentAgeCards[index]);
    }
    return std::nullopt;
}

void GameState::initializeAge(int age, std::vector<std::unique_ptr<Card>>& deck)
{
    m_currentAge = age;

    int i = 0;
    for (auto& card : deck) {
        if (i < 20 && card != nullptr) {
            m_currentAgeCards[i] = std::move(card);
            i++;
        }
    }

    buildPyramidStructure(age);
}

bool GameState::isCardAccessible(int index) const
{
    if (index < 0 || index >= m_pyramid.size()) return false;
    if (m_pyramid[index].m_isRemoved) return false;

    for (int blocker : m_pyramid[index].m_blockedBy)
        if (!m_pyramid[blocker].m_isRemoved)
            return false;

    return true;
}

void GameState::removeCardFromPyramid(int index)
{
    if (index < 0 || index >= 20) return;

    m_pyramid[index].m_isRemoved = true;

    for (auto& node : m_pyramid) {
		if (node.m_isRemoved) continue; // sarim cartile deja eliminate

        // e blocata?
        bool isBlocked = false;
        for (int blockerIndex : node.m_blockedBy) {
			// o carte care blocheaza inca nu a fost eliminata
            if (!m_pyramid[blockerIndex].m_isRemoved) {
                isBlocked = true;
                break;
            }
        }

		// o intorc daca nu e blocata
        if (!isBlocked) {
            node.m_isFaceUp = true;
        }
    }
}

std::unique_ptr<Card> GameState::takeCard(int index)
{
    removeCardFromPyramid(index);

    if (index >= 0 && index < m_currentAgeCards.size()) {
        return std::move(m_currentAgeCards[index]);
    }
    return nullptr;
}


bool GameState::saveGame(std::string&& filename) const
{
    try
    {
        json j = *this;

        std::ofstream file(filename);
        if (!file.is_open()) {

            return false;
        }

        file << j.dump(4);
        file.close();
        return true;
    }
    catch (const nlohmann::json::exception& e)
    {
        return false;
    }
}

bool GameState::loadGame(std::string&& filename)
{
    try
    {
        std::ifstream file(filename);
        if (!file.is_open()) {
            std::cerr << "Eroare: Nu s-a putut deschide fisierul" << filename<< " pentru incarcare : " << std::endl;
            return false;
        }

        nlohmann::json j;
        file >> j;
        file.close();

        from_json(j, *this);

        return true;
    }
    catch (const nlohmann::json::exception& e)
    {
        std::cerr << "Eroare la deserializare JSON : " << e.what() << std::endl;
        return false;
    }
}

const std::vector<std::unique_ptr<ProgressToken>>& GameState::getAvailableTokens() const
{
    return m_availableProgressToken;
}

void GameState::addToAvailableTokens(std::unique_ptr<ProgressToken>&& token)
{
    if (token)
       m_availableProgressToken.push_back(std::move(token));
}


std::vector<std::unique_ptr<Card>>& GameState::getDiscardedCards()
{
    return m_discardedCards;
}

std::unique_ptr<ProgressToken> GameState::removeAvailableTokens(int index)
{
    if (index >= m_availableProgressToken.size() || index < 0)
    {
        return nullptr;
    }

    auto it = m_availableProgressToken.begin() + index;
    std::unique_ptr<ProgressToken> extractedToken = std::move(*it);
    m_availableProgressToken.erase(it);

    return extractedToken;
}

const std::vector<std::unique_ptr<Card>>& GameState::getDiscardedCards() const
{
    return m_discardedCards;
}

void GameState::addToDiscardCards(std::unique_ptr<Card>&& card)
{
    if(card)
        m_discardedCards.push_back(std::move(card));
}

std::unique_ptr<Card> GameState::extractDiscardedCard(int index)
{
    if (index >= m_discardedCards.size() || index<0)
    {
        return nullptr;
    }

    auto it = m_discardedCards.begin() + index;
    std::unique_ptr<Card> extractedCard = std::move(*it);
    m_discardedCards.erase(it);

    return extractedCard;
}

std::vector<std::unique_ptr<ProgressToken>>& GameState::getDiscardedTokens()
{
    return m_discardedProgressTokens;
}

const std::vector<std::unique_ptr<ProgressToken>>& GameState::getDiscardedTokens() const
{
    return m_discardedProgressTokens;
}

void GameState::addToDiscardTokens(std::unique_ptr<ProgressToken>&& token)
{
    if(token)
        m_discardedProgressTokens.push_back(std::move(token));
}

std::unique_ptr<ProgressToken> GameState::extractDiscardedTokens(int index)
{
    if (index >= m_discardedProgressTokens.size() || index < 0)
    {
        return nullptr;
    }

    auto it = m_discardedProgressTokens.begin() + index;
    std::unique_ptr<ProgressToken> extractedToken = std::move(*it);
    m_discardedProgressTokens.erase(it);

    return extractedToken;
}

GameState::~GameState() = default;

bool GameState::removeMilitaryToken(int index)
{
    if (index < 0 || index > 1) return false;
    if (!m_militaryTokensDropped[index]) {
        m_militaryTokensDropped[index] = true; 
        return true; 
    }
    return false;
}

bool GameState::hasPendingScientificReward() const
{
    return m_pendingScientificReward;
}

void GameState::setPendingScientificReward(bool pending)
{
    m_pendingScientificReward = pending;
}

uint8_t GameState::getCurrentPlayerIndex() const
{
    return m_currentPlayerIndex;
}
void GameState::setWinner(uint8_t index)
{
    m_winnerIndex = index;
}
int GameState::getMilitaryPosition() const
{
    int player1Shields = m_players[0]->getMilitaryShields();
    int player2Shields = m_players[1]->getMilitaryShields();
	return player1Shields - player2Shields;
}

Player& GameState::getPlayerWithMostCardsPerColor(const CardColor& color)
{
    Player& player1 = getCurrentPlayer();
    Player& player2 = getOpponent();
    if (player1.getCardPerColor(color) > player2.getCardPerColor(color))
        return player1;
    return player2;
}

void to_json(json& j, const GameState& state)
{
    j = nlohmann::json{
        {"players", state.m_players},
        {"currentPlayerIndex", state.m_currentPlayerIndex},
        {"currentTurn", state.m_currentTurn},
        {"winnerIndex", state.m_winnerIndex},
        {"ageIDeck", state.m_ageIDeck},
        {"ageIIDeck", state.m_ageIIDeck},
        {"ageIIIDeck", state.m_ageIIIDeck},
        {"currentAge", state.m_currentAge},
        {"currentAgeCards", state.m_currentAgeCards},
        {"pyramid", state.m_pyramid},
        {"allWonders", state.m_allWonders},
        {"availableProgressToken", state.m_availableProgressToken},
        {"gameOver", state.m_gameOver},
        {"militaryTokensDropped", state.m_militaryTokensDropped},
        {"pendingScientificReward", state.m_pendingScientificReward}
    };
}

void from_json(const json& j, GameState& state)
{
    j.at("players").get_to(state.m_players);
    j.at("currentPlayerIndex").get_to(state.m_currentPlayerIndex);
    j.at("currentTurn").get_to(state.m_currentTurn);
    state.m_winnerIndex = get_optional<uint8_t>(j, "winnerIndex");
    j.at("ageIDeck").get_to(state.m_ageIDeck);
    j.at("ageIIDeck").get_to(state.m_ageIIDeck);
    j.at("ageIIIDeck").get_to(state.m_ageIIIDeck);
    j.at("currentAge").get_to(state.m_currentAge);
    j.at("currentAgeCards").get_to(state.m_currentAgeCards);
    j.at("pyramid").get_to(state.m_pyramid);
    j.at("allWonders").get_to(state.m_allWonders);
    j.at("availableProgressToken").get_to(state.m_availableProgressToken);
    j.at("gameOver").get_to(state.m_gameOver);
    state.m_militaryTokensDropped = j.value("militaryTokensDropped", std::array<bool, 2>{false, false});
    state.m_pendingScientificReward = j.value("pendingScientificReward", false);
}
	
