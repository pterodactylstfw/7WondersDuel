#include "MainWindow.h"
#include "./ui_MainWindow.h" 
#include <QFileDialog> 
#include <QMessageBox>
#include <QInputDialog>
#include <QString> 
#include <QDebug>
#include <QApplication>
#include <qpainter.h>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QLabel>

MainWindow::MainWindow(QWidget* parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
    , m_game(*this)
{
    this->setWindowIcon(QIcon(":/assets/coins.png"));
    ui->setupUi(this); 

    ui->labelMilitaryBoard->setPixmap(QPixmap(":/assets/military_board.png"));
    ui->labelMilitaryLead->setPixmap(QPixmap(":/assets/military_lead.png"));

	ui->stackedWidget->setCurrentIndex(0);

    connect(ui->btnStart, &QPushButton::clicked, this, &MainWindow::onBtnStartClicked);
    connect(ui->btnLoad, &QPushButton::clicked, this, &MainWindow::onBtnLoadClicked);
	connect(ui->btnExit, &QPushButton::clicked, this, &MainWindow::onBtnExitClicked);

    connect(ui->btnBack, &QPushButton::clicked, this, [this]() {
        if (m_game.hasGameStarted() && !m_game.isGameOver()) {
            auto reply = QMessageBox::question(this, "Confirm",
                "Going back will end the current game. Are you sure?",
                QMessageBox::Yes | QMessageBox::No);

            if (reply == QMessageBox::No) return;
        }

		cleanupVisuals();
        ui->stackedWidget->setCurrentIndex(0); // Inapoi la meniu
        });

    ui->actionSave->setEnabled(false);
    connect(ui->actionSave, &QAction::triggered, this, [this]() {
        QString fileName = QFileDialog::getSaveFileName(this, "Save Game", "", "JSON Files (*.json)");
        if (!fileName.isEmpty()) {
            try {
                m_game.saveGame(fileName.toStdString());
                QMessageBox::information(this, "Saved", "Game saved successfully!");
            }
            catch (const std::exception& e) {
                QMessageBox::critical(this, "Error", QString("Save failed: %1").arg(e.what()));
            }
        }
        });

    ui->actionLoad->setVisible(false);
    connect(ui->actionLoad, &QAction::triggered, this, &MainWindow::onBtnLoadClicked);

    connect(ui->actionExit, &QAction::triggered, this, &MainWindow::onBtnExitClicked);

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
    if (!m_game.hasGameStarted()) return;

    updateGameUI();

    const auto& state = m_game.getGameState();

    if (state.getCurrentPhase() == GamePhase::DRAFTING) {
        drawDraftBoard();
    }
    else {
        updateCardStructures();
        this->setWindowTitle("7 Wonders Duel - Age " + QString::number(state.getCurrentAge()));
    }

    updatePlayerArea(state.getCurrentPlayer(), ui->playerWonders, ui->playerCards);
    updatePlayerArea(state.getOpponent(), ui->opponentWonders, ui->opponentCards);
}

int MainWindow::askInt(int min, int max, const std::string& prompt) {
    bool ok;
    int val = QInputDialog::getInt(this, "Input", QString::fromStdString(prompt), min, min, max, 1, &ok);
    return ok ? val : min;
}

ResourceType MainWindow::askResourceSelection(const std::vector<ResourceType>& options, const std::string& prompt) {
    if (options.empty()) return ResourceType::NONE;

    QStringList items;
    for (const auto& r : options) {
        items << QString::fromStdString(resourceToString(r));
    }

    bool ok;
    QString item = QInputDialog::getItem(this, "Select Resource",
        QString::fromStdString(prompt),
        items, 0, false, &ok);

    if (ok && !item.isEmpty()) {
        int index = items.indexOf(item);
        if (index >= 0 && index < options.size()) return options[index];
    }
    return options[0];
}

int MainWindow::askWonderSelection(const std::vector<std::unique_ptr<Wonder>>& wonders, const std::string& playerName) {
    if (wonders.empty()) return -1;

    QStringList items;
    for (const auto& w : wonders) {
        items << QString::fromStdString(w->getName());
    }

    bool ok;
    QString item = QInputDialog::getItem(this, "Select Wonder",
        QString::fromStdString(playerName + ", choose a Wonder:"),
        items, 0, false, &ok);

    if (ok && !item.isEmpty()) {
        return items.indexOf(item);
    }
    return 0;
}

int MainWindow::askTokenSelection(const std::vector<std::unique_ptr<ProgressToken>>& tokens, const std::string& prompt) {
    if (tokens.empty()) return -1;

    QStringList items;
    for (const auto& token : tokens) {
        QString label = QString::fromStdString(std::string(token->getName()));
        items << label;
    }

    bool ok;
    QString item = QInputDialog::getItem(this,
        "Select Progress Token",
        QString::fromStdString(prompt),
        items,
        0,
        false,
        &ok);

    if (ok && !item.isEmpty()) {
        return items.indexOf(item);
    }

    return 0; // default: primul
}

int MainWindow::askCardSelectionFromList(const std::vector<std::reference_wrapper<const Card>>& cards, const std::string& prompt) {
    if (cards.empty()) return -1;

    QStringList items;
    for (const auto& c : cards) {
        items << QString::fromStdString(c.get().getName());
    }

    bool ok;
    QString item = QInputDialog::getItem(this, "Select Card",
        QString::fromStdString(prompt),
        items, 0, false, &ok);

    if (ok && !item.isEmpty()) {
        return items.indexOf(item);
    }
    return 0;
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

    ui->stackedWidget->setCurrentIndex(1);
	qDebug() << "Switched to game view";

	updateGameUI();
	qDebug() << "Game UI updated";

	QApplication::processEvents();
	setupLayouts();
    m_game.startNewGame(player1Name.toStdString(), player2Name.toStdString());

    ui->actionSave->setEnabled(true);
    ui->actionLoad->setVisible(true);


    ui->stackedWidget->setCurrentIndex(1);
	updateGameUI();
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

        ui->actionSave->setEnabled(true);

        ui->stackedWidget->setCurrentIndex(1);

        QApplication::processEvents();

		setupLayouts();
        onStateUpdated();

        QMessageBox::information(this, "Success", "Game loaded successfully!");
    }
    catch (const std::exception& e) {
        QMessageBox::critical(this, "Error", QString("Could not load game:\n").arg(e.what()));
    }
}

