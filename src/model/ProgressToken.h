#pragma once
#include "Constants.h"
#include <string>
#include "CardEffect.h"
#include "JsonUtils.h"
#include "CoreExport.h"

class CORE_API ProgressToken {
private:
    ProgressTokenType m_tokenType;
    std::string m_name;
    CardEffect m_effect;
    bool m_isActive;
	std::string m_imagePath;

public:
    ProgressToken() = default;
    ProgressToken(ProgressTokenType type, const std::string& name, CardEffect effect, std::string imagePath);

    ProgressToken(const ProgressToken&) = default;
    ProgressToken& operator=(const ProgressToken&) = default;
    ProgressToken(ProgressToken&&) noexcept = default;
    ProgressToken& operator=(ProgressToken&&) noexcept = default;
    ~ProgressToken() = default;

    ProgressTokenType getType() const;
    std::string getName() const;
    std::string getDescription() const;
	std::string getImagePath() const;

    uint8_t getVictoryPoints() const;
    bool isActive() const;
    CardEffect& getEffect(); 
    void setActive(bool active);

    std::string toString() const;
    //auto operator<=>(const ProgressToken& other) const = default;
    bool operator==(const ProgressToken& other) const = default;

    friend std::ostream& operator<<(std::ostream& os, const ProgressToken& token);
    friend void to_json(json& j, const ProgressToken& token);
    friend void from_json(const json& j, ProgressToken& token);
};