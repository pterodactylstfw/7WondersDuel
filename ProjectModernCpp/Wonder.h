#pragma once

#include <string>
#include "Cost.h"
#include "CardEffect.h"
#include  "Constants.h"
#include "JsonUtils.h"

class Wonder
{
private:
	std::string m_name;
	Cost m_cost;
	bool m_isBuilt;
	uint8_t m_victoryPoints;
	CardEffect m_effect;
	WonderType m_type;

public:

	Wonder() = default;

	Wonder(std::string&& name, Cost& cost);
	
	Wonder(const Wonder& other) = default;

	Wonder& operator=(const Wonder& other) = default;

	Wonder(Wonder&& other) noexcept = default;

	Wonder& operator=(Wonder&& other) noexcept = default;

	Wonder(WonderType type, Cost cost, uint8_t victoryPoints, CardEffect effect);
	Wonder(WonderType type, Cost cost, uint8_t victoryPoints);

	// for the next 2 functions, waiting for commits from my teammates:

	// void constructWonder(Player& owner, Player& opponent, GameState& state);

	// virtual void applyEffect(Player& owner, Player& opponent, GameState & state);


	std::string_view getName() const;
	const Cost& getCost() const;
	const CardEffect& getEffect() const;

	bool isBuilt() const;
	std::string toString() const;
	
	uint8_t getVictoryPoints() const;
	WonderType getWonderType() const;

	friend void to_json(json& j, const Wonder& wonder);
	friend void from_json(const json& j, Wonder& wonder);

	~Wonder() = default;
};