void MainWindow::onBtnExitClicked() {
    QApplication::quit();
}

void MainWindow::updatePlayerArea(const Player& player, QWidget* wondersArea, QWidget* cityArea)
{
    if (!wondersArea || !cityArea) return;

    int windowW = this->width();

    int wonderW = windowW * 0.12;
    int wonderH = wonderW * 0.66;
    if (wonderW < 80) { wonderW = 80; wonderH = 55; }
    if (wonderW > 250) { wonderW = 250; wonderH = 166; }

    int cardW = windowW * 0.07;
    int cardH = cardW * 1.5;
    if (cardW < 50) { cardW = 50; cardH = 75; }
    if (cardW > 180) { cardW = 180; cardH = 270; }

    // MINUNI
    // stergem butoanele vechi
    qDeleteAll(wondersArea->findChildren<QWidget*>(QString(), Qt::FindDirectChildrenOnly));

    // asiguram layout orizontal
    if (!wondersArea->layout()) {
        QHBoxLayout* l = new QHBoxLayout(wondersArea);
        l->setSpacing(10);
        l->setContentsMargins(0, 0, 0, 0);
    }

    const auto& wonders = player.getWonders();
    for (const auto& wonder : wonders) {
        if (!wonder) continue; 

        QPushButton* btn = new QPushButton(wondersArea);
        btn->setFixedSize(wonderW, wonderH); 

        QString imgPath = QString::fromStdString(wonder->getImagePath());
        QPixmap pix(imgPath);

        if (!pix.isNull()) {
            btn->setIcon(QIcon(pix));
            btn->setIconSize(QSize(wonderW - 6, wonderH - 6));
        }
        else {
            btn->setText(QString::fromStdString(wonder->getName()));
        }

        if (wonder->isBuilt()) {
            btn->setStyleSheet(
                "QPushButton { border: 3px solid #FFD700; background-color: black; border-radius: 4px; }"
                "QPushButton:hover { border: 3px solid white; }"
            );
            btn->setToolTip("BUILT: " + QString::fromStdString(wonder->getDescription()));
        }
        else {
            btn->setStyleSheet(
                "QPushButton { border: 2px dashed gray; background-color: rgba(0, 0, 0, 50); border-radius: 4px; }"
                "QPushButton:hover { border: 2px solid orange; }"
            );
            btn->setToolTip("COST: " + QString::fromStdString(wonder->getCost().toString()) +
                "\nEFFECT: " + QString::fromStdString(wonder->getDescription()));
        }

        wondersArea->layout()->addWidget(btn);
    }

    // CARTILE
    
    // stergem coloanele vechi
    qDeleteAll(cityArea->findChildren<QWidget*>(QString(), Qt::FindDirectChildrenOnly));

	// asiguram layout orizontal
    if (!cityArea->layout()) {
        QHBoxLayout* l = new QHBoxLayout(cityArea);
        l->setSpacing(5);
        l->setContentsMargins(0, 0, 0, 0);
    }

	// luam culorile in ordine
    std::vector<CardColor> colors = {
        CardColor::BROWN, CardColor::GREY, CardColor::BLUE,
        CardColor::GREEN, CardColor::YELLOW, CardColor::RED, CardColor::PURPLE
    };

    for (const auto& color : colors) {
        auto cards = player.getCardsOfType(color);
        if (!cards.empty()) {
			// cream coloana pentru aceasta culoare
            QWidget* col = createColorColumn(cards, cardW, cardH);
            cityArea->layout()->addWidget(col);
        }
    }
}

