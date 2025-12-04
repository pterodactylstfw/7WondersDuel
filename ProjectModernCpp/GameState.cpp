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

void GameState::switchPlayer()
{ 
	m_currentPlayerIndex = 1 - m_currentPlayerIndex;
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

const std::vector<CardNode>& GameState::getPyramid() const
{
    return m_pyramid;
}

const std::optional<uint8_t>& GameState::getWinnerIndex() const
{
	return m_winnerIndex;
}

const Card* GameState::getCardPtr(int index) const
{
    if (index >= 0 && index < m_currentAgeCards.size())
        return m_currentAgeCards[index].get();
    return nullptr;
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

const std::vector<std::unique_ptr<Card>>& GameState::getDiscardedCards() const
{
    return m_discardedCards;
}

void GameState::addToDiscardCards(std::unique_ptr<Card>&& card)
{
    if(card)
        m_discardedCards.push_back(std::move(card));
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

GameState::~GameState() = default;


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
        {"allWonders", state.m_allWonders},
        {"availableProgressToken", state.m_availableProgressToken},
        {"gameOver", state.m_gameOver}
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
    j.at("allWonders").get_to(state.m_allWonders);
    j.at("availableProgressToken").get_to(state.m_availableProgressToken);
    j.at("gameOver").get_to(state.m_gameOver);
}
	
