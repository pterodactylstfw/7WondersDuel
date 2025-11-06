#include "Player.h"

Player::Player(const std::string& playerName) : 
	name(playerName), coins(GameConstants::STARTING_COINS), militaryShields(0), victoryPoints(0) { }

void Player::addCard(std::unique_ptr<Card>&& card)
{
	constructedCards.push_back(std::move(card));
}

void Player::addWonder(std::unique_ptr<Wonder>&& wonder, int index)
{
	if (index < 0 || index > 4) return;
	wonders[index] = std::move(wonder);
}

void Player::addProgressToken(std::unique_ptr<ProgressToken>&& token)
{
	progressTokens.push_back(std::move(token));
}




