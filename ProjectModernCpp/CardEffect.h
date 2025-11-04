#pragma once

#include <iostream>
#include <map>

#include "Constants.h"
#include "ResourceProduction.h"

class CardEffect
{
private:

	bool m_grantsProgressToken = false;

	int m_coinsPerWonder = 0;
	std::map<CardColor, int> m_coinsPerCardType;

	int m_pointsPerWonder = 0;
	std::map<CardColor, int> m_pointsPerCardType;

	bool m_playAgain = false;
	bool m_countOpponentCards = false;
	bool m_copyGuild = false;

	ResourceProduction m_production; 
	ScientificSymbol m_scienceSymbol = ScientificSymbol::NONE;

	int m_victoryPoints = 0;
	int m_shields = 0;
	int m_baseCoins = 0;

	int m_discountRaw = 0;
	int m_discountManufactured = 0;


public:

	CardEffect() = default;

	bool isEmpty() const;
	
	std::string getDescription() const;

};

