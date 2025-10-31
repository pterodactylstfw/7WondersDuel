#include "Card.h"

bool Card::canBeBuiltFreelyAfter(const Card& previousCard) const
{
	return (m_freeChainFrom == previousCard.getName());
}

bool Card::canBuildFreeChainTo(const Card& nextCard) const
{
	return (m_providesChainTo == nextCard.getName());
}

void Card::displayCardInfo() const
{
	std::string colorStr = colorToString(m_color); // from Constants.h

	std::cout << "Card Name: " << m_name << "\n";
	std::cout << "Color: " << colorStr << "\n";
	std::cout << "Age: " << static_cast<int>(m_age) << "\n";
	if (!m_freeChainFrom.empty()) {
		std::cout << "Can be built freely after: " << m_freeChainFrom << "\n";
	}
	if (!m_providesChainTo.empty()) {
		std::cout << "Provides free chain to: " << m_providesChainTo << "\n";
	}
}

std::string Card::getName() const
{
	return m_name;
}

CardColor Card::getColor() const
{
	return m_color;
}

uint8_t Card::getAge() const
{
	return m_age;
}

std::string Card::getFreeChainFrom() const
{
	return m_freeChainFrom;
}

std::string Card::getProvidesChainTo() const
{
	return m_providesChainTo;
}

void Card::setFreeChainFrom(const std::string_view cardName)
{
	m_freeChainFrom = cardName;
}

void Card::setProvidesChainTo(const std::string_view cardName)
{
	m_providesChainTo = cardName;
}
