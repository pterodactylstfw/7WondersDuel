#pragma once

#include <string>
#include <iostream>
#include "Constants.h"
#include <optional>
#include <sstream>
#include "Cost.h"
#include "CardEffect.h"
#include "JsonUtils.h"


class Card
{
private:
	std::string m_name;
	CardColor m_color;
	uint8_t m_age;
	Cost m_cost;
	CardEffect m_effect;

	std::optional<std::string> m_freeChainFrom; // Card care ofera lant liber catre aceasta carte (daca exista)
	std::optional<std::string> m_providesChainTo; // Cardul pentru care aceasta carte ofera lant liber (daca exista)

public:
	Card() = default; // default constructor pentru json - pana la revizuire ulterioara - sa vad cum pot sa fac altfel

	Card(std::string_view name, CardColor color, uint8_t age, Cost cost, CardEffect effect,
		std::optional<std::string> freeChainFrom, std::optional<std::string> providesChainTo);

	Card(std::string_view name, CardColor color, uint8_t age, Cost cost, CardEffect effect);

	Card(const Card& other) = default;

	Card& operator = (const Card& other) = default;

	Card(Card&& other) = default;
	Card& operator = (Card&& other) = default;

	const std::string& getName() const;
	CardColor getColor() const;
	uint8_t getAge() const;

	const Cost& getCost() const;
	const CardEffect& getEffect() const;

	const std::optional<std::string>& getFreeChainFrom() const;
	const std::optional<std::string>& getProvidesChainTo() const;

	bool canBeBuiltFreelyAfter(const Card& previousCard) const;
	bool canBuildFreeChainTo(const Card& nextCard) const;

	void applyEffect(); // To be defined later
	std::string getEffectDescription() const; // To be defined later

	std::string displayCardInfo() const;

	friend void to_json(json& j, const Card& card);
	friend void from_json(const json& j, Card& card);


	~Card() = default;

};