QWidget* MainWindow::createColorColumn(const std::vector<std::reference_wrapper<const Card>>& cards, int width, int height)
{
    QWidget* columnWidget = new QWidget();
    QVBoxLayout* layout = new QVBoxLayout(columnWidget);

    int visibleHeader = height * 0.22;
    int negativeSpacing = -(height - visibleHeader);

    layout->setSpacing(negativeSpacing);
    layout->setContentsMargins(0, 0, 0, 0);
    layout->setAlignment(Qt::AlignTop);

    for (const auto& cardRef : cards) {
        const Card& card = cardRef.get();
        QPushButton* btn = new QPushButton(columnWidget);

        btn->setFixedSize(width, height); 

        QString imgPath = QString::fromStdString(card.getImagePath());
        QPixmap pix(imgPath);
        if (!pix.isNull()) {
            btn->setIcon(QIcon(pix));
            btn->setIconSize(QSize(width, height));
            btn->setStyleSheet("border: none; background: transparent;");
        }
        else {
            btn->setText(QString::fromStdString(card.getName()));
            btn->setStyleSheet("background-color: gray; color: white; border: 1px solid black;");
        }

        btn->setToolTip(QString::fromStdString(card.displayCardInfo()));
        btn->raise(); 
        layout->addWidget(btn);
    }

	columnWidget->setFixedWidth(width);
    return columnWidget;
}

