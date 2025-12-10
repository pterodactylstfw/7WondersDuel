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
	case ScientificSymbol::ASTROLABE:        return "Astrolabe";
	case ScientificSymbol::SCALES:           return "Scales";
	case ScientificSymbol::SUNDIAL:          return "Sundial";
	case ScientificSymbol::MORTAR_PESTLE:    return "Mortar and Pestle";
	case ScientificSymbol::DRAFTING_COMPASS: return "Drafting Compass";
	case ScientificSymbol::QUILL_INKWELL:    return "Quill and Inkwell";
	case ScientificSymbol::WHEEL:            return "Wheel";
    default:                                 return "Unknown";
    }
}


std::string progressTokenTypeToString(ProgressTokenType tokenType)
{
    switch (tokenType) {
    case ProgressTokenType::AGRICULTURE:   return "Agriculture";
    case ProgressTokenType::ARCHITECTURE:  return "Architecture";
    case ProgressTokenType::ECONOMY:       return "Economy";
    case ProgressTokenType::LAW:           return "Law";
    case ProgressTokenType::MASONRY:       return "Masonry";
    case ProgressTokenType::MATHEMATICS:   return "Mathematics";
    case ProgressTokenType::PHILOSOPHY:    return "Philosophy";
    case ProgressTokenType::STRATEGY:      return "Strategy";
    case ProgressTokenType::THEOLOGY:      return "Theology";
    case ProgressTokenType::URBANISM:      return "Urbanism";
    case ProgressTokenType::NONE:          return "None";
    default:                               return "Unknown";
    }
}

std::string wonderTypeToString(WonderType type)
{
    switch (type) {
    case WonderType::THE_APPIAN_WAY:        return "The Appian Way";
    case WonderType::CIRCUS_MAXIMUS:        return "Circus Maximus";
    case WonderType::THE_COLOSSUS:          return "The Colossus";
    case WonderType::THE_GREAT_LIBRARY:     return "The Great Library";
    case WonderType::THE_GREAT_LIGHTHOUSE:  return "The Great Lighthouse";
    case WonderType::THE_HANGING_GARDENS:   return "The Hanging Gardens";
    case WonderType::THE_MAUSOLEUM:         return "The Mausoleum";
    case WonderType::PIRAEUS:               return "Piraeus";
    case WonderType::THE_PYRAMIDS:          return "The Pyramids";
    case WonderType::THE_SPHINX:            return "The Sphinx";
    case WonderType::THE_STATUE_OF_ZEUS:    return "The Statue of Zeus";
    case WonderType::THE_TEMPLE_OF_ARTEMIS: return "The Temple of Artemis";
    case WonderType::NONE:                  return "None";
    default:                                return "Unknown";
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