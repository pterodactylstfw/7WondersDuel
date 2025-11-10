#include "ProgressToken.h"

ProgressToken::ProgressToken(ProgressTokenType type, const std::string& name,
    const std::string& description, uint8_t victoryPoints)
    : m_tokenType(type), m_name(name), m_description(description),
    m_victoryPoints(victoryPoints), m_isActive(false) {}