void MainWindow::showActionDialog(int cardIndex)
{
    auto cardView = m_game.getGameState().getCardView(cardIndex);
    if (!cardView.has_value()) return;
    const Card& card = cardView->get();

    // cream o fereastra separata pentru cele 3 butoane
    QDialog dialog(this);
    dialog.setWindowTitle("Action for: " + QString::fromStdString(card.getName()));
    dialog.setMinimumWidth(300);

    QVBoxLayout* layout = new QVBoxLayout(&dialog);

    QLabel* infoLabel = new QLabel("Choose an action:", &dialog);
    infoLabel->setAlignment(Qt::AlignCenter);
    layout->addWidget(infoLabel);

	// primul buton pentru CONSTRUCT BUILDING
    QPushButton* btnBuild = new QPushButton("Construct Building", &dialog);
    btnBuild->setStyleSheet("background-color: #4CAF50; color: white; padding: 10px;");
    layout->addWidget(btnBuild);

	// al doilea buton pentru DISCARD FOR COINS
    QPushButton* btnDiscard = new QPushButton("Discard for Coins", &dialog);
    btnDiscard->setStyleSheet("background-color: #f44336; color: white; padding: 10px;");
    layout->addWidget(btnDiscard);

	// al treilea buton pentru CONSTRUCT WONDER
    QPushButton* btnWonder = new QPushButton("Construct Wonder", &dialog);
    btnWonder->setStyleSheet("background-color: #FF9800; color: white; padding: 10px;");
    layout->addWidget(btnWonder);

	// CONSTRUCT BUILDING
    connect(btnBuild, &QPushButton::clicked, [&]() {
		// apelam controllerul sa execute actiunea
        bool success = m_game.executeAction(cardIndex, PlayerAction::CONSTRUCT_BUILDING);
        if (success) {
            dialog.accept(); 
        }
        else {
            dialog.reject(); 
        }
        });

	// DISCARD FOR COINS
    connect(btnDiscard, &QPushButton::clicked, [&]() {
        bool success = m_game.executeAction(cardIndex, PlayerAction::DISCARD_FOR_COINS);
        if (success) dialog.accept();
        else dialog.reject();
        });

	// CONSTRUCT WONDER
    /*
    connect(btnWonder, &QPushButton::clicked, [&]() {
        std::string currentPlayerName = m_game.getGameState().getCurrentPlayer().getName();
        int wonderIndex = selectWonderIndex(currentPlayerName);

        if (wonderIndex != -1) { 
            bool success = m_game.executeAction(cardIndex, PlayerAction::CONSTRUCT_WONDER, wonderIndex);
            if (success) dialog.accept();
        }
        });
        */

    // afisam fereastra
    dialog.exec();
}

void MainWindow::drawDraftBoard()
{
    for (auto btn : m_cardButtons) {
        delete btn;
    }
    m_cardButtons.clear();

	const auto& gameState = m_game.getGameState();
	const auto& draftedWonders = gameState.getDraftedWonders();
    
	if (draftedWonders.empty()) return;

    int containerW = ui->cardContainer->width();
    int containerH = ui->cardContainer->height();

    int wonderW = containerW * 0.20;
    int wonderH = containerH * 0.40;
    wonderH = wonderW * 0.66; 

    int spacing = containerW * 0.05;

    int totalContentWidth = (draftedWonders.size() * wonderW) + ((draftedWonders.size() - 1) * spacing);
    int startX = (containerW - totalContentWidth) / 2;
    int startY = (containerH - wonderH) / 2;

    if (startX < 0) startX = 0;
    if (startY < 0) startY = 0;

    for (size_t i = 0; i < draftedWonders.size(); i++) {
		const auto& wonder = draftedWonders[i];
		if (!wonder) continue;

        QPushButton* btn = new QPushButton(ui->cardContainer);
        int x = startX + i * (wonderW + spacing);
        int y = startY;

        btn->setGeometry(x, y, wonderW, wonderH);

        QString imgPath = QString::fromStdString(wonder->getImagePath());
        QPixmap pix(imgPath);

        if (!pix.isNull()) {
            btn->setIcon(QIcon(pix));
            btn->setIconSize(QSize(wonderW, wonderH));
            btn->setStyleSheet("border: none; background: transparent;");
        }
        else {
            btn->setText(QString::fromStdString(wonder->getName()));
            btn->setStyleSheet("background-color: orange; border: 2px solid white; font-weight: bold;");
        }

        btn->setToolTip(QString::fromStdString(wonder->getDescription()));

        connect(btn, &QPushButton::clicked, [this, i]() {
            bool success = m_game.pickWonder(i);
            
            if (!success) {
                QMessageBox::warning(this, "Atenție", "Nu poți alege această minune acum.");
            }
            });

        btn->show();
        m_cardButtons.push_back(btn); 
    }

    this->setWindowTitle("DRAFT PHASE: " + QString::fromStdString(gameState.getCurrentPlayer().getName()) + " is choosing...");
}

