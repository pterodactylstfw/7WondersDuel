#include "Wonder.h"

Wonder::Wonder(WonderType type, Cost cost, uint8_t victoryPoints, CardEffect effect)
	: m_type(type) , m_cost(std::move(cost)), m_isBuilt(false), 
	  m_victoryPoints(victoryPoints), m_effect(std::move(effect)) { }

Wonder::Wonder(WonderType type, Cost cost, uint8_t victoryPoints)
	: m_type(type), m_cost(std::move(cost)), m_isBuilt(false),
	m_victoryPoints(victoryPoints) { }

std::string_view Wonder::getName() const {
	return m_name;
}

const Cost& Wonder::getCost() const {
	return m_cost;
}

bool Wonder::isBuilt() const {
	return m_isBuilt;
}

std::string Wonder::getDescription() const {
	return m_effectDescription;
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
		{"effectDescription", wonder.m_effectDescription},
		{"victoryPoints", wonder.m_victoryPoints},
		{"effect", wonder.m_effect }
	};
}

void from_json(const json& j, Wonder& wonder)
{
	j.at("name").get_to(wonder.m_name);
	j.at("cost").get_to(wonder.m_cost);
	j.at("isBuilt").get_to(wonder.m_isBuilt);
	j.at("effectDescription").get_to(wonder.m_effectDescription);
	j.at("victoryPoints").get_to(wonder.m_victoryPoints);
	j.at("effect").get_to(wonder.m_effect);
}
