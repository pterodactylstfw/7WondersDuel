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