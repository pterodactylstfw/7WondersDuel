#pragma once

#include <string>
#include <iostream>
#include "Constants.h"
#include <optional>
#include "Cost.h"
#include "CardEffect.h"

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

	Card(std::string_view name, CardColor color, uint8_t age, Cost cost, CardEffect effect,
		std::optional<std::string> freeChainFrom, std::optional<std::string> providesChainTo);

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
	//pe astea 3 nu le mai implementez
	//void setFreeChainFrom(const std::string_view cardName);
	//void setProvidesChainTo(const std::string_view cardName);
	// void setEffect(const CardEffect & effect);

	bool canBeBuiltFreelyAfter(const Card& previousCard) const;
	bool canBuildFreeChainTo(const Card& nextCard) const;

	void applyEffect(); // To be defined later
	std::string getEffectDescription() const; // To be defined later

	void displayCardInfo() const;


	~Card() = default;

};

