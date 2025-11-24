#include "ProgressTokenFactory.h"

std::unique_ptr<ProgressToken> ProgressTokenFactory::createToken(ProgressTokenType type) {
    switch (type) {
    case ProgressTokenType::AGRICULTURE:
        return std::make_unique<ProgressToken>(
            type,
            "Agriculture",
            CardEffect()
            .withBaseCoins(6)
            .withVictoryPoints(4)
            .withCustomDescription("Immediately take 6 coins from the Bank. The token is worth 4 victory points.")
        );

    case ProgressTokenType::ARCHITECTURE:
        return std::make_unique<ProgressToken>(
            type,
            "Architecture",
            CardEffect()
            .withCustomDescription("Any future Wonders built by you will cost 2 fewer resources.")
        );

    case ProgressTokenType::ECONOMY:
        return std::make_unique<ProgressToken>(
            type,
            "Economy",
            CardEffect()
            .withCustomDescription("You gain the money spent by your opponent when they trade for resources.")
        );

    case ProgressTokenType::LAW:
        return std::make_unique<ProgressToken>(
            type,
            "Law",
            CardEffect()
            .withScienceSymbol(ScientificSymbol::SCALES)
            .withCustomDescription("This token is worth a scientific symbol.")
        );
    case ProgressTokenType::MASONRY:
        return std::make_unique<ProgressToken>(
            type,
            "Masonry",
            CardEffect()
            .withCustomDescription("Any future blue cards constructed by you will cost 2 fewer resources.")
        );
    case ProgressTokenType::MATHEMATICS:
        return std::make_unique<ProgressToken>(
            type,
            "Mathematics",
            CardEffect()
            .withCustomDescription("At the end of the game, score 3 victory points for each Progress token in your possession.")
        );

    case ProgressTokenType::PHILOSOPHY:
        return std::make_unique<ProgressToken>(
            type,
            "Philosophy",
            CardEffect()
            .withVictoryPoints(7)
            .withCustomDescription("The token is worth 7 victory points.")
        );

    case ProgressTokenType::STRATEGY:
        return std::make_unique<ProgressToken>(
            type,
            "Strategy",
            CardEffect()
            .withCustomDescription("Your new military Buildings (red cards) will benefit from 1 extra Shield.")
        );

    case ProgressTokenType::THEOLOGY:
        return std::make_unique<ProgressToken>(
            type,
            "Theology",
            CardEffect()
            .withCustomDescription("All future Wonders constructed by you are treated as though they have the 'Play Again' effect.")
        );

    case ProgressTokenType::URBANISM:
        return std::make_unique<ProgressToken>(
            type,
            "Urbanism",
            CardEffect()
            .withBaseCoins(6)
            .withCustomDescription("Immediately take 6 coins. Linking gives 4 coins.")
        );

    default:
        return std::make_unique<ProgressToken>(
            ProgressTokenType::NONE,
            "Unknown",
            CardEffect().withCustomDescription("Unknown Token")
        );
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