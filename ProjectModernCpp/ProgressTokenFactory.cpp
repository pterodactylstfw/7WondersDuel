#include "ProgressTokenFactory.h"

std::unique_ptr<ProgressToken> ProgressTokenFactory::createToken(ProgressTokenType type) {
    switch (type) {
    case ProgressTokenType::AGRICULTURE:
        return std::make_unique<ProgressToken>(type, "Agriculture",
            "Take 6 coins, worth 4 VP.",
            CardEffect().withBaseCoins(6).withVictoryPoints(4));

    case ProgressTokenType::ARCHITECTURE:
        return std::make_unique<ProgressToken>(type, "Architecture",
            "Future Wonders cost 2 fewer resources.",
            CardEffect());

    case ProgressTokenType::ECONOMY:
        return std::make_unique<ProgressToken>(type, "Economy",
            "Gain money spent by opponent on trading.",
            CardEffect());

    case ProgressTokenType::LAW:
        return std::make_unique<ProgressToken>(type, "Law",
            "Worth a scientific symbol.",
            CardEffect().withScienceSymbol(ScientificSymbol::SCALES));

    case ProgressTokenType::MASONRY:
        return std::make_unique<ProgressToken>(type, "Masonry",
            "Blue cards cost 2 fewer resources.",
            CardEffect());

    case ProgressTokenType::MATHEMATICS:
        return std::make_unique<ProgressToken>(type, "Mathematics",
            "3 VP for each Progress token.",
            CardEffect());

    case ProgressTokenType::PHILOSOPHY:
        return std::make_unique<ProgressToken>(type, "Philosophy",
            "7 Victory Points.",
            CardEffect().withVictoryPoints(7));

    case ProgressTokenType::STRATEGY:
        return std::make_unique<ProgressToken>(type, "Strategy",
            "New military buildings get +1 Shield.",
            CardEffect());

    case ProgressTokenType::THEOLOGY:
        return std::make_unique<ProgressToken>(type, "Theology",
            "Wonders have Play Again effect.",
            CardEffect());

    case ProgressTokenType::URBANISM:
        return std::make_unique<ProgressToken>(type, "Urbanism",
            "Take 6 coins. Linking gives 4 coins.",
            CardEffect().withBaseCoins(6));

    default:
        return std::make_unique<ProgressToken>(ProgressTokenType::NONE, "Unknown", "Unknown", CardEffect());
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