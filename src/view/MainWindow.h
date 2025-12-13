#pragma once
#include <QMainWindow>
#include "GameController.h"

namespace Ui {
    class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget* parent = nullptr);
    ~MainWindow();

private slots:
    void onBtnStartClicked();
    void onBtnLoadClicked();
	void onBtnExitClicked();

private:
    Ui::MainWindow* ui;
    GameController m_game;

private:
    void updateGameUI();
    void updatePlayerPanel(const Player& player, bool isOpponent);
    void updateMilitaryTrack();
};