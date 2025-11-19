#include "GameController.h"


void GameController::applyWonderEffect(Player& player, Player& opponent, const Wonder& wonder)
{
	WonderType type = wonder.getWonderType();

	switch (type)
	{
		case WonderType::THE_APPIAN_WAY:
		{
			player.addCoins(3);
			opponent.removeCoins(3);
			//+ play again player
			break;
		}
		case WonderType::CIRCUS_MAXIMUS:
		{
			player.addMilitaryShields(1);
			auto opponentCards = opponent.getCardsOfType(CardColor::GREY);

			if (opponentCards.empty()) {
				std::cout << "Opponent has no grey cards to destroy.\n";
				break;
			}

			std::cout << "Your opponent's grey cards:\n";
			int index = 1;
			for (const Card* card : opponentCards)
			{
				std::cout << index<<"."<< card->getName() << "\n";
				index++;
			}

			std::cout << "Choose a card to destroy: \n";
			std::cin >> index;

			while (index < 1 || index > opponentCards.size())
			{
				std::cout << "Invalid choice. Try again: \n";
				std::cin >> index;
			}

			const Card* chosen = opponentCards[index - 1];
			opponent.removeCard(*chosen);
			//+ sa punem cartea in decartate
			break;
		}
	}
}
