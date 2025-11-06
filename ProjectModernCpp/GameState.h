#pragma once

#include <cstdint>
#include <memory>
#include <random>
#include "Constants.h"
#include "Card.h"
#include "Player.h"
#include "Wonder.h"

//class Player;
//class Card;
//class Wonder; de revizuit cu forward declarations


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
	std::array<bool, GameConstants::CARDS_PER_AGE> m_cardAvailability;

	std::vector<std::unique_ptr<Wonder>> m_allWonders;
	std::vector<std::unique_ptr<ProgressToken>> m_availableProgressToken;

	bool m_gameOver;
	//std::optional<VictoryType> m_victoryType;
	std::mt19937 m_rng;

public:

	GameState();

	void switchPlayer();

	const Player& getCurrentPlayer() const;
	const Player& getOpponent() const;
	uint8_t getCurrentAge() const;
	bool isGameOver() const;
	const std::optional<uint8_t>& getWinnerIndex() const;
	// VictoryType getVictoryType() const;

	Player& getCurrentPlayer(); //pentru GameController
	Player& getOpponent(); //pentru GameController

	bool saveGame(std::string&& filename) const;
	bool loadGame(std::string&& filename);

	~GameState();

};

