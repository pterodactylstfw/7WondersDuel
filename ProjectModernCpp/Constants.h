#pragma once
#include<string>

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

enum class ResourceType {
    WOOD,       
    CLAY,       
    STONE,      
    GLASS,      
    PAPYRUS,    
    NONE
};

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

enum class VictoryType {
    NONE,
    MILITARY_SUPREMACY,      
    SCIENTIFIC_SUPREMACY,    
    CIVILIAN_VICTORY         
};

enum class ProgressToken {
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


enum class PlayerAction {
    CONSTRUCT_BUILDING,   
    DISCARD_FOR_COINS,    
    CONSTRUCT_WONDER      
};

std::string colorToString(CardColor color);

std::string resourceToString(ResourceType type); 

std::string scientificSymbolToString(ScientificSymbol symbol);

std::string progressTokenToString(ProgressToken token);

std::string victoryTypeToString(VictoryType type);

std::string actionToString(PlayerAction action);
