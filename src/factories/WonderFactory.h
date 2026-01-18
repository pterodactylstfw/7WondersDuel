#pragma once

#include <vector>
#include <memory>
#include "Wonder.h"
#include "CoreExport.h"

class CORE_API WonderFactory
{
public:
	std::vector<std::unique_ptr<Wonder>> createWonders();
};

