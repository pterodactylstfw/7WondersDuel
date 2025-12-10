#include "ProgressToken.h"
#include <sstream>

ProgressToken::ProgressToken(ProgressTokenType type, const std::string& name, CardEffect effect)
	: m_tokenType(type), m_name(name), m_effect(std::move(effect)), m_isActive(false)
{
}

ProgressTokenType ProgressToken::getType() const {
	return m_tokenType;
}

std::string_view ProgressToken::getName() const {
	return m_name;
}

std::string ProgressToken::getDescription() const {
	return m_effect.getDescription();
}

uint8_t ProgressToken::getVictoryPoints() const {
	return static_cast<uint8_t>(m_effect.getVictoryPointsPerCard().value_or(0));
}

CardEffect& ProgressToken::getEffect() {
	return m_effect;
}

bool ProgressToken::isActive() const {
	return m_isActive;
}

void ProgressToken::setActive(bool active) {
	m_isActive = active;
}

std::string ProgressToken::toString() const {
	std::stringstream ss;
	ss << m_name;

	int vp = getVictoryPoints();
	if (vp > 0) {
		ss << " (" << vp << " VP)";
	}

	if (m_isActive) {
		ss << " [ACTIVE]";
	}
	return ss.str();
}

std::ostream& operator<<(std::ostream& os, const ProgressToken& token) {
	os << token.toString();
	return os;
}

void to_json(json& j, const ProgressToken& token)
{
	j = json{
		{"type", token.m_tokenType},
		{"name", token.m_name},
		{"effect", token.m_effect},
		{"isActive", token.m_isActive}
	};
}

void from_json(const json& j, ProgressToken& token)
{
	j.at("type").get_to(token.m_tokenType);
	j.at("name").get_to(token.m_name);
	j.at("effect").get_to(token.m_effect);
	j.at("isActive").get_to(token.m_isActive);
}