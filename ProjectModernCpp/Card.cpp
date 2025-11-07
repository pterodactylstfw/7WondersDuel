#include "Card.h"

Card::Card(std::string_view name, CardColor color, uint8_t age, Cost cost, CardEffect effect,
	std::optional<std::string> freeChainFrom, std::optional<std::string> providesChainTo) :
	m_name(name),
	m_color(color),
	m_age(age),
	m_cost(std::move(cost)),
	m_effect(std::move(effect)),
	m_freeChainFrom(std::move(freeChainFrom)),
	m_providesChainTo(std::move(providesChainTo)) {
}

Card::Card(std::string_view name, CardColor color, uint8_t age, Cost cost, CardEffect effect):
	Card(name, color, age, std::move(cost), std::move(effect), std::nullopt, std::nullopt) {
}

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
	std::cout << "Cost: " << m_cost.toString() << "\n";
	std::cout << "Effect: " << m_effect.getDescription() << "\n";
	if (m_freeChainFrom.has_value()) {
		std::cout << "Can be built freely after: " << m_freeChainFrom.value() << "\n";
	}
	if (m_providesChainTo.has_value()) {
		std::cout << "Provides free chain to: " << m_providesChainTo.value() << "\n";
	}
}


const std::string& Card::getName() const
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

const Cost& Card::getCost() const
{
	return m_cost;
}

const CardEffect& Card::getEffect() const
{
	return m_effect;
}

const std::optional<std::string>& Card::getFreeChainFrom() const
{
	return m_freeChainFrom;
}

const std::optional<std::string>& Card::getProvidesChainTo() const
{
	return m_providesChainTo;
}

//void Card::setFreeChainFrom(const std::string_view cardName)
//{
//	m_freeChainFrom = cardName;
//}
//
//void Card::setProvidesChainTo(const std::string_view cardName)
//{
//	m_providesChainTo = cardName;
//} // de sters, nu e nevoie de ele intrucat se initializeaza o singura data in constructor
