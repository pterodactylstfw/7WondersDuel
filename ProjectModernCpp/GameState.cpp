#include "GameState.h"

GameState::GameState():m_currentPlayerIndex(0),
m_currentTurn(1),
m_winnerIndex(std::nullopt),
m_currentAge(1),
m_gameOver(false),
m_rng(std::random_device{}())
{}

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

uint8_t GameState::getCurrentAge() const
{
	return m_currentAge;
}

bool GameState::isGameOver() const
{
	return m_gameOver;
}

const std::optional<uint8_t>& GameState::getWinnerIndex() const
{
	return m_winnerIndex;
}

Player& GameState::getCurrentPlayer()
{
	return *m_players[m_currentPlayerIndex];
}

Player& GameState::getOpponent()
{
	return *m_players[1 - m_currentPlayerIndex];
}

GameState::~GameState() = default;
