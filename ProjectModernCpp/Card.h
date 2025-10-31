#pragma once

#include <string>
#include <iostream>
#include "Constants.h"
#include <optional>

class Card
{
private:
	std::string m_name;
	CardColor m_color;
	uint8_t m_age;
	// Cost cost; // To be defined later
	// CardEffect effect; // To be defined later 

	std::string m_freeChainFrom; // Card care ofera lant liber catre aceasta carte (daca exista)
	std::string m_providesChainTo; // Cardul pentru care aceasta carte ofera lant liber (daca exista)

public:

	Card(const std::string_view name, const CardColor& color, const uint8_t age) : m_name(name), m_color(color), m_age(age) {}
	Card(const Card& other) = default;
	Card& operator = (const Card& other) = default;

	std::string getName() const;
	CardColor getColor() const;

	uint8_t getAge() const;

	std::string getFreeChainFrom() const;
	std::string getProvidesChainTo() const;

	void setFreeChainFrom(const std::string_view cardName);
	void setProvidesChainTo(const std::string_view cardName);
	// void setEffect(const CardEffect & effect);

	bool canBeBuiltFreelyAfter(const Card& previousCard) const;
	bool canBuildFreeChainTo(const Card& nextCard) const;

	void applyEffect(); // To be defined later
	std::string getEffectDescription() const; // To be defined later

	void displayCardInfo() const;

	std::string resourceTypesToString() const; // To be defined later
	std::string scienceSymbolsToString() const; // To be defined later

	~Card() = default;

};

