#include "Wonder.h"

Wonder::Wonder(WonderType type, Cost cost, uint8_t victoryPoints, CardEffect effect)
	: m_type(type) , m_cost(std::move(cost)), m_isBuilt(false), 
	  m_victoryPoints(victoryPoints), m_effect(std::move(effect)) {
	m_name = wonderTypeToString(type);
}

Wonder::Wonder(WonderType type, Cost cost, uint8_t victoryPoints)
	: m_type(type), m_cost(std::move(cost)), m_isBuilt(false),
	m_victoryPoints(victoryPoints) {
m_name = wonderTypeToString(type);}

std::string_view Wonder::getName() const {
	return m_name;
}

const Cost& Wonder::getCost() const {
	return m_cost;
}

const CardEffect& Wonder::getEffect() const {
	return m_effect;
}

std::string Wonder::getDescription() const
{
	return m_effect.getDescription();
}

bool Wonder::isBuilt() const {
	return m_isBuilt;
}

std::string Wonder::toString() const {
	return "Wonder: " + m_name + "\nCost: " + m_cost.toString() +
		"\nEffect: " + m_effect.getDescription() +
		"\nVictory Points: " + std::to_string(m_victoryPoints) +
		"\nStatus: " + (m_isBuilt ? "Built" : "Not built") + "\n";
}

uint8_t Wonder::getVictoryPoints() const {
	return m_victoryPoints;
}

WonderType Wonder::getWonderType() const
{
	return m_type;
}

void to_json(json& j, const Wonder& wonder)
{
	j = json{
		{"name", wonder.m_name},
		{"cost", wonder.m_cost},
		{"isBuilt", wonder.m_isBuilt},
		{"victoryPoints", wonder.m_victoryPoints},
		{"effect", wonder.m_effect }
	};
}

void from_json(const json& j, Wonder& wonder)
{
	j.at("name").get_to(wonder.m_name);
	j.at("cost").get_to(wonder.m_cost);
	j.at("isBuilt").get_to(wonder.m_isBuilt);
	j.at("victoryPoints").get_to(wonder.m_victoryPoints);
	j.at("effect").get_to(wonder.m_effect);
}
