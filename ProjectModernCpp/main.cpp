#include <iostream>
#include <cassert>
#include <vector>
#include <memory>

#include "GameController.h"


void printHeader(const std::string& title) {
    std::cout << "\n=========================================\n";
    std::cout << "   TEST: " << title << "\n";
    std::cout << "=========================================\n";
}

int main()
{

    // TEST 1: Factories & Deck 
    printHeader("Generare Pachete Carti");

    CardFactory cardFactory;

    auto age1Deck = cardFactory.createAgeIDeck();
    auto age2Deck = cardFactory.createAgeIIDeck();
    auto age3Deck = cardFactory.createAgeIIIDeck();
    auto guildDeck = cardFactory.createGuildDeck();

    std::cout << "Age I Deck Size: " << age1Deck.size() << '\n';
    std::cout << "Age II Deck Size: " << age2Deck.size() << '\n';
    std::cout << "Age III Deck Size: " << age3Deck.size() << '\n';
    std::cout << "Guild Deck Size: " << guildDeck.size() << '\n';

    // Afisare carte din age 1
    if (!age1Deck.empty()) {
        std::cout << "\n--- Exemplu Carte din Age I ---\n";
        age1Deck[0]->displayCardInfo();
    }

    // Afisare carte din age 2
    if (!age2Deck.empty()) {
        std::cout << "\n--- Exemplu Carte din Age II ---\n";
        age1Deck[2]->displayCardInfo();
    }

    // Afisare carte din age 3
    if (!age3Deck.empty()) {
        std::cout << "\n--- Exemplu Carte din Age III ---\n";
        age1Deck[5]->displayCardInfo();
    }

    // Afisare guild card
    if (!guildDeck.empty()) {
        std::cout << "\n--- Exemplu Guild Card ---\n";
        guildDeck[3]->displayCardInfo();
    }

  
    // TEST 2: Chaining 
    printHeader("Chaining");

    Card* scriptorium = nullptr;
    Card* library = nullptr;

    for (const auto& card : age1Deck) {
        if (card->getName() == "Scriptorium") scriptorium = card.get();
    }
    for (const auto& card : age2Deck) {
        if (card->getName() == "Library") library = card.get();
    }

    if (scriptorium && library) {
        std::cout << "Scriptorium se leaga de: "
            << (scriptorium->getProvidesChainTo().has_value() ? scriptorium->getProvidesChainTo().value() : "None") << "\n";
        std::cout << "Library este legata de: "
            << (library->getFreeChainFrom().has_value() ? library->getFreeChainFrom().value() : "None") << "\n";

        bool canChain = library->canBeBuiltFreelyAfter(*scriptorium);
        std::cout << "RESULT: Putem construi Library gratuit dupa construirea Scriptorium? " << (canChain ? "DA" : "NU") << "\n";
    }
    else {
        std::cout << "Nu s-au gasit cartile pentru testul de chaining\n";
    }


   
    // TEST 3: Economie & Trading
    printHeader("Economie & Trading");

    Player player1("Jucator 1");
    Player player2("Jucator 2");

  
    player2.addResource(ResourceType::STONE, 2);
    std::cout << "Jucatorul 2 are 2 Stone in productie fixa.\n";

    Cost costNeedsStone(0);
    costNeedsStone.withResourceCost(ResourceType::STONE, 1);

    std::cout << "Jucatorul 1 vrea sa cumpere ceva cu cost: " << costNeedsStone.toString() << "\n";

    int calculatedCost = player1.calculateResourceCost(costNeedsStone, player2);
    std::cout << "Cost calculat pentru Jucatorul 1: " << calculatedCost << " monede.\n";

    if (calculatedCost == 4) {
        std::cout << "[OK] Calculul costului de trading este corect.\n";
    }
    else {
        std::cout << "[FAIL] Calcul gresit! Asteptat: 4, Primit: " << calculatedCost << "\n";
    }

    player1.removeCoins(player1.getCoins());
    player1.addCoins(3);
    std::cout << "Jucatorul 1 are 3 monede. Isi permite (cost 4)? " << (player1.canAfford(costNeedsStone, player2) ? "Yes" : "No") << "\n";

    player1.addCoins(2);
    std::cout << "Jucatorul 1 are 5 monede. Isi permite (cost 4)? " << (player1.canAfford(costNeedsStone, player2) ? "Yes" : "No") << "\n";


    // TEST 4: Wonders
    printHeader("Wonders");

    WonderFactory wonderFactory;
    auto wonders = wonderFactory.createWonders();
    std::cout << "Total minuni create: " << wonders.size() << "\n";
    if (!wonders.empty()) {
		std::cout << wonders[4]->toString() << "\n";
    }

    
    // TEST 5: JSON Serialization (GameState)
    printHeader("JSON Save/Load System");

    GameState gameState;
    std::string filename = "test_savegame.json";

    if (gameState.saveGame(std::string(filename))) {
        std::cout << "Joc salvat cu succes in " << filename << "\n";
    }
    else {
        std::cout << "Eroare la salvarea jocului!\n";
    }

    gameState.switchPlayer();

    GameState loadedState;
    if (loadedState.loadGame(std::string(filename))) {
        std::cout << "Joc incarcat cu succes.\n";
        std::cout << "Monedele jucatorului curent: "
            << loadedState.getCurrentPlayer().getCoins()
            << " (Verificare JSON ok)\n";
    }
    else {
        std::cout << "Eroare la incarcarea jocului!\n";
    }

  
    // TEST 6: Pyramid Logic
    printHeader("Logica Piramidei");

    GameState pyramidState;

    CardFactory testFactory;
    auto ageIDeck = testFactory.createAgeIDeck();

    // Inițializăm piramida pentru age 1 
    std::cout << "Piramida pentru age 1\n\n";
    pyramidState.initializeAge(1, ageIDeck);
    const auto& pyramid1 = pyramidState.getPyramid();

    if (pyramid1.size() == 20) {
        std::cout << "Piramida initializata cu 20 sloturi.\n";

        bool p0_avail = pyramidState.isCardAccessible(0); // Ar trebui sa fie libera
        bool p6_avail = pyramidState.isCardAccessible(6); // Ar trebui sa fie blocata
        bool p6_faceUp = pyramid1[6].m_isFaceUp;          // Ar trebui sa fie Face Down

        std::cout << "Card 0 accesibila: " << (p0_avail ? "DA" : "NU") << " (Asteptat: DA)\n";
        std::cout << "Card 6 accesibila: " << (p6_avail ? "DA" : "NU") << " (Asteptat: NU - blocata de 0 si 1)\n";
        std::cout << "Card 6 FaceUp: " << (p6_faceUp ? "DA" : "NU") << " (Asteptat: NU - face down)\n";

        // Simulam ca un jucator ia cartea 0 
        std::cout << "-> Jucatorul ia Card 0...\n";
        pyramidState.removeCardFromPyramid(0);

        // Verificam daca s-a deblocat 6
        p6_avail = pyramidState.isCardAccessible(6);
        std::cout << "Card 6 accesibila (dupa eliminare Card 0): " << (p6_avail ? "DA" : "NU") << " (Asteptat: NU - inca blocata de 1)\n";

        // Simulam ca un jucator ia cartea 1
        std::cout << "-> Jucatorul ia Card 1...\n";
        pyramidState.removeCardFromPyramid(1);

        // Verificam daca s-a deblocat 6
        p6_avail = pyramidState.isCardAccessible(6);
        std::cout << "Card 6 accesibila (dupa eliminare Card 1): " << (p6_avail ? "DA" : "NU") << " (Asteptat: DA)\n";
        
		// Verificam daca s-a intors
        p6_faceUp = pyramidState.getPyramid()[6].m_isFaceUp;
        std::cout << "Card 6 FaceUp: " << (p6_faceUp ? "DA" : "NU") << " (Asteptat: DA - s-a intors)\n";

    }
    else {
        std::cout << "[FAIL] Piramida nu a fost construita (Size != 20). Verifica GameState::initializeAge!\n";
    }

    // Inițializăm piramida pentru age 2 
    std::cout << "\n\nPiramida pentru age 2\n\n";

    auto ageIIDeck = testFactory.createAgeIIDeck();
    pyramidState.initializeAge(2, ageIIDeck);
    const auto& pyramid2 = pyramidState.getPyramid();

    if (pyramid2.size() == 20) {
        std::cout << "Piramida initializata cu 20 sloturi.\n";

        bool p0_avail = pyramidState.isCardAccessible(0); // Ar trebui sa fie libera
        bool p3_avail = pyramidState.isCardAccessible(3); // Ar trebui sa fie blocata
        bool p3_faceUp = pyramid2[3].m_isFaceUp;          // Ar trebui sa fie Face Down

        std::cout << "Card 0 accesibila: " << (p0_avail ? "DA" : "NU") << " (Asteptat: DA)\n";
        std::cout << "Card 3 accesibila: " << (p3_avail ? "DA" : "NU") << " (Asteptat: NU - blocata de 0 si 1)\n";
        std::cout << "Card 3 FaceUp: " << (p3_faceUp ? "DA" : "NU") << " (Asteptat: NU - face down)\n";

        // Simulam ca un jucator ia cartea 0 
        std::cout << "-> Jucatorul ia Card 0...\n";
        pyramidState.removeCardFromPyramid(0);

        // Verificam daca s-a deblocat 3
        p3_avail = pyramidState.isCardAccessible(3);
        std::cout << "Card 3 accesibila (dupa eliminare Card 0): " << (p3_avail ? "DA" : "NU") << " (Asteptat: NU - inca blocata de 1)\n";

        // Simulam ca un jucator ia cartea 1
        std::cout << "-> Jucatorul ia Card 1...\n";
        pyramidState.removeCardFromPyramid(1);

        // Verificam daca s-a deblocat 3
        p3_avail = pyramidState.isCardAccessible(3);
        std::cout << "Card 3 accesibila (dupa eliminare Card 1): " << (p3_avail ? "DA" : "NU") << " (Asteptat: DA)\n";
        
		// Verificam daca s-a intors
        p3_faceUp = pyramidState.getPyramid()[3].m_isFaceUp;
        std::cout << "Card 3 FaceUp: " << (p3_faceUp ? "DA" : "NU") << " (Asteptat: DA - s-a intors)\n";

    }
    else {
        std::cout << "[FAIL] Piramida nu a fost construita (Size != 20). Verifica GameState::initializeAge!\n";
    }

   
    // Inițializăm piramida pentru age 3
    std::cout << "\n\nPiramida pentru age 3\n\n";

    auto ageIIIDeck = testFactory.createAgeIIIDeck();
    pyramidState.initializeAge(3, ageIIIDeck);
    const auto& pyramid3 = pyramidState.getPyramid();

    if (pyramid3.size() == 20) {
        std::cout << "Piramida initializata cu 20 sloturi.\n";

        bool p1_avail = pyramidState.isCardAccessible(1); // Ar trebui sa fie libera
        bool p4_avail = pyramidState.isCardAccessible(4); // Ar trebui sa fie blocata
        bool p4_faceUp = pyramid3[4].m_isFaceUp;          // Ar trebui sa fie Face Down

        std::cout << "Card 1 accesibila: " << (p1_avail ? "DA" : "NU") << " (Asteptat: DA)\n";
        std::cout << "Card 4 accesibila: " << (p4_avail ? "DA" : "NU") << " (Asteptat: NU - blocata de 0 si 1)\n";
        std::cout << "Card 4 FaceUp: " << (p4_faceUp ? "DA" : "NU") << " (Asteptat: NU - face down)\n";

        // Simulam ca un jucator ia cartea 1 
        std::cout << "-> Jucatorul ia Card 1...\n";
        pyramidState.removeCardFromPyramid(1);

        // Verificam daca s-a deblocat 4
        p4_avail = pyramidState.isCardAccessible(4);
        std::cout << "Card 4 accesibila (dupa eliminare Card 1): " << (p4_avail ? "DA" : "NU") << " (Asteptat: DA)\n";

		// Verificam daca s-a intors
        p4_faceUp = pyramidState.getPyramid()[4].m_isFaceUp;
        std::cout << "Card 4 FaceUp: " << (p4_faceUp ? "DA" : "NU") << " (Asteptat: DA - s-a intors)\n";

    }
    else {
        std::cout << "[FAIL] Piramida nu a fost construita (Size != 20). Verifica GameState::initializeAge!\n";
    }

    return 0;
}