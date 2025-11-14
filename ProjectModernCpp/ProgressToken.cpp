#include "ProgressToken.h"
#include <sstream>
ProgressToken::ProgressToken(ProgressTokenType type, const std::string& name,
    const std::string& description, uint8_t victoryPoints)
    : m_tokenType(type), m_name(name), m_description(description),
    m_victoryPoints(victoryPoints), m_isActive(false) {}

ProgressTokenType ProgressToken::getType() const {
    return m_tokenType;
}
std::string_view ProgressToken::getName() const {
    return m_name;
}
std::string_view ProgressToken::getDescription() const {
    return m_description;
}
uint8_t ProgressToken::getVictoryPoints() const {
    return m_victoryPoints;
}

bool ProgressToken::isActive() const {
    return m_isActive;
}
void ProgressToken::setActive(bool active) {
    m_isActive = active;
}
std::string ProgressToken::toString() const {
    std::stringstream ss;
    ss << m_name << " (" << static_cast<int>(m_victoryPoints) << " VP)";
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
        {"description", token.m_description},
        {"victoryPoints", token.m_victoryPoints},
        {"isActive", token.m_isActive}
	};
}

void from_json(const json& j, ProgressToken& token)
{
    j.at("type").get_to(token.m_tokenType);
    j.at("name").get_to(token.m_name);
    j.at("description").get_to(token.m_description);
    j.at("victoryPoints").get_to(token.m_victoryPoints);
	j.at("isActive").get_to(token.m_isActive);
}
