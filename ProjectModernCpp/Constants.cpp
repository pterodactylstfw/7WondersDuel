#include "Constants.h"

std::string colorToString(CardColor color){
        switch (color) {
        case CardColor::BROWN:   return "Brown";
        case CardColor::GREY:    return "Grey";
        case CardColor::BLUE:    return "Blue";
        case CardColor::GREEN:   return "Green";
        case CardColor::YELLOW:  return "Yellow";
        case CardColor::RED:     return "Red";
        case CardColor::PURPLE:  return "Purple";
        case CardColor::NONE:    return "None";
        default:                 return "Unknown";
        }
}
std::string resourceToString(ResourceType type) {
        switch (type) {
        case ResourceType::WOOD:     return "Wood";
        case ResourceType::CLAY:     return "Clay";
        case ResourceType::STONE:    return "Stone";
        case ResourceType::GLASS:    return "Glass";
        case ResourceType::PAPYRUS:  return "Papyrus";
        case ResourceType::NONE:     return "None";
        default:                     return "Unknown";
        }
}

std::string scientificSymbolToString(ScientificSymbol symbol){
    switch (symbol) {
    case ScientificSymbol::COMPASS:        return "Compass";
    case ScientificSymbol::GEAR:           return "Gear";
    case ScientificSymbol::TABLET:         return "Tablet";
    case ScientificSymbol::SUNDIAL:        return "Sundial";
    case ScientificSymbol::WHEEL:          return "Wheel";
    case ScientificSymbol::MORTAR_PESTLE:  return "Mortar & Pestle";
    case ScientificSymbol::ASTROLABE:      return "Astrolabe";
    case ScientificSymbol::NONE:           return "None";
    default:                               return "Unknown";
    }
}

std::string progressTokenToString(ProgressToken token)
{
    switch (token) {
    case ProgressToken::AGRICULTURE:   return "Agriculture";
    case ProgressToken::ARCHITECTURE:  return "Architecture";
    case ProgressToken::ECONOMY:       return "Economy";
    case ProgressToken::LAW:           return "Law";
    case ProgressToken::MASONRY:       return "Masonry";
    case ProgressToken::MATHEMATICS:   return "Mathematics";
    case ProgressToken::PHILOSOPHY:    return "Philosophy";
    case ProgressToken::STRATEGY:      return "Strategy";
    case ProgressToken::THEOLOGY:      return "Theology";
    case ProgressToken::URBANISM:      return "Urbanism";
    case ProgressToken::NONE:          return "None";
    default:                           return "Unknown";
    }
}

std::string victoryTypeToString(VictoryType type)
{
    switch (type) {
    case VictoryType::NONE:                  return "None";
    case VictoryType::MILITARY_SUPREMACY:    return "Military Supremacy";
    case VictoryType::SCIENTIFIC_SUPREMACY:  return "Scientific Supremacy";
    case VictoryType::CIVILIAN_VICTORY:      return "Civilian Victory";
    default:                                 return "Unknown";
    }
}

std::string actionToString(PlayerAction action)
{
    switch (action) {
    case PlayerAction::CONSTRUCT_BUILDING:  return "Construct Building";
    case PlayerAction::DISCARD_FOR_COINS:   return "Discard for Coins";
    case PlayerAction::CONSTRUCT_WONDER:    return "Construct Wonder";
    default:                                return "Unknown";
    }
}