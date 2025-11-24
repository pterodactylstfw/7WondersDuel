#pragma once
#include "Constants.h"
#include <string>
#include <vector>
#include "CardEffect.h"

using json = nlohmann::json;

class ProgressToken {
private:
    ProgressTokenType m_tokenType;
    std::string m_name;
    std::string m_description;
    CardEffect m_effect;     
    bool m_isActive;        

public:
    ProgressToken() = default;
    ProgressToken(ProgressTokenType type, const std::string& name,
        const std::string& description, CardEffect effect);

    ProgressToken(const ProgressToken&) = default;
    ProgressToken& operator=(const ProgressToken&) = default;
    ProgressToken(ProgressToken&&) noexcept = default;
    ProgressToken& operator=(ProgressToken&&) noexcept = default;
    ~ProgressToken() = default;

    ProgressTokenType getType() const;
    std::string_view getName() const;
    std::string_view getDescription() const;

    uint8_t getVictoryPoints() const;
    bool isActive() const;
    const CardEffect& getEffect() const; 
    void setActive(bool active);

    std::string toString() const;
    auto operator<=>(const ProgressToken& other) const = default;
    bool operator==(const ProgressToken& other) const = default;

    friend std::ostream& operator<<(std::ostream& os, const ProgressToken& token);
    friend void to_json(json& j, const ProgressToken& token);
    friend void from_json(const json& j, ProgressToken& token);
};