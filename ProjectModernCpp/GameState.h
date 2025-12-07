#pragma once

#include <cstdint>
#include <memory>
#include <random>
#include "Constants.h"
#include "Card.h"
#include "Player.h"
#include "Wonder.h"
#include "ProgressToken.h"
#include "JsonUtils.h"

//class Player;
//class Card;
//class Wonder; de revizuit cu forward declarations


struct CardNode {
	int m_index;// index in m_currentAgeCards
	bool m_isFaceUp; // daca e cu fata in suseUp;
	bool m_isRemoved; // daca a fost deja luataoved;
	std::vector<int> m_blockedBy; // indecsii cartilor care blocheaza aceasta carteint> m_blockedBy;
};

NLOHMANN_DEFINE_TYPE_NON_INTRUSIVE(CardNode, m_index, m_isFaceUp, m_isRemoved, m_blockedBy)

class GameState
{
private:	

	std::array<std::unique_ptr<Player>, GameConstants::NUMBER_OF_PLAYERS> m_players;
	uint8_t m_currentPlayerIndex;
	uint8_t m_currentTurn;
	std::optional<uint8_t> m_winnerIndex;

	std::vector<std::unique_ptr<Card>> m_ageIDeck;
	std::vector<std::unique_ptr<Card>> m_ageIIDeck;
	std::vector<std::unique_ptr<Card>> m_ageIIIDeck;
	uint8_t m_currentAge;

	std::array<std::unique_ptr<Card>, GameConstants::CARDS_PER_AGE> m_currentAgeCards;

	std::vector<std::unique_ptr<Wonder>> m_allWonders;
	std::vector<std::unique_ptr<ProgressToken>> m_availableProgressToken;

	std::vector<std::unique_ptr<Card>> m_discardedCards;
	std::vector<std::unique_ptr<ProgressToken>> m_discardedProgressTokens;

	bool m_gameOver;
	//std::optional<VictoryType> m_victoryType;

	std::vector<CardNode> m_pyramid;
	void buildPyramidStructure(int age);
	std::array<bool, 2> m_militaryTokensDropped = { false, false };
	bool m_pendingScientificReward = false; 
public:

	GameState();
	GameState(const std::string& p1Name, const std::string& p2Name);

	void switchPlayer();

	const Player& getCurrentPlayer() const;
	const Player& getOpponent() const;
	Player& getCurrentPlayer();
	Player& getOpponent();
	uint8_t getCurrentPlayerIndex() const;
	void setWinner(uint8_t index);

    Player& getPlayerWithMostCardsPerColor(const CardColor& color);

	uint8_t getCurrentAge() const;
	bool isGameOver() const;
	void setGameOver(bool over);

	int getAllConstructedWondersCount() const;

	const std::vector<CardNode>& getPyramid() const;

	const std::optional<uint8_t>& getWinnerIndex() const;

	const Card* getCardPtr(int index) const; // pt verif costuri


	void initializeAge(int age, std::vector<std::unique_ptr<Card>>& deck);
	bool isCardAccessible(int index) const;
	void removeCardFromPyramid(int index); // pick card
	std::unique_ptr<Card> takeCard(int index); // transfer de detinator carte intre playeri

	bool saveGame(std::string&& filename) const;
	bool loadGame(std::string&& filename);

	const std::vector<std::unique_ptr<ProgressToken>>& getAvailableTokens() const;
	void addToAvailableTokens(std::unique_ptr<ProgressToken>&& token);
	std::unique_ptr<ProgressToken> removeAvailableTokens(int index);

	std::vector<std::unique_ptr<Card>>& getDiscardedCards();
	const std::vector<std::unique_ptr<Card>>& getDiscardedCards() const;

	void addToDiscardCards(std::unique_ptr<Card>&& card);
	std::unique_ptr<Card> extractDiscardedCard(int index);

	std::vector<std::unique_ptr<ProgressToken>>& getDiscardedTokens();
	const std::vector<std::unique_ptr<ProgressToken>>& getDiscardedTokens() const;

	void addToDiscardTokens(std::unique_ptr<ProgressToken>&& token);
	std::unique_ptr<ProgressToken> extractDiscardedTokens(int index);

	bool removeMilitaryToken(int index); 
	bool hasPendingScientificReward() const;
	void setPendingScientificReward(bool pending);

	friend void to_json(json& j, const GameState& state);
	friend void from_json(const json& j, GameState& state);

	~GameState();

};

