#pragma once

#include <string>
#include "Cost.h"
#include "CardEffect.h"

class Wonder
{
private:
	std::string m_name;
	Cost m_cost;
	bool m_isBuilt;
	std::optional<int> m_victoryPoints;
	CardEffect m_effect;

public:
	
	Wonder(std::string_view name, Cost cost, std::optional<int> victoryPoints, CardEffect effect);
	Wonder(std::string_view name, Cost cost, std::optional<int> victoryPoints);
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
	std::optional<int> getVictoryPoints() const;

	~Wonder() = default;
};