void MainWindow::cleanupVisuals() // pentru resetarea UI-ului la iesirea din joc
{
    for (QPushButton* btn : m_cardButtons) {
        delete btn;
    }
    m_cardButtons.clear();


    if (ui->playerWonders) qDeleteAll(ui->playerWonders->findChildren<QWidget*>(QString(), Qt::FindDirectChildrenOnly));
    if (ui->playerCards) qDeleteAll(ui->playerCards->findChildren<QWidget*>(QString(), Qt::FindDirectChildrenOnly));

    if (ui->opponentWonders) qDeleteAll(ui->opponentWonders->findChildren<QWidget*>(QString(), Qt::FindDirectChildrenOnly));
    if (ui->opponentCards) qDeleteAll(ui->opponentCards->findChildren<QWidget*>(QString(), Qt::FindDirectChildrenOnly));

    ui->labelCurrentPlayerName->setText("-");
    ui->labelOpponentName->setText("-");

}

void MainWindow::resizeEvent(QResizeEvent* event)
{
    QMainWindow::resizeEvent(event); 

    setupLayouts();
    /*if (ui->stackedWidget->currentIndex() == 1 && m_game.hasGameStarted()) 
    {
		 onStateUpdated(); //crash aici uneori
    }*/
}

void MainWindow::closeEvent(QCloseEvent* event)
{

    if (m_game.hasGameStarted() && !m_game.isGameOver()) {
        QMessageBox::StandardButton reply;
        reply = QMessageBox::question(this, "Quit Game",
            "Are you sure you want to quit? Unsaved progress will be lost.",
            QMessageBox::Yes | QMessageBox::No);

        if (reply == QMessageBox::No) {
            event->ignore();
        }
        else {
            event->accept();
        }
    }
    else {
        event->accept(); // meniu/game over -> inchid direct
    }
}

void MainWindow::updateGameUI()
{
    if (!m_game.hasGameStarted()) return;
    if (m_game.isGameOver()) return;
    

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

    QWidget* panel = ui->militaryPanel;

    const int trackHeight = panel->height();

    int leadX = (panel->width() / 2) - (ui->labelMilitaryLead->width() / 2);

    double percentage = (militaryPosition + 9.0) / 18.0;
    int leadY = (trackHeight * (1.0 - percentage)) - (ui->labelMilitaryLead->height() / 2);

    ui->labelMilitaryLead->move(leadX, leadY);
}

void MainWindow::setupLayouts()
{
    const QSize containerSize = ui->cardContainer->size();
    qDebug() << "SetupLayouts Container Size:" << containerSize;

    // Verifică validitatea containerului
    if (!containerSize.isValid() || containerSize.width() <= 0) return;

    const int availableWidth = containerSize.width();
    const int availableHeight = containerSize.height();

    // --- MASURILE TALE ORIGINALE ---
    const int pyramidTotalHeight = availableHeight * 0.9;
    const int cardH = pyramidTotalHeight / 3;
    const int cardW = cardH / 1.5;

    const int horizontalGap = 5;
    const int verticalOverlap = cardH / 2;
    const int stepX = cardW + horizontalGap;

    const int totalWidthOfBottomRow = 6 * cardW + 5 * horizontalGap;
    const int totalHeightOfPyramid = cardH + 4 * verticalOverlap; // Inaltimea pentru 5 randuri (Age 1 & 2)

    // StartY calculat pentru piramidele cu 5 rânduri (Age 1 si 2)
    const int startX = (availableWidth - totalWidthOfBottomRow) / 2;
    const int startY = (availableHeight - totalHeightOfPyramid) / 2;

    // age 1 layout

    // Randul 1 (jos, 6 carti)
    for (int i = 0; i < 6; ++i) m_age1Layout[i] = { startX + i * stepX, startY + 4 * verticalOverlap };
    // Randul 2 (5 carti)
    for (int i = 0; i < 5; ++i) m_age1Layout[i + 6] = { startX + (stepX / 2) + i * stepX, startY + 3 * verticalOverlap };
    // Randul 3 (4 carti)
    for (int i = 0; i < 4; ++i) m_age1Layout[i + 11] = { startX + stepX + i * stepX, startY + 2 * verticalOverlap };
    // Randul 4 (3 carti)
    for (int i = 0; i < 3; ++i) m_age1Layout[i + 15] = { startX + stepX + (stepX / 2) + i * stepX, startY + 1 * verticalOverlap };
    // Randul 5 (sus, 2 carti)
    for (int i = 0; i < 2; ++i) m_age1Layout[i + 18] = { startX + (2 * stepX) + (i * stepX), startY };


    // age 2 layout

    // Randul 5 (jos 2 carti)
    for (int i = 0; i < 2; ++i) m_age2Layout[i] = { startX + (2 * stepX) + i * stepX, startY + 4 * verticalOverlap };

    // Randul 4 (3 carti)
    for (int i = 0; i < 3; ++i) m_age2Layout[2 + i] = { startX + stepX + (stepX / 2) + i * stepX, startY + 3 * verticalOverlap };

    // Randul 3 (4 carti)
    for (int i = 0; i < 4; ++i) m_age2Layout[5 + i] = { startX + stepX + i * stepX, startY + 2 * verticalOverlap };

    // Randul 2 (5 carti)
    for (int i = 0; i < 5; ++i) m_age2Layout[9 + i] = { startX + (stepX / 2) + i * stepX, startY + 1 * verticalOverlap };

    // Randul 1 (sus 6 carti)
    for (int i = 0; i < 6; ++i) m_age2Layout[14 + i] = { startX + i * stepX, startY };


    // age 3 layout
    const int startY_Age3 = startY - verticalOverlap;

    // Randul 7 (jos 2 carti)
    for (int i = 0; i < 2; ++i) m_age3Layout[i] = { startX + (2 * stepX) + i * stepX, startY_Age3 + 6 * verticalOverlap };

    // Randul 6 (3 carti)
    for (int i = 0; i < 3; ++i) m_age3Layout[2 + i] = { startX + stepX + (stepX / 2) + i * stepX, startY_Age3 + 5 * verticalOverlap };

    // Randul 5 (4 carti)
    for (int i = 0; i < 4; ++i) m_age3Layout[5 + i] = { startX + stepX + i * stepX, startY_Age3 + 4 * verticalOverlap };

    // Randul 4 (mijloc - Guilds - 2 carti)
    for (int i = 0; i < 2; ++i) m_age3Layout[9 + i] = { startX + (2 * stepX) + i * stepX, startY_Age3 + 3 * verticalOverlap };

    // Randul 3 (4 carti)
    for (int i = 0; i < 4; ++i) m_age3Layout[11 + i] = { startX + stepX + i * stepX, startY_Age3 + 2 * verticalOverlap };

    // Randul 2 (3 carti)
    for (int i = 0; i < 3; ++i) m_age3Layout[15 + i] = { startX + stepX + (stepX / 2) + i * stepX, startY_Age3 + 1 * verticalOverlap };

    // Randul 1 (sus 2 carti)
    for (int i = 0; i < 2; ++i) m_age3Layout[18 + i] = { startX + (2 * stepX) + i * stepX, startY_Age3 };
}

