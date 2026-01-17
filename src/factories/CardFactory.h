#pragma once

#include <vector>
#include <memory>

#include "Card.h"
#include "CoreExport.h"

class CORE_API CardFactory
{
public:
	std::vector<std::unique_ptr<Card>> createAgeIDeck();
	std::vector<std::unique_ptr<Card>> createAgeIIDeck();
	std::vector<std::unique_ptr<Card>> createAgeIIIDeck();

	std::vector<std::unique_ptr<Card>> createGuildDeck();
};

