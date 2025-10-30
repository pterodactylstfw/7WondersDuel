#pragma once

#include <string>
#include "Cost.h"

class Wonder
{
private:
	std::string m_name;
	Cost m_cost;
	bool m_isConstructed;
	std::string m_effectDescription;
	uint8_t m_victoryPoints;

public:

	Wonder(std::string&& name, Cost& cost) :m_name(name), m_cost(cost), m_isConstructed(false), m_effectDescription(""), m_victoryPoints(0) {}
	
	Wonder(const Wonder& other) = default;
	
	Wonder& operator=(const Wonder& other) = default;
	
	Wonder(Wonder&& other) noexcept = default;
	
	Wonder& operator=(Wonder&& other) noexcept = default;

	// for the next 2 functions, waiting for commits from my teammates:

	// void constructWonder(Player& owner, Player& opponent, GameState& state);

	// virtual void applyEffect(Player& owner, Player& opponent, GameState & state);


	std::string_view getName() const;
	const Cost& getCost() const;
	bool isBuilt() const;
	std::string_view getDescription() const;
	uint8_t getVictoryPoints() const;


	~Wonder() = default;
};

