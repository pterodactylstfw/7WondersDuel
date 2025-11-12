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
