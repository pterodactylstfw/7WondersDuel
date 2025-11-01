#pragma once

#include <iostream>
#include <map>

#include "Constants.h"

class CardEffect
{
private:

	bool m_grantsProgressToken;

	int m_coinsPerWonder;
	std::map<CardColor, int> m_coinsPerCardType;

	int m_pointsPerWonder;
	std::map<CardColor, int> m_pointsPerCardType;

	bool m_playAgain;
	bool m_countOpponentCards;
	bool m_copyGuild;

	// ResourceProduction m_production; 
	ScientificSymbol m_scienceSymbol;

	int m_victoryPoints;
	int m_shields;
	int m_baseCoins;

	int m_discountRaw;
	int m_discountManufactured;


public:

	CardEffect() = default;
	bool isEmpty() const;

};

