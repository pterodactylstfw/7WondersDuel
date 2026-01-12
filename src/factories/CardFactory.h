#pragma once

#include <vector>
#include <memory>

#include "Card.h"

class CardFactory
{
public:
	std::vector<std::unique_ptr<Card>> createAgeIDeck();
	std::vector<std::unique_ptr<Card>> createAgeIIDeck();
	std::vector<std::unique_ptr<Card>> createAgeIIIDeck();

	std::vector<std::unique_ptr<Card>> createGuildDeck();
};

