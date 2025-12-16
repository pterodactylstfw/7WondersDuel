#include "MainWindow.h"
#include "./ui_MainWindow.h" 
#include <QFileDialog> 
#include <QMessageBox>
#include <QInputDialog>
#include <QString> 
#include <QDebug>
#include <QApplication>

MainWindow::MainWindow(QWidget* parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
    , m_game(*this)
{
    ui->setupUi(this); 

    ui->labelMilitaryBoard->setPixmap(QPixmap(":/assets/military_board.png"));
    ui->labelMilitaryLead->setPixmap(QPixmap(":/assets/military_lead.png"));

	ui->stackedWidget->setCurrentIndex(0);

    connect(ui->btnStart, &QPushButton::clicked, this, &MainWindow::onBtnStartClicked);
    connect(ui->btnLoad, &QPushButton::clicked, this, &MainWindow::onBtnLoadClicked);
	connect(ui->btnExit, &QPushButton::clicked, this, &MainWindow::onBtnExitClicked);

    connect(ui->btnBack, &QPushButton::clicked, this, [this]() {
        ui->stackedWidget->setCurrentIndex(0); // Inapoi la meniu
        });

    this->setWindowTitle("7 Wonders Duel");
}

void MainWindow::onMessage(const std::string& message) {
    // Implementare temporara
    qDebug() << "Message:" << QString::fromStdString(message);
}

void MainWindow::onError(const std::string& error) {
    qDebug() << "Error:" << QString::fromStdString(error);
}

void MainWindow::onStateUpdated() {
    updateGameUI();
}

int MainWindow::askInt(int min, int max, const std::string& prompt) {
    return min; // Default
}

ResourceType MainWindow::askResourceSelection(const std::vector<ResourceType>& options, const std::string& prompt) {
    if(options.empty()) return ResourceType::NONE;
    return options[0]; // Default
}

int MainWindow::askWonderSelection(const std::vector<std::unique_ptr<Wonder>>& wonders, const std::string& playerName) {
    return 0; // Default
}

int MainWindow::askTokenSelection(const std::vector<std::unique_ptr<ProgressToken>>& tokens, const std::string& prompt) {
    return 0; // Default
}

int MainWindow::askCardSelectionFromList(const std::vector<std::reference_wrapper<const Card>>& cards, const std::string& prompt) {
    return 0; // Default
}


void MainWindow::onBtnStartClicked()
{
    bool ok1, ok2;

    QString player1Name = QInputDialog::getText(this, "Player 1", "Enter name for Player 1:", QLineEdit::Normal, "Player 1", &ok1);
    if (!ok1 || player1Name.isEmpty()) {
        return;
    }

    QString player2Name = QInputDialog::getText(this, "Player 2", "Enter name for Player 2:", QLineEdit::Normal, "Player 2", &ok2);
    if (!ok2 || player2Name.isEmpty()) {
        return;
    }

	m_game.startNewGame(player1Name.toStdString(), player2Name.toStdString());

    ui->stackedWidget->setCurrentIndex(1);
	qDebug() << "Switched to game view";

	updateGameUI();
	qDebug() << "Game UI updated";

	QApplication::processEvents();
}

void MainWindow::onBtnLoadClicked()
{
    QString fileName = QFileDialog::getOpenFileName(this,
        "Load Game",
        "",
        "JSON Files (*.json);;All Files(*)"
	);

    if (fileName.isEmpty())
        return;

    try {
        m_game.loadGame(fileName.toStdString());

        ui->stackedWidget->setCurrentIndex(1);
        QMessageBox::information(this, "Success", "Game loaded successfully!");

		updateGameUI();
    }
    catch (const std::exception& e) {
        QMessageBox::critical(this, "Error", QString("Could not load game:\n").arg(e.what()));
    }
}

void MainWindow::onBtnExitClicked() {
    QApplication::quit();
}

void MainWindow::updateGameUI()
{
    if (m_game.isGameOver()) {
        return;
    }

	const auto& gameState = m_game.getGameState();

	updatePlayerPanel(gameState.getCurrentPlayer(), false);
	updatePlayerPanel(gameState.getOpponent(), true);

    updateMilitaryTrack();
}

void MainWindow::updatePlayerPanel(const Player& player, bool isOpponent)
{
    const QString styleTemplate =
        "background-image: url(%1);"
        "background-repeat: no-repeat;"
        "background-position: center;"
        "border-radius: 20px;"
        "color: white;"
        "font-size: 16px;"
        "font-weight: bold;"
        "qproperty-alignment: 'AlignCenter';";

    if (isOpponent) {
		ui->labelOpponentName->setText(QString::fromStdString(player.getName()));

        ui->labelOpponentCoins->setStyleSheet(styleTemplate.arg(":/assets/coins.png"));
        ui->labelOpponentCoins->setText(QString::number(player.getCoins()));

        ui->labelOpponentVP->setStyleSheet(styleTemplate.arg(":/assets/victorypoints.png"));
        ui->labelOpponentVP->setText(QString::number(player.getVictoryPoints()));
    }
	else {
		ui->labelCurrentPlayerName->setText(QString::fromStdString(player.getName()));

        ui->labelCurrentPlayerCoins->setStyleSheet(styleTemplate.arg(":/assets/coins.png"));
        ui->labelCurrentPlayerCoins->setText(QString::number(player.getCoins()));

        ui->labelCurrentPlayerVP->setStyleSheet(styleTemplate.arg(":/assets/victorypoints.png"));
        ui->labelCurrentPlayerVP->setText(QString::number(player.getVictoryPoints()));
    }
}

void MainWindow::updateMilitaryTrack()
{
    const auto& gameState = m_game.getGameState();
    int militaryPosition = gameState.getMilitaryPosition();

    const int startY = ui->labelMilitaryBoard->y();     
    const int endY = ui->labelMilitaryBoard->y() + ui->labelMilitaryBoard->height();
    const int trackHeight = endY - startY;

    int leadX = ui->labelMilitaryBoard->x() + (ui->labelMilitaryBoard->width() / 2) - (ui->labelMilitaryLead->width() / 2) - 5;

    double percentage = (militaryPosition + 9.0) / 18.0;

    int leadY = startY + (trackHeight * percentage) - (ui->labelMilitaryLead->height() / 2);

    ui->labelMilitaryLead->move(leadX, leadY);
}

MainWindow::~MainWindow()
{
    delete ui;
}