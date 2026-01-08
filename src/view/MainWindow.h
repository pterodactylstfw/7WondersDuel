#pragma once
#include <QMainWindow>
#include <QPushButton>
#include <QCloseEvent>
#include <QTimer>
#include <array>

#include "GameController.h"
#include "IGameView.h"
#include "Constants.h"
#include "AIController.h"
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

    std::vector<QPushButton*> m_cardButtons;
    std::vector<QPushButton*> m_progressTokenButtons;

    std::array<CardPosition, GameConstants::CARDS_PER_AGE> m_age1Layout;
    std::array<CardPosition, GameConstants::CARDS_PER_AGE> m_age2Layout;
    std::array<CardPosition, GameConstants::CARDS_PER_AGE> m_age3Layout;

    void updatePlayerArea(const Player& player, QWidget* wondersArea, QWidget* cityArea);
    QWidget* createColorColumn(const std::vector<std::reference_wrapper<const Card>>& cards, int width, int height);
    QPixmap addTextToImage(const QPixmap& baseImage, const QString& text, int width, int height);

    void showFloatingText(const QString& text, const QString& colorStyle);
    void showHintText(const QString& text);
    void showActionDialog(int cardIndex);

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

};