#include "Wonder.h"

Wonder::Wonder(std::string_view name, Cost cost, std::optional<int> victoryPoints, CardEffect effect) 
	: m_name(name) , m_cost(std::move(cost)), m_isBuilt(false), 
	  m_victoryPoints(victoryPoints), m_effect(std::move(effect)) { }

//constructor temporar ptr a nu avea erori 
//ptr cartile de nu au efect 
Wonder::Wonder(std::string_view name, Cost cost, std::optional<int> victoryPoints)
	: m_name(name), m_cost(std::move(cost)), m_isBuilt(false),
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

std::optional<int> Wonder::getVictoryPoints() const {
	return m_victoryPoints;
}