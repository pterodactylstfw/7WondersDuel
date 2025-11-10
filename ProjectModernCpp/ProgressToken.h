#pragma once
#include "Constants.h"
#include <string>
#include <vector>
#include <iostream>

class ProgressToken{
private:
	ProgressTokenType m_tokenType;
	std::string m_name;
	std::string m_description;
	uint8_t m_victoryPoints;
	bool m_isActive;
public:
	ProgressToken(ProgressTokenType type, const std::string& name,
		const std::string& description, uint8_t victoryPoints);

	ProgressToken(const ProgressToken&) = default;
	ProgressToken& operator=(const ProgressToken&) = default;
	ProgressToken(ProgressToken&&) noexcept = default;
	ProgressToken& operator=(ProgressToken&&) noexcept = default;
	~ProgressToken() = default;
};

