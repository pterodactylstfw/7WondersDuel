#include "ProgressTokenFactory.h"

std::unique_ptr<ProgressToken> ProgressTokenFactory::createToken(ProgressTokenType type) {
    switch (type) {
    case ProgressTokenType::AGRICULTURE:
        return std::make_unique<ProgressToken>(type, "Agriculture",
            "Immediately take 6 coins from the Bank. The token is worth 4 victory points.", 4);

    case ProgressTokenType::ARCHITECTURE:
        return std::make_unique<ProgressToken>(type, "Architecture",
            "Any future Wonders built by you will cost 2 fewer resources.", 0);

    case ProgressTokenType::ECONOMY:
        return std::make_unique<ProgressToken>(type, "Economy",
            "You gain the money spent by your opponent when they trade for resources.", 0);

    case ProgressTokenType::LAW:
        return std::make_unique<ProgressToken>(type, "Law",
            "This token is worth a scientific symbol.", 0);

    case ProgressTokenType::MASONRY:
        return std::make_unique<ProgressToken>(type, "Masonry",
            "Any future blue cards constructed by you will cost 2 fewer resources.", 0);

    case ProgressTokenType::MATHEMATICS:
        return std::make_unique<ProgressToken>(type, "Mathematics",
            "At the end of the game, score 3 victory points for each Progress token in your possession (including itself).", 0);

    case ProgressTokenType::PHILOSOPHY:
        return std::make_unique<ProgressToken>(type, "Philosophy",
            "The token is worth 7 victory points.", 7);

    case ProgressTokenType::STRATEGY:
        return std::make_unique<ProgressToken>(type, "Strategy",
            "Once this token enters play, your new military Buildings (red cards) will benefit from 1 extra Shield.", 0);

    case ProgressTokenType::THEOLOGY:
        return std::make_unique<ProgressToken>(type, "Theology",
            "All future Wonders constructed by you are all treated as though they have the \"Play Again\" effect.", 0);

    case ProgressTokenType::URBANISM:
        return std::make_unique<ProgressToken>(type, "Urbanism",
            "Immediately take 6 coins from the Bank. Each time you construct a Building for free through linking, you gain 4 coins.", 0);

    default:
        return std::make_unique<ProgressToken>(ProgressTokenType::NONE, "Unknown", "Unknown effect", 0);
    }
}

std::vector<std::unique_ptr<ProgressToken>> ProgressTokenFactory::createAllTokens() {
    std::vector<std::unique_ptr<ProgressToken>> tokens;
    tokens.reserve(10);
    tokens.push_back(createToken(ProgressTokenType::AGRICULTURE));
    tokens.push_back(createToken(ProgressTokenType::ARCHITECTURE));
    tokens.push_back(createToken(ProgressTokenType::ECONOMY));
    tokens.push_back(createToken(ProgressTokenType::LAW));
    tokens.push_back(createToken(ProgressTokenType::MASONRY));
    tokens.push_back(createToken(ProgressTokenType::MATHEMATICS));
    tokens.push_back(createToken(ProgressTokenType::PHILOSOPHY));
    tokens.push_back(createToken(ProgressTokenType::STRATEGY));
    tokens.push_back(createToken(ProgressTokenType::THEOLOGY));
    tokens.push_back(createToken(ProgressTokenType::URBANISM));
    return tokens;
}