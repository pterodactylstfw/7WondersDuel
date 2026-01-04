#pragma once
#include <string>

#include "Constants.h"
#include "CoreExport.h"

class Card;
class ProgressToken;
class Wonder;

class CORE_API IGameView {
public:
    virtual ~IGameView() = default;


    virtual void onMessage(const std::string& message) = 0;
    virtual void onError(const std::string& errorMessage) = 0;

    virtual int askInt(int min, int max, const std::string& prompt) = 0; // input generic


    virtual ResourceType askResourceSelection(const std::vector<ResourceType>& options, const std::string& prompt) = 0; // masonry / architecture

    virtual int askWonderSelection(const std::vector<std::unique_ptr<Wonder>>& wonders, const std::string& playerName) = 0; // selectare minuni

    // efecte specifice
    virtual int askCardSelectionFromList(const std::vector<std::reference_wrapper<const Card>>& cards, const std::string& prompt) = 0;
    virtual int askTokenSelection(const std::vector<std::unique_ptr<ProgressToken>>& tokens, const std::string& prompt) = 0;

    //starea jocului s-a schimbat => update ui
    virtual void onStateUpdated() = 0;
};
