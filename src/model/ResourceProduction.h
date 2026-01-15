#pragma once

#include <vector>
#include <map>
#include "Constants.h"
#include "JsonUtils.h"
#include "CoreExport.h"

class CORE_API ResourceProduction
{
private:
	std::map<ResourceType, int> m_fixedResources;
	std::vector<std::vector<ResourceType>> m_choices;

public:
	ResourceProduction() = default;

	ResourceProduction(const ResourceProduction& other) = default;
	ResourceProduction& operator=(const ResourceProduction& other) = default;

	ResourceProduction(ResourceProduction&& other) noexcept;
	ResourceProduction& operator=(ResourceProduction&& other) noexcept;

	ResourceProduction& addFixedResource(ResourceType type, int quantity);
	ResourceProduction& addChoice(const std::vector<ResourceType>& options);

	const std::map<ResourceType, int>& getFixedResources() const;
	const std::vector<std::vector<ResourceType>>& getChoices() const;

	bool operator==(const ResourceProduction& other) const = default;

	bool hasChoices() const;
	std::map<ResourceType, int> getTotalProduction() const;
	bool isEmpty() const;
	std::string getDescription() const;

	void removeFixedResource(const ResourceProduction& resource);

	friend void to_json(json& j, const ResourceProduction& prod);
	friend void from_json(const json& j, ResourceProduction& prod);
};