void MainWindow::updateCardStructures()
{
    /*for (QPushButton* button : m_cardButtons) {
		delete button; // crash uneori
    }
	m_cardButtons.clear();*/

    for (QPushButton* button : m_cardButtons) {
        // sterge cand revii in event loop
        button->deleteLater();
    }
    m_cardButtons.clear();

    const auto& gameState = m_game.getGameState();
    const auto& pyramid = gameState.getPyramid();
	const int currentAge = gameState.getCurrentAge();

    const std::array<CardPosition, 20>* currentLayout = nullptr;
    if (currentAge == 1) {
        currentLayout = &m_age1Layout;
    }
    else if (currentAge == 2) {
        currentLayout = &m_age2Layout;
    }
    else if (currentAge == 3) {
        currentLayout = &m_age3Layout;
    }

    if (!currentLayout) {
        qDebug() << "Error: No layout found for age" << currentAge;
        return;
    }

    const QSize containerSize = ui->cardContainer->size();
    if (!containerSize.isValid() || containerSize.height() < 50) return;

    const int availableHeight = containerSize.height();
    const int pyramidTotalHeight = availableHeight * 0.9;
    const int cardH = pyramidTotalHeight / 3;
    const int cardW = cardH / 1.5;

    for (auto it = pyramid.rbegin(); it != pyramid.rend(); ++it) {
        const CardNode& node = *it;
        if (node.m_isRemoved) {
            continue;
        }

        QString imagePath;
        if (node.m_isFaceUp) {
            auto cardViewOpt = gameState.getCardView(node.m_index);
            if (!cardViewOpt) continue;
            const Card& card = cardViewOpt->get();

            // fix image path
            QString rawPath = QString::fromStdString(card.getImagePath());

            if (!rawPath.startsWith(":/")) {
                imagePath = QString(":/assets/age%1/%2")
                    .arg(card.getAge())
                    .arg(rawPath);
            }
            else {
                imagePath = rawPath;
            }
        }
        else {
            imagePath = QString(":/assets/age%1/age%1_back.png").arg(currentAge);
        }

        QPixmap cardPixmap(imagePath);

        if (cardPixmap.isNull()) {
            qDebug() << "Failed to load image:" << imagePath; // de debug, imagine gri
            cardPixmap = QPixmap(cardW, cardH);
            cardPixmap.fill(Qt::gray);
        }

        QPixmap finalPixmap(cardW, cardH);
        finalPixmap.fill(Qt::transparent);

        QPainter painter(&finalPixmap);
        painter.drawPixmap(0, 0, cardW, cardH, cardPixmap);

        if (node.m_isFaceUp) {
            auto cardViewOpt = gameState.getCardView(node.m_index);
            const Card& card = cardViewOpt->get();

            QFont font = painter.font();
            int fontSize = cardH / 16; 
            font.setPixelSize(fontSize);
            font.setBold(true);
            painter.setFont(font);

            painter.setPen(QPen(Qt::black));
            painter.drawText(QRect(2, cardH - (fontSize * 2.2), cardW - 4, fontSize * 2), Qt::AlignCenter | Qt::TextWordWrap, QString::fromStdString(card.getName()));
        }

        painter.end();

        QPushButton* cardButton = new QPushButton(ui->cardContainer);
        const CardPosition& pos = (*currentLayout)[node.m_index];

        cardButton->setGeometry(pos.x, pos.y, cardW, cardH);
        cardButton->setIcon(QIcon(finalPixmap)); 
        cardButton->setIconSize(QSize(cardW, cardH));
        cardButton->setText(""); 

        cardButton->setStyleSheet(
            "QPushButton { border: 1px solid rgba(0,0,0,150); background-color: transparent; border-radius: 5px; }"
            "QPushButton:hover { border: 2px solid yellow; }"
        );

        connect(cardButton, &QPushButton::clicked, [this, cardIndex = node.m_index]() {
            showActionDialog(cardIndex);
            });

        cardButton->show();
        m_cardButtons.push_back(cardButton);
    }
}

MainWindow::~MainWindow()
{
    delete ui;
}