#pragma once
#include <vector>
#include <memory>
#include "ProgressToken.h"
#include "CoreExport.h"

class CORE_API ProgressTokenFactory
{
private:
	std::unique_ptr<ProgressToken> createToken(ProgressTokenType type);

public:
	std::vector<std::unique_ptr<ProgressToken>> createAllTokens();
};