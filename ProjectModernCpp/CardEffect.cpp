#include "CardEffect.h"

bool CardEffect::isEmpty() const
{
	return !m_grantsProgressToken &&
		m_coinsPerWonder == 0 &&
		m_coinsPerCardType.empty() &&
		m_pointsPerWonder == 0 &&
		m_pointsPerCardType.empty() &&
		!m_playAgain &&
		!m_countOpponentCards &&
		!m_copyGuild &&
		m_scienceSymbol == ScientificSymbol::NONE &&
		m_victoryPoints == 0 &&
		m_shields == 0 &&
		m_baseCoins == 0 &&
		m_discountRaw == 0 &&
		m_discountManufactured == 0;
	// && m_production.isEmpty();
}
