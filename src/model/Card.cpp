#include "Card.h"

Card::Card(std::string_view name, CardColor color, uint8_t age, Cost cost, CardEffect effect, std::string imagePath,
	std::optional<std::string> freeChainFrom, std::optional<std::string> providesChainTo) :
	m_name(name),
	m_color(color),
	m_age(age),
	m_cost(std::move(cost)),
	m_effect(std::move(effect)),
	m_imagePath(imagePath),
	m_freeChainFrom(std::move(freeChainFrom)),
	m_providesChainTo(std::move(providesChainTo)) {
}

Card::Card(std::string_view name, CardColor color, uint8_t age, Cost cost, CardEffect effect, std::string imagePath) :
	Card(name, color, age, std::move(cost), std::move(effect), imagePath, std::nullopt, std::nullopt) {
}

bool Card::canBeBuiltFreelyAfter(const Card& previousCard) const
{
	return (m_freeChainFrom.has_value() && m_freeChainFrom.value() == previousCard.getName());
}

bool Card::canBuildFreeChainTo(const Card& nextCard) const
{
	return (m_providesChainTo.has_value() && m_providesChainTo.value() == nextCard.getName());
}

std::string Card::displayCardInfo() const
{
	std::stringstream ss;
	std::string colorStr = colorToString(m_color); // from Constants.h

	ss << "Card Name: " << m_name << "\n";
	ss << "Color: " << colorStr << "\n";
	ss << "Age: " << static_cast<int>(m_age) << "\n";
	ss << "Cost: " << m_cost.toString() << "\n";
	ss << "Effect: " << m_effect.getDescription() << "\n";
	if (m_freeChainFrom.has_value()) {
		ss << "Can be built freely after: " << m_freeChainFrom.value() << "\n";
	}
	if (m_providesChainTo.has_value()) {
		ss << "Provides free chain to: " << m_providesChainTo.value() << "\n";
	}
	return ss.str();
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

std::string Card::getImagePath() const
{
	std::string path = ":/assets/";
	if (m_color == CardColor::PURPLE)
		path += "guilds/";
	else path += "age" + std::to_string(m_age) + '/';
	return path + m_imagePath;
}

const std::optional<std::string>& Card::getFreeChainFrom() const
{
	return m_freeChainFrom;
}

const std::optional<std::string>& Card::getProvidesChainTo() const
{
	return m_providesChainTo;
}

void to_json(json& j, const Card& card)
{
	j = json{
		{"name", card.m_name},
		{"color", card.m_color},
		{"age", card.m_age},
		{"cost", card.m_cost},
		{"effect", card.m_effect},
		{"freeChainFrom", card.m_freeChainFrom},
		{"providesChainTo", card.m_providesChainTo},
		{"imagePath", card.m_imagePath}
	};
}


void from_json(const json& j, Card& card)
{
	j.at("name").get_to(card.m_name);
	//card.m_color = static_cast<CardColor>(j.at("color").get<int>());
	j.at("color").get_to(card.m_color);
	j.at("age").get_to(card.m_age);
	j.at("cost").get_to(card.m_cost);
	j.at("effect").get_to(card.m_effect);
	card.m_freeChainFrom = get_optional<std::string>(j, "freeChainFrom");
	card.m_providesChainTo = get_optional<std::string>(j, "providesChainTo");
	if (j.contains("imagePath") && !j["imagePath"].is_null())
		card.m_imagePath = j["imagePath"].get<std::string>();
	else
		card.m_imagePath = "";
}
