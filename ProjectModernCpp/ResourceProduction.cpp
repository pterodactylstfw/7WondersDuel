#include "ResourceProduction.h"
#include <sstream>

ResourceProduction::ResourceProduction(ResourceProduction&& other) noexcept :
	m_fixedResources(std::move(other.m_fixedResources)),
	m_choices(std::move(other.m_choices))
{
}

ResourceProduction& ResourceProduction::operator=(ResourceProduction&& other) noexcept
{
	if (this != &other)
	{
		m_fixedResources = std::move(other.m_fixedResources);
		m_choices = std::move(other.m_choices);
	}
	return *this;
}


ResourceProduction& ResourceProduction::addFixedResource(ResourceType type, int quantity)
{
	this->m_fixedResources[type] += quantity;
	return *this; 
}

ResourceProduction& ResourceProduction::addChoice(const std::vector<ResourceType>& options)
{
	this->m_choices.push_back(options);
	return *this; 
}

const std::map<ResourceType, int>& ResourceProduction::getFixedResources() const
{
	return m_fixedResources;
}

const std::vector<std::vector<ResourceType>>& ResourceProduction::getChoices() const
{
	return m_choices;
}

bool ResourceProduction::hasChoices() const
{
	return !this->m_choices.empty();
}

std::map<ResourceType, int> ResourceProduction::getTotalProduction() const
{
	std::map<ResourceType, int> total = m_fixedResources;

	for (const auto& group : m_choices)
		for (const auto& type : group)
			total[type] += 1;

	return total;
}

bool ResourceProduction::isEmpty() const
{
	return m_fixedResources.empty() && m_choices.empty();
}

std::string ResourceProduction::getDescription() const
{
	if (isEmpty()) {
		return "";
	}

	std::stringstream ss;
	bool firstElement = true;

	for (const auto& pair : m_fixedResources) {
		if (!firstElement) {
			ss << ", ";
		}
		ss << pair.second << " " << resourceToString(pair.first); 
		firstElement = false;
	}

	for (const auto& choiceGroup : m_choices) {
		if (!firstElement) {
			ss << ", ";
		}

		for (int i = 0; i < choiceGroup.size(); i++) {
			ss << resourceToString(choiceGroup[i]);
			if (i < choiceGroup.size() - 1) {
				ss << "/"; 
			}
		}
		firstElement = false;
	}

	return ss.str();
}