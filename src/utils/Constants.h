#pragma once
#include <string>
#include "JsonUtils.h"
#include "CoreExport.h"

namespace GameConstants {

    constexpr uint8_t NUMBER_OF_PLAYERS = 2;
    constexpr uint8_t STARTING_COINS = 7;
    constexpr uint8_t CARDS_PER_AGE = 20;
    constexpr uint8_t MAX_WONDERS_TOTAL = 7;
    constexpr uint8_t WONDERS_PER_PLAYER = 4;

    constexpr uint8_t MILITARY_SUPREMACY_DISTANCE = 9;
    constexpr uint8_t SCIENTIFIC_SUPREMACY_SYMBOLS = 6;

    constexpr uint8_t BASE_TRADE_COST = 2;
    constexpr uint8_t DISCARD_COINS_BASE = 2;
    constexpr uint8_t COINS_PER_VP = 3;

    constexpr uint8_t MILITARY_ZONE_1 = 3;
    constexpr uint8_t MILITARY_ZONE_2 = 6;
}

namespace Colors {
    const std::string RESET = "\033[0m";
    const std::string RED = "\033[31m";     // Military
    const std::string GREEN = "\033[32m";   // Science
    const std::string YELLOW = "\033[33m";  // Commercial
    const std::string BLUE = "\033[34m";    // Civilian
    const std::string MAGENTA = "\033[35m"; // Guilds
    const std::string CYAN = "\033[36m";    // Wonders
    const std::string GREY = "\033[90m";    // Raw Materials
    const std::string BROWN = "\033[38;5;94m"; // Manufactured
}

enum class CardColor {
    BROWN,   
    GREY,     
    BLUE,     
    GREEN,    
    YELLOW,   
    RED,      
    PURPLE,   
    NONE
};

NLOHMANN_JSON_SERIALIZE_ENUM(CardColor,
    {
        {CardColor::BROWN, "Brown"},
        {CardColor::GREY, "Grey"},
        {CardColor::BLUE, "Blue"},
        {CardColor::GREEN, "Green"},
        {CardColor::YELLOW, "Yellow"},
        {CardColor::RED, "Red"},
        {CardColor::PURPLE, "Purple"},
        {CardColor::NONE, "None"}
    }
);


enum class ResourceType {
    WOOD,       
    CLAY,       
    STONE,      
    GLASS,      
    PAPYRUS,    
    NONE
};

NLOHMANN_JSON_SERIALIZE_ENUM(ResourceType,
    {
        {ResourceType::WOOD, "Wood"},
        {ResourceType::CLAY, "Clay"},
        {ResourceType::STONE, "Stone"},
        {ResourceType::GLASS, "Glass"},
        {ResourceType::PAPYRUS, "Papyrus"},
        {ResourceType::NONE, "None"}
    }
);


enum class ScientificSymbol {
    ASTROLABE,      
    SCALES,         
    SUNDIAL,        
    MORTAR_PESTLE,  
    DRAFTING_COMPASS, 
    QUILL_INKWELL,  
    WHEEL,          
    NONE
};

NLOHMANN_JSON_SERIALIZE_ENUM(ScientificSymbol,
    {
        {ScientificSymbol::ASTROLABE, "Astrolabe"},
        {ScientificSymbol::SCALES, "Scales"},
        {ScientificSymbol::SUNDIAL, "Sundial"},
        {ScientificSymbol::MORTAR_PESTLE, "Mortar and Pestle"},
        {ScientificSymbol::DRAFTING_COMPASS, "Drafting Compass"},
        {ScientificSymbol::QUILL_INKWELL, "Quill and Inkwell"},
        {ScientificSymbol::WHEEL, "Wheel"},
        {ScientificSymbol::NONE, "None"}
    }
);


enum class VictoryType {
    NONE,
    MILITARY_SUPREMACY,      
    SCIENTIFIC_SUPREMACY,    
    CIVILIAN_VICTORY         
};

NLOHMANN_JSON_SERIALIZE_ENUM(VictoryType,
    {
        {VictoryType::NONE, "None"},
        {VictoryType::MILITARY_SUPREMACY, "Military Supremacy"},
        {VictoryType::SCIENTIFIC_SUPREMACY, "Scientific Supremacy"},
        {VictoryType::CIVILIAN_VICTORY, "Civilian Victory"}
    }
);

enum class ProgressTokenType {
    AGRICULTURE,    
    ARCHITECTURE,    
    ECONOMY,         
    LAW,            
    MASONRY,        
    MATHEMATICS,    
    PHILOSOPHY,     
    STRATEGY,       
    THEOLOGY,       
    URBANISM,       
	NONE
};

NLOHMANN_JSON_SERIALIZE_ENUM(ProgressTokenType,
    {
        {ProgressTokenType::AGRICULTURE, "Agriculture"},
        {ProgressTokenType::ARCHITECTURE, "Architecture"},
        {ProgressTokenType::ECONOMY, "Economy"},
        {ProgressTokenType::LAW, "Law"},
        {ProgressTokenType::MASONRY, "Masonry"},
        {ProgressTokenType::MATHEMATICS, "Mathematics"},
        {ProgressTokenType::PHILOSOPHY, "Philosophy"},
        {ProgressTokenType::STRATEGY, "Strategy"},
        {ProgressTokenType::THEOLOGY, "Theology"},
        {ProgressTokenType::URBANISM, "Urbanism"},
        {ProgressTokenType::NONE, "None"}
    }
);

enum class WonderType {
    THE_APPIAN_WAY,
    CIRCUS_MAXIMUS,
    THE_COLOSSUS,
    THE_GREAT_LIBRARY,
    THE_GREAT_LIGHTHOUSE,
    THE_HANGING_GARDENS,
    THE_MAUSOLEUM,
    PIRAEUS,
    THE_PYRAMIDS,
    THE_SPHINX,
    THE_STATUE_OF_ZEUS,
    THE_TEMPLE_OF_ARTEMIS,
    NONE
};

NLOHMANN_JSON_SERIALIZE_ENUM(WonderType,
    {
        {WonderType::THE_APPIAN_WAY, "The Appian Way"},
        {WonderType::CIRCUS_MAXIMUS, "Circus Maximus"},
        {WonderType::THE_COLOSSUS, "The Colossus"},
        {WonderType::THE_GREAT_LIBRARY, "The Great Library"},
        {WonderType::THE_GREAT_LIGHTHOUSE, "The Great Lighthouse"},
        {WonderType::THE_HANGING_GARDENS, "The Hanging Gardens"},
        {WonderType::THE_MAUSOLEUM, "The Mausoleum"},
        {WonderType::PIRAEUS, "Piraeus"},
        {WonderType::THE_PYRAMIDS, "The Pyramids"},
        {WonderType::THE_SPHINX, "The Sphinx"},
        {WonderType::THE_STATUE_OF_ZEUS, "The Statue of Zeus"},
        {WonderType::THE_TEMPLE_OF_ARTEMIS, "The Temple of Artemis"},
        { WonderType::NONE, "None" }
    }
);

enum class PlayerAction {
    CONSTRUCT_BUILDING,   
    DISCARD_FOR_COINS,    
    CONSTRUCT_WONDER,
	CHOOSE_ANOTHER_CARD,
	ACTION_SUCCESSFUL
};

NLOHMANN_JSON_SERIALIZE_ENUM(PlayerAction,
    {
        {PlayerAction::CONSTRUCT_BUILDING, "Construct Building"},
        {PlayerAction::DISCARD_FOR_COINS, "Discard for Coins"},
        {PlayerAction::CONSTRUCT_WONDER, "Construct Wonder"},
        {PlayerAction::CHOOSE_ANOTHER_CARD, "Choose Another Card"},
        {PlayerAction::ACTION_SUCCESSFUL, "Action Successful"}
    }
);

enum class GamePhase {
    DRAFTING,
    AGE_I,
    AGE_II,
	AGE_III,
    GAME_OVER
};

NLOHMANN_JSON_SERIALIZE_ENUM(GamePhase,
    {
        {GamePhase::DRAFTING, "Drafting"},
        {GamePhase::AGE_I, "Age I"},
        {GamePhase::AGE_II, "Age II"},
        {GamePhase::AGE_III, "Age III"},
        {GamePhase::GAME_OVER, "Game Over"}
    }
);

CORE_API std::string colorToString(CardColor color);

CORE_API std::string resourceToString(ResourceType type);

CORE_API std::string scientificSymbolToString(ScientificSymbol symbol);

CORE_API std::string progressTokenTypeToString(ProgressTokenType tokenType);

CORE_API std::string victoryTypeToString(VictoryType type);

CORE_API std::string actionToString(PlayerAction action);

CORE_API std::string wonderTypeToString(WonderType type);
