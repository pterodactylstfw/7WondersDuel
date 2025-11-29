#pragma once

#include <optional>
#include <memory>
#include "../include/nlohmann/json.hpp"

using json = nlohmann::json;

template<typename T> // helper std::optional <-> json
std::optional<T> get_optional(const json& j, const std::string& key) {
	if (j.contains(key) && !j[key].is_null()) {
		return std::optional<T>(j[key].get<T>());
	}
	return std::nullopt;
}

NLOHMANN_JSON_NAMESPACE_BEGIN // serializer std::unique_ptr

template <typename T> struct adl_serializer<std::unique_ptr<T>> {
    template <typename BasicJsonType> static void to_json(BasicJsonType& json_value, const std::unique_ptr<T>& ptr)
    {
        if (ptr.get()) {
            json_value = *ptr;
        }
        else {
            json_value = nullptr;
        }
    }

    template <typename BasicJsonType> static void from_json(const BasicJsonType& json_value, std::unique_ptr<T>& ptr)
    {
        if (json_value.is_null()) {
            ptr = nullptr;
        }
        else {
            T inner_val = json_value.template get<T>();
            ptr = std::make_unique<T>(std::move(inner_val));
        }
    }
};

NLOHMANN_JSON_NAMESPACE_END

