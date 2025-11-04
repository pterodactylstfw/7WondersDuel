#include "CardEffect.h"
#include <sstream>

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
		m_production.isEmpty() &&
		m_scienceSymbol == ScientificSymbol::NONE &&
		m_victoryPoints == 0 &&
		m_shields == 0 &&
		m_baseCoins == 0 &&
		m_discountRaw == 0 &&
		m_discountManufactured == 0;
}

std::string CardEffect::getDescription() const
{
    std::stringstream ss;

    if(m_grantsProgressToken) {
        ss << "Oferă un token de progres.\n";
	}

    if (m_victoryPoints > 0) {
        ss << "Oferă " << m_victoryPoints << " puncte de victorie.\n";
    }

    if (m_baseCoins > 0) {
        ss << "Oferă " << m_baseCoins << " monede.\n";
    }

    if (m_shields > 0) {
        ss << "Oferă " << m_shields << " scuturi militare.\n";
    }

    std::string productionDesc = m_production.getDescription(); 
    if (!productionDesc.empty()) {
        ss << "Produce: " << productionDesc << ".\n";
    }

    if (m_scienceSymbol != ScientificSymbol::NONE) {
        ss << "Oferă simbolul științific: " << scientificSymbolToString(m_scienceSymbol) << ".\n";
    }

    if (m_playAgain) {
        ss << "Permite jucătorului să mai joace o tură. ";
    }

    if (m_discountRaw > 0) {
        ss << "Oferă discount de " << m_discountRaw << " la resursele brute (lemn, piatră, argilă).\n";
    }

    if (m_discountManufactured > 0) {
        ss << "Oferă discount de " << m_discountManufactured << " la resursele manufacturate (sticlă, papirus).\n";
    }

    if (!m_coinsPerCardType.empty()) {
        ss << "Oferă monede pentru fiecare carte de tip ";
        for (auto const& [color, coins] : m_coinsPerCardType) {
            ss << colorToString(color) << " (" << coins << " monede/carte).\n";
        }
    }

    if (!m_pointsPerCardType.empty()) {
        ss << "Oferă puncte la final pentru fiecare carte de tip ";
        for (auto const& [color, points] : m_pointsPerCardType) {
            ss << colorToString(color) << " (" << points << " puncte/carte).\n";
        }
    }

    if(m_coinsPerWonder > 0) {
        ss << "Oferă " << m_coinsPerWonder << " monede pentru minunea construită.\n";
	}

    if (m_pointsPerWonder > 0) {
        ss << "Oferă " << m_pointsPerWonder << " puncte pentru minunea construită.\n";
    }

    if (m_countOpponentCards) {
        ss << "Numără și cărțile adversarilor pentru efectele sale.\n";
    }

    if(m_copyGuild) {
        ss << "Permite copierea efectului unei carti Guild adversare.\n";
	}

    std::string result = ss.str();

    if (result.empty()) {
        return "Niciun efect special.";
    }

    return result;
}