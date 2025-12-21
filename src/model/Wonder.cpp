#include "Wonder.h"

Wonder::Wonder(const std::string& name, Cost cost, CardEffect effect, std::string imagePath)
	: m_name(name), m_cost(std::move(cost)), m_isBuilt(false), m_effect(std::move(effect)), m_imagePath(std::move(imagePath))
{ }

Wonder::Wonder(const std::string& name, Cost cost, std::string imagePath)
	: m_name(name), m_cost(std::move(cost)), m_isBuilt(false), m_imagePath(std::move(imagePath))
{ }

std::string Wonder::getName() const {
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

std::string Wonder::getImagePath() const
{
	return ":/assets/wonders/" + m_imagePath;
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
