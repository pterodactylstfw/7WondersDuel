#include "Wonder.h"

Wonder::Wonder(std::string&& name, Cost& cost):m_name(name), m_cost(cost), m_isBuilt(false), m_effectDescription(""), m_victoryPoints(0) {}

std::string_view Wonder::getName() const {
	return m_name;
}

const Cost& Wonder::getCost() const {
	return m_cost;
}

bool Wonder::isBuilt() const {
	return m_isBuilt;
}

std::string_view Wonder::getDescription() const {
	return m_effectDescription;
}

uint8_t Wonder::getVictoryPoints() const {
	return m_victoryPoints;
}

void to_json(json& j, const Wonder& wonder)
{
	j = json{
		{"name", wonder.m_name},
		{"cost", wonder.m_cost},
		{"isBuilt", wonder.m_isBuilt},
		{"effectDescription", wonder.m_effectDescription},
		{"victoryPoints", wonder.m_victoryPoints}
	};
}

void from_json(const json& j, Wonder& wonder)
{
	j.at("name").get_to(wonder.m_name);
	j.at("cost").get_to(wonder.m_cost);
	j.at("isBuilt").get_to(wonder.m_isBuilt);
	j.at("effectDescription").get_to(wonder.m_effectDescription);
	j.at("victoryPoints").get_to(wonder.m_victoryPoints);
}
