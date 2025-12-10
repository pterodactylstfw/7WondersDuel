#pragma once

#include <string>
#include <sstream>
#include "Cost.h"
#include "CardEffect.h"
#include "Constants.h"
#include "JsonUtils.h"

class Wonder
{
private:
	std::string m_name;
	Cost m_cost;
	bool m_isBuilt;
	CardEffect m_effect;

public:

	Wonder() = default;

	Wonder(const Wonder& other) = default;
	Wonder& operator=(const Wonder& other) = default;

	Wonder(Wonder&& other) noexcept = default;
	Wonder& operator=(Wonder&& other) noexcept = default;

	Wonder(const std::string& name, Cost cost, CardEffect effect);
	Wonder(const std::string& name, Cost cost);

	std::string getName() const;
	const Cost& getCost() const;
	const CardEffect& getEffect() const;
	std::string getDescription() const;

	bool isBuilt() const;
	std::string toString() const;

	friend void to_json(json& j, const Wonder& wonder);
	friend void from_json(const json& j, Wonder& wonder);

	~Wonder() = default;
};

