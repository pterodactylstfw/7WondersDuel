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

    bool isDrafting = false;
    

    if (isDrafting) {
        drawDraftBoard(); 
    }
    else {
        setupLayouts();    
    }

    const auto& state = m_game.getGameState();

    updatePlayerArea(state.getCurrentPlayer(), ui->playerWonders, ui->playerCards);
    updatePlayerArea(state.getOpponent(), ui->opponentWonders, ui->opponentCards);
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
	setupLayouts();
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

        ui->stackedWidget->setCurrentIndex(1);
        QMessageBox::information(this, "Success", "Game loaded successfully!");

		setupLayouts();
		updateGameUI();
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

    // MINUNI
    // 
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
        btn->setFixedSize(100, 60); 
        btn->setText(QString::fromStdString(wonder->getName()));

		// stil diferit daca e construita sau nu
        if (wonder->isBuilt()) {
            btn->setStyleSheet("background-color: #ffd700; border: 2px solid green; color: black; font-weight: bold;");
            btn->setToolTip("BUILT: " + QString::fromStdString(wonder->getDescription()));
        }
        else {
            btn->setStyleSheet("background-color: #555; border: 1px dashed white; color: white;");
            btn->setToolTip("COST: " + QString::fromStdString(wonder->getCost().toString()) + "\nEFFECT: " + QString::fromStdString(wonder->getDescription()));
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
            QWidget* col = createColorColumn(cards);
            cityArea->layout()->addWidget(col);
        }
    }
}

QWidget* MainWindow::createColorColumn(const std::vector<std::reference_wrapper<const Card>>& cards)
{
    QWidget* columnWidget = new QWidget();
    QVBoxLayout* layout = new QVBoxLayout(columnWidget);

    // suprapunere
    layout->setSpacing(-80);
    layout->setContentsMargins(0, 0, 0, 0);
    layout->setAlignment(Qt::AlignTop);

    for (const auto& cardRef : cards) {
        const Card& card = cardRef.get();
        QPushButton* btn = new QPushButton(columnWidget);
        btn->setFixedSize(70, 100); 

        QString imgPath = QString::fromStdString(card.getImagePath());
        QPixmap pix(imgPath);
        if (!pix.isNull()) {
            btn->setIcon(QIcon(pix));
            btn->setIconSize(QSize(70, 100));
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

    // 2. Obținem minunile disponibile pentru draft
    // ATENȚIE: Aici presupunem că ai o metodă în GameController care returnează cele 4 minuni curente
    // Deocamdată, ca să testăm vizual, vom "inventa" logica de afișare.
    // În realitate, vei lua vectorul din m_game.getGameState().getDraftPool() (trebuie implementat în backend)

    // EXEMPLU VIZUAL: Să zicem că avem 4 minuni fictive sau le luăm din factory temporar
    // (În pasul următor vom lega asta de backend-ul real)

    // Calculăm dimensiunile pentru 4 minuni mari
    int containerW = ui->cardContainer->width();
    int containerH = ui->cardContainer->height();

    int wonderW = 150; // Mai late decât cărțile normale
    int wonderH = 100;
    int spacing = 20;

    // Calculăm poziția de start ca să fie centrate
    int totalWidth = (4 * wonderW) + (3 * spacing);
    int startX = (containerW - totalWidth) / 2;
    int startY = (containerH - wonderH) / 2; // Centrat vertical

    // Desenăm cele 4 sloturi
    // (Aici ar trebui să iterezi prin vectorul de minuni disponibile)
    for (int i = 0; i < 4; ++i) {
        // Dacă minunea a fost deja luată, sărim peste ea (nu o desenăm)
        // if (availableWonders[i] == nullptr) continue; 

        QPushButton* btn = new QPushButton(ui->cardContainer);
        int x = startX + i * (wonderW + spacing);
        int y = startY;

        btn->setGeometry(x, y, wonderW, wonderH);

        // Text temporar până legăm imaginile
        btn->setText("Wonder " + QString::number(i + 1));
        btn->setStyleSheet("background-color: #FF9800; border: 2px solid white; color: white; font-weight: bold; font-size: 14px;");

        // Conectăm click-ul
        connect(btn, &QPushButton::clicked, [this, i]() {
            // Aici vom apela funcția din controller: m_game.pickWonder(i);
            qDebug() << "Player selected Wonder index: " << i;
            });

        btn->show();
        m_cardButtons.push_back(btn); // Le salvăm ca să le putem șterge la refresh
    }

    // Adăugăm și un titlu sus ca să știm cine alege
    // Putem folosi un QLabel existent sau creăm unul temporar
}

void MainWindow::resizeEvent(QResizeEvent* event)
{
    QMainWindow::resizeEvent(event); 

    setupLayouts();
    if (ui->stackedWidget->currentIndex() == 1) 
    {
        updateGameUI();
    }
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
	updateCardStructures();
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
    if (!containerSize.isValid()) return; 

    const int availableWidth = containerSize.width();
    const int availableHeight = containerSize.height();

    const int pyramidTotalHeight = availableHeight * 0.9;
    const int cardH = pyramidTotalHeight / 3; 
    const int cardW = cardH / 1.5;

    const int horizontalGap = 5;
    const int verticalOverlap = cardH / 2;
    const int stepX = cardW + horizontalGap;

    const int totalWidthOfBottomRow = 6 * cardW + 5 * horizontalGap;
    const int totalHeightOfPyramid = cardH + 4 * verticalOverlap;

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

	// age 3 layout
}

void MainWindow::updateCardStructures()
{
    for (QPushButton* button : m_cardButtons) {
		delete button;
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
            imagePath = QString::fromStdString(cardViewOpt->get().getImagePath());
        }
        else {
            imagePath = QString(":/assets/age%1/age%1_back.png").arg(currentAge);
        }

        QPixmap cardPixmap(imagePath);
        if (cardPixmap.isNull()) {
            qDebug() << "Failed to load image:" << imagePath;
            continue;
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