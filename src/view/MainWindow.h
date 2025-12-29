#pragma once
#include <QMainWindow>
#include <QPushButton>
#include <array>

#include "GameController.h"
#include "IGameView.h"
#include "Constants.h"

namespace Ui {
    class MainWindow;
}

class MainWindow : public QMainWindow, public IGameView
{
    Q_OBJECT

public:
    MainWindow(QWidget* parent = nullptr);
    ~MainWindow();

    void onMessage(const std::string& message) override;
    void onError(const std::string& error) override;
    void onStateUpdated() override;
    int askInt(int min, int max, const std::string& prompt) override;
    ResourceType askResourceSelection(const std::vector<ResourceType>& options, const std::string& prompt) override;
    int askWonderSelection(const std::vector<std::unique_ptr<Wonder>>& wonders, const std::string& playerName) override;
    int askTokenSelection(const std::vector<std::unique_ptr<ProgressToken>>& tokens, const std::string& prompt) override;
    int askCardSelectionFromList(const std::vector<std::reference_wrapper<const Card>>& cards, const std::string& prompt) override;

private slots:
    void onBtnStartClicked();
    void onBtnLoadClicked();
	void onBtnExitClicked();

private:
    struct CardPosition {
        int x;
        int y;
    };

    Ui::MainWindow* ui;
    GameController m_game;

    std::vector<QPushButton*> m_cardButtons;

    std::array<CardPosition, GameConstants::CARDS_PER_AGE> m_age1Layout;
    std::array<CardPosition, GameConstants::CARDS_PER_AGE> m_age2Layout;
    std::array<CardPosition, GameConstants::CARDS_PER_AGE> m_age3Layout;

protected:
    void resizeEvent(QResizeEvent* event) override;

private:
    void updateGameUI();
    void updatePlayerPanel(const Player& player, bool isOpponent);
    void updateMilitaryTrack();

    void setupLayouts();
    void updateCardStructures();

};