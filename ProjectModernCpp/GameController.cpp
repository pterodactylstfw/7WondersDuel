#include "GameController.h"


void GameController::applyWonderEffect(Player& player, Player& opponent, const Wonder& wonder, GameState& gameState)
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
			auto removedCard = opponent.removeCard(*chosen);

			gameState.addToDiscardCards(std::move(removedCard));
		
			break;
		}
		case WonderType::THE_COLOSSUS:
		{
			player.addMilitaryShields(2);
			player.addVictoryPoints(3);
			break;
		}
		case WonderType::THE_GREAT_LIBRARY:
		{
			player.addVictoryPoints(4);
			//+Randomly draw 3 Progress tokens from among those discarded at
			//the beginning of the game.Choose one, play it, and return the other
			//2 to the box.
			break;
		}
		case WonderType::THE_GREAT_LIGHTHOUSE:
		{
			player.addVictoryPoints(4);
			player.addResourceChoice({
				ResourceType::WOOD,
				ResourceType::STONE,
				ResourceType::CLAY
				});
			break;
		}
	}
}
