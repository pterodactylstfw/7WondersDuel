#pragma once
#include <QMainWindow>
#include <QPushButton>
#include <QCloseEvent>
#include <QTimer>
#include <array>
#include <QLabel>

#include "GameController.h"
#include "IGameView.h"
#include "Constants.h"
#include "NetworkClient.h"
#include "AIController.h"
#include "server/GameServer.h"

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
    int askWonderSelection(const std::array<std::unique_ptr<Wonder>,GameConstants::WONDERS_PER_PLAYER>& wonders, const std::string& playerName) override;
    int askTokenSelection(const std::vector<std::unique_ptr<ProgressToken>>& tokens, const std::string& prompt) override;
    int askCardSelectionFromList(const std::vector<std::reference_wrapper<const Card>>& cards, const std::string& prompt) override;

private slots:
    void onBtnStartClicked();
    void onBtnLoadClicked();
	void onBtnExitClicked();
    void onBtnHintClicked();

private:
    struct CardPosition {
        int x;
        int y;
    };

    Ui::MainWindow* ui;
    GameController m_game;
    AIDifficulty m_aiDifficulty = AIDifficulty::HARD;
    QPushButton* m_btnHint = nullptr;

    NetworkClient* m_netClient;
    std::unique_ptr<GameState> m_networkState;
    QString m_myUserName; // online
    int m_myIndex = -1;

    bool m_isOnlineMode = false;

    const GameState& getCurrentGameState() const;

    std::vector<QPushButton*> m_cardButtons;
    std::vector<QPushButton*> m_myWonderButtons;
    std::vector<QPushButton*> m_progressTokenButtons;
    QList<QLabel*> m_activeMessages;

    std::array<CardPosition, GameConstants::CARDS_PER_AGE> m_age1Layout;
    std::array<CardPosition, GameConstants::CARDS_PER_AGE> m_age2Layout;
    std::array<CardPosition, GameConstants::CARDS_PER_AGE> m_age3Layout;

    std::unique_ptr<GameServer> m_integratedServer; // server hibrid host - sa nu mai pornesc aplicatia cu exe

    void updatePlayerArea(const Player& player, QWidget* wondersArea, QWidget* cityArea);
    QWidget* createColorColumn(const std::vector<std::reference_wrapper<const Card>>& cards, int width, int height);
	void fillWondersArea(const Player& player, QWidget* container, int width, int height, bool isInteractive);
    QPixmap addTextToImage(const QPixmap& baseImage, const QString& text, int width, int height);

    QWidget* createPlayerEndGameWidget(const Player& player, int totalScore, bool isWinner);

    void showFloatingText(const QString& text, const QString& colorStyle);
    void showHintText(const QString& text);
    void showActionDialog(int cardIndex);
    void showGameOverScreen();

    void drawProgressTokens();
    void drawDraftBoard();
    void cleanupVisuals();
    void highlightCardUI(int cardIndex);
protected:
    void resizeEvent(QResizeEvent* event) override;
    void closeEvent(QCloseEvent* event) override;

private:
    void updateGameUI();
    void updatePlayerPanel(const Player& player, bool isOpponent);
    void updateMilitaryTrack();

    void setupLayouts();
    void updateCardStructures();

    void removeMessageLabel(QLabel* label);
    void nonBlockingWait(int milliseconds);

    void setupNetworkConnections();
    void resetNetwork();
};