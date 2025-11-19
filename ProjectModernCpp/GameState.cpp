#include "GameState.h"
#include <fstream>
NLOHMANN_JSON_NAMESPACE_BEGIN

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

GameState::GameState():m_currentPlayerIndex(0),
m_currentTurn(1),
m_winnerIndex(std::nullopt),
m_currentAge(1),
m_gameOver(false),
m_rng(std::random_device{}())
{
    m_players[0] = std::make_unique<Player>("Player 1");
    m_players[1] = std::make_unique<Player>("Player 2");
}

void GameState::switchPlayer()
{ 
	m_currentPlayerIndex = 1 - m_currentPlayerIndex;
}

const Player& GameState::getCurrentPlayer() const
{
	return *m_players[m_currentPlayerIndex];
}

const Player& GameState::getOpponent() const
{
	return *m_players[1 - m_currentPlayerIndex];
}

uint8_t GameState::getCurrentAge() const
{
	return m_currentAge;
}

bool GameState::isGameOver() const
{
	return m_gameOver;
}

const std::optional<uint8_t>& GameState::getWinnerIndex() const
{
	return m_winnerIndex;
}

Player& GameState::getCurrentPlayer()
{
	return *m_players[m_currentPlayerIndex];
}

Player& GameState::getOpponent()
{
	return *m_players[1 - m_currentPlayerIndex];
}

bool GameState::saveGame(std::string&& filename) const
{
    try
    {
        json j = *this;

        std::ofstream file(filename);
        if (!file.is_open()) {

            return false;
        }

        file << j.dump(4);
        file.close();
        return true;
    }
    catch (const nlohmann::json::exception& e)
    {
        return false;
    }
}

bool GameState::loadGame(std::string&& filename)
{
    try
    {
        std::ifstream file(filename);
        if (!file.is_open()) {
            std::cerr << "Eroare: Nu s-a putut deschide fisierul" << filename<< " pentru incarcare : " << std::endl;
            return false;
        }

        nlohmann::json j;
        file >> j;
        file.close();

        from_json(j, *this);

        m_rng = std::mt19937(std::random_device{}());

        return true;
    }
    catch (const nlohmann::json::exception& e)
    {
        std::cerr << "Eroare la deserializare JSON : " << e.what() << std::endl;
        return false;
    }
}

GameState::~GameState() = default;

template<typename T>
std::optional<T> get_optional(const json& j, const std::string& key) {
	if (j.contains(key) && !j[key].is_null()) {
		return std::optional<T>(j[key].get<T>());
	}
	return std::nullopt;
}

void to_json(json& j, const GameState& state)
{
    j = nlohmann::json{
        {"players", state.m_players},
        {"currentPlayerIndex", state.m_currentPlayerIndex},
        {"currentTurn", state.m_currentTurn},
        {"winnerIndex", state.m_winnerIndex},
        {"ageIDeck", state.m_ageIDeck},
        {"ageIIDeck", state.m_ageIIDeck},
        {"ageIIIDeck", state.m_ageIIIDeck},
        {"currentAge", state.m_currentAge},
        {"currentAgeCards", state.m_currentAgeCards},
        {"cardAvailability", state.m_cardAvailability},
        {"allWonders", state.m_allWonders},
        {"availableProgressToken", state.m_availableProgressToken},
        {"gameOver", state.m_gameOver}
    };
}

void from_json(const json& j, GameState& state)
{
    j.at("players").get_to(state.m_players);
    j.at("currentPlayerIndex").get_to(state.m_currentPlayerIndex);
    j.at("currentTurn").get_to(state.m_currentTurn);
    state.m_winnerIndex = get_optional<uint8_t>(j, "winnerIndex");
    j.at("ageIDeck").get_to(state.m_ageIDeck);
    j.at("ageIIDeck").get_to(state.m_ageIIDeck);
    j.at("ageIIIDeck").get_to(state.m_ageIIIDeck);
    j.at("currentAge").get_to(state.m_currentAge);
    j.at("currentAgeCards").get_to(state.m_currentAgeCards);
    j.at("cardAvailability").get_to(state.m_cardAvailability);
    j.at("allWonders").get_to(state.m_allWonders);
    j.at("availableProgressToken").get_to(state.m_availableProgressToken);
    j.at("gameOver").get_to(state.m_gameOver);
}
	
