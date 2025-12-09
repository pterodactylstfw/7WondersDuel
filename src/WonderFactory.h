#pragma once

#include <vector>
#include <memory>
#include "Wonder.h"
#include "Cost.h"
#include "CardEffect.h"
#include "Constants.h"

class WonderFactory
{
public:
	std::vector<std::unique_ptr<Wonder>> createWonders();
};

