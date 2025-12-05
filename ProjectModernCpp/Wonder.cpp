#include "Wonder.h"

Wonder::Wonder(const std::string& name, Cost cost, CardEffect effect)
	: m_name(name), m_cost(std::move(cost)), m_isBuilt(false), m_effect(std::move(effect)) 
{ }

Wonder::Wonder(const std::string& name, Cost cost)
	: m_name(name), m_cost(std::move(cost)), m_isBuilt(false)
{ }

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
	std::stringstream ss;
	ss << "Wonder: " << m_name << "\n";

	if(this->m_cost.isFree()) {
		ss << "Cost: Free\n";
	}
	else {
		ss << "Cost: " << m_cost.toString() << "\n";
	}

	if(this->m_effect.isEmpty()) {
		ss << "Effect: None\n";
	} else {
		ss << "Effect: " << m_effect.getDescription() << "\n";
	}

	ss << "Status: " << (m_isBuilt ? "Built" : "Not built") << "\n";
	
	return ss.str();
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
		{"effect", wonder.m_effect }
	};
}

void from_json(const json& j, Wonder& wonder)
{
	j.at("name").get_to(wonder.m_name);
	j.at("cost").get_to(wonder.m_cost);
	j.at("isBuilt").get_to(wonder.m_isBuilt);
	j.at("effect").get_to(wonder.m_effect);
}
