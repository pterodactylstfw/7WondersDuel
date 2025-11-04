#pragma once

#include <vector>
#include <map>
#include "Constants.h"

class ResourceProduction
{
private:
	std::map<ResourceType, int> m_fixedResources;
	std::vector<std::vector<ResourceType>> m_choices;

public:
	ResourceProduction() = default;

	void addFixedResource(ResourceType type, int quantity);
	void addChoice(std::vector<ResourceType>& options);

	const std::map<ResourceType, int>& getFixedResources() const;
	const std::vector<std::vector<ResourceType>>& getChoices() const;

	bool hasChoices() const;

	std::map<ResourceType, int> getTotalProduction() const;

	bool isEmpty() const;
};

