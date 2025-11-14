#pragma once

#include <string>
#include "Cost.h"
#include "CardEffect.h"

using json = nlohmann::json;

class Wonder
{
private:
	std::string m_name;
	Cost m_cost;
	bool m_isBuilt;
	uint8_t m_victoryPoints;
	std::string m_effectDescription;
	CardEffect m_effect;

public:

	Wonder() = default;

	Wonder(std::string&& name, Cost& cost);
	
	Wonder(const Wonder& other) = default;

	Wonder& operator=(const Wonder& other) = default;

	Wonder(Wonder&& other) noexcept = default;

	Wonder& operator=(Wonder&& other) noexcept = default;

	Wonder(std::string_view name, Cost cost, uint8_t victoryPoints, CardEffect effect);
	Wonder(std::string_view name, Cost cost, uint8_t victoryPoints);

	// for the next 2 functions, waiting for commits from my teammates:

	// void constructWonder(Player& owner, Player& opponent, GameState& state);

	// virtual void applyEffect(Player& owner, Player& opponent, GameState & state);


	std::string_view getName() const;
	const Cost& getCost() const;
	bool isBuilt() const;
	std::string getDescription() const;
	uint8_t getVictoryPoints() const;

	friend void to_json(json& j, const Wonder& wonder);
	friend void from_json(const json& j, Wonder& wonder);

	~Wonder() = default;
};

