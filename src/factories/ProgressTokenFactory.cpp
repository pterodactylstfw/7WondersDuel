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
            .withCustomDescription("Immediately take 6 coins from the Bank. The token is worth 4 victory points."),
			"agriculture.png"
        );

    case ProgressTokenType::ARCHITECTURE:
        return std::make_unique<ProgressToken>(
            type,
            "Architecture",
            CardEffect()
            .withArchitectureEffect()
            .withCustomDescription("Any future Wonders built by you will cost 2 fewer resources."),
			"architecture.png"
        );

    case ProgressTokenType::ECONOMY:
        return std::make_unique<ProgressToken>(
            type,
            "Economy",
            CardEffect()
            .withEconomyEffect()
            .withCustomDescription("You gain the money spent by your opponent when they trade for resources."),
			"economy.png"
        );

    case ProgressTokenType::LAW:
        return std::make_unique<ProgressToken>(
            type,
            "Law",
            CardEffect()
            .withScienceSymbol(ScientificSymbol::SCALES)
            .withCustomDescription("This token is worth a scientific symbol."),
			"law.png"
        );
    case ProgressTokenType::MASONRY:
        return std::make_unique<ProgressToken>(
            type,
            "Masonry",
            CardEffect()
            .withMasonryEffect()
            .withCustomDescription("Any future blue cards constructed by you will cost 2 fewer resources."),
			"masonry.png"
        );
    case ProgressTokenType::MATHEMATICS:
        return std::make_unique<ProgressToken>(
            type,
            "Mathematics",
            CardEffect()
            .withMathematicsEffect()
            .withCustomDescription("At the end of the game, score 3 victory points for each Progress token in your possession."),
			"mathematics.png"
        );

    case ProgressTokenType::PHILOSOPHY:
        return std::make_unique<ProgressToken>(
            type,
            "Philosophy",
            CardEffect()
            .withVictoryPoints(7)
            .withCustomDescription("The token is worth 7 victory points."),
			"philosophy.png"
        );

    case ProgressTokenType::STRATEGY:
        return std::make_unique<ProgressToken>(
            type,
            "Strategy",
            CardEffect()
            .withStrategyEffect()
            .withCustomDescription("Your new military Buildings (red cards) will benefit from 1 extra Shield."),
			"strategy.png"
        );

    case ProgressTokenType::THEOLOGY:
        return std::make_unique<ProgressToken>(
            type,
            "Theology",
            CardEffect()
            .grantsPlayAgain()
            .withCustomDescription("All future Wonders constructed by you are treated as though they have the 'Play Again' effect."),
			"theology.png"
        );

    case ProgressTokenType::URBANISM:
        return std::make_unique<ProgressToken>(
            type,
            "Urbanism",
            CardEffect()
            .withBaseCoins(6)
            .withCustomDescription("Immediately take 6 coins. Linking gives 4 coins."),
			"urbanism.png"
        );
    default:
        return std::make_unique<ProgressToken>(
            ProgressTokenType::NONE,
            "Unknown",
            CardEffect().withCustomDescription("Unknown Token"),
			"unknown.png"
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