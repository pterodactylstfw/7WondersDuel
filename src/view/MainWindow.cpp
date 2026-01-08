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
#include "AIController.h"
#include <QThread>

MainWindow::MainWindow(QWidget* parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
    , m_game(*this)
    , m_age1Layout{}
    , m_age2Layout{}
    , m_age3Layout{}
{
    this->setWindowIcon(QIcon(":/assets/coins.png"));
    ui->setupUi(this);

    ui->labelMilitaryBoard->setPixmap(QPixmap(":/assets/military_board.png"));
    ui->labelMilitaryLead->setPixmap(QPixmap(":/assets/military_lead.png"));

    ui->verticalLayout_6->removeWidget(ui->labelMilitaryLead);
    ui->labelMilitaryLead->setParent(ui->militaryPanel);
    ui->labelMilitaryLead->raise();
    ui->labelMilitaryLead->show();

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

    m_btnHint = new QPushButton("HINT", this);
    m_btnHint->setStyleSheet(
        "QPushButton {"
        "  background-color: #2E7D32;"
        "  color: white;"
        "  font-weight: bold;"
        "  border-radius: 5px;"
        "  border: 1px solid #1B5E20;"
        "  font-size: 13px;"
        "}"
        "QPushButton:hover { background-color: #388E3C; }"
        "QPushButton:pressed { background-color: #1B5E20; }"
    );

    connect(m_btnHint, &QPushButton::clicked, this, &MainWindow::onBtnHintClicked);
    m_btnHint->show();
    this->setWindowTitle("7 Wonders Duel");
}

void MainWindow::onMessage(const std::string& message) {
    qDebug() << "Message:" << QString::fromStdString(message);
    QString text = QString::fromStdString(message);
    showFloatingText(text, "color: #ffffff; font-weight: bold; font-size: 16px;");
}

void MainWindow::onError(const std::string& error) {
    qDebug() << "Error:" << QString::fromStdString(error);
}

void MainWindow::onStateUpdated() {
    if (!m_game.hasGameStarted()) return;

    updateGameUI();
    drawProgressTokens();

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
    QStringList modes;
    modes << "Player vs Player" << "Player vs AI";

    bool okMode;
    QString mode = QInputDialog::getItem(this, "Game Mode",
        "Select Game Mode:",
        modes, 0, false, &okMode);

    if (!okMode || mode.isEmpty()) return;

    bool isVsAI = (mode == "Player vs AI");

    if (isVsAI) {
        QStringList levels;
        levels << "Easy" << "Hard";
        bool okDiff;
        QString level = QInputDialog::getItem(this, "Difficulty",
            "Select AI Difficulty:",
            levels, 1, false, &okDiff);
        if (!okDiff) return;

        if (level == "Easy")
            m_aiDifficulty = AIDifficulty::EASY;
        else
            m_aiDifficulty = AIDifficulty::HARD;
    }

    bool ok1;
    QString player1Name = QInputDialog::getText(this, "Player 1",
        "Enter name for Player 1:",
        QLineEdit::Normal, "Player 1", &ok1);
    if (!ok1 || player1Name.isEmpty()) return;

    QString player2Name;
    if (isVsAI) {
        QString diffLabel = (m_aiDifficulty == AIDifficulty::EASY) ? "Easy" : "Hard";
        player2Name = "AI (" + diffLabel + ")";
    }
    else {
        bool ok2;
        player2Name = QInputDialog::getText(this, "Player 2",
            "Enter name for Player 2:",
            QLineEdit::Normal, "Player 2", &ok2);
        if (!ok2 || player2Name.isEmpty()) return;
    }
    ui->stackedWidget->setCurrentIndex(1);
    qDebug() << "Switched to game view";

    updateGameUI();
    QApplication::processEvents();
    setupLayouts();
    m_game.startNewGame(player1Name.toStdString(), player2Name.toStdString());

    if (isVsAI) {
        const_cast<GameState&>(m_game.getGameState()).getPlayers()[1]->setAI(true);
    }
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

void MainWindow::onBtnHintClicked() {
    if (!m_game.hasGameStarted() || m_game.isGameOver()) return;
    if (m_game.getGameState().getCurrentPlayer().isAI()) {
        showFloatingText("Wait for your turn!", "color: red; font-size: 20px;");
    }
    AIController hintAI(AIDifficulty::HARD);
    AIMove bestMove = hintAI.decideMove(m_game.getGameState());

    if (bestMove.cardIndex != -1) {
        highlightCardUI(bestMove.cardIndex);
        QString actionText;
        switch (bestMove.action) {
            case PlayerAction::CONSTRUCT_BUILDING:actionText = "Build this!"; break;
            case PlayerAction::DISCARD_FOR_COINS:actionText = "Sell this!"; break;
            case PlayerAction::CONSTRUCT_WONDER:actionText = "Build Wonder!"; break;
        }
        showFloatingText("Hint: " + actionText, "color: cyan; font-weight: bold; font-size: 24px;");
    }
}

void MainWindow::updatePlayerArea(const Player& player, QWidget* wondersArea, QWidget* cityArea)
{
    if (!wondersArea || !cityArea) return;

    int availableH = cityArea->height();
    if (availableH < 50) availableH = this->height() * 0.2; 
    int maxH = availableH - 10;

    int wonderW = this->width() * 0.11;
    int wonderH = wonderW * 0.66;

    if (wonderH > maxH) {
        wonderH = maxH;
        wonderW = wonderH / 0.66; 
    }

    int cardW = this->width() * 0.04;
    int cardH = cardW * 1.5;

    if (cardH > maxH) {
        cardH = maxH;
        cardW = cardH / 1.5; 
    }

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
            QPixmap labeledPix = addTextToImage(pix, QString::fromStdString(wonder->getName()), wonderW, wonderH);

            btn->setIcon(QIcon(labeledPix));
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
        l->setSpacing(cardW * 0.1);
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

    int visibleHeader = height * 0.25;
    int currentY = 30; // top margin pentru banda maro

    for (const auto& cardRef : cards) {
        const Card& card = cardRef.get();
        QPushButton* btn = new QPushButton(columnWidget);

        btn->setGeometry(0, currentY, width, height);

        QString imgPath = QString::fromStdString(card.getImagePath());
        QPixmap pix(imgPath);
        if (!pix.isNull()) {
            QPixmap labeledPix = addTextToImage(pix, QString::fromStdString(card.getName()), width, height);

            btn->setIcon(QIcon(labeledPix));
            btn->setIconSize(QSize(width, height));
            btn->setStyleSheet("border: none; background: transparent;");
        }
        else {
            btn->setText(QString::fromStdString(card.getName()));
            btn->setStyleSheet("background-color: gray; color: white; border: 1px solid black;");
        }

        btn->setToolTip(QString::fromStdString(card.displayCardInfo()));
		btn->show();
        btn->raise(); 
        currentY += visibleHeader;
    }

    int totalHeight = currentY + ((cards.size() - 1) * visibleHeader) + height;
    columnWidget->setFixedSize(width, totalHeight);
    return columnWidget;
}

QPixmap MainWindow::addTextToImage(const QPixmap& baseImage, const QString& text, int width, int height)
{
	if (baseImage.isNull()) return QPixmap();

	QPixmap textPixmap(width, height);
	textPixmap.fill(Qt::transparent);

	QPainter painter(&textPixmap);
	painter.drawPixmap(0, 0, baseImage.scaled(width, height, Qt::KeepAspectRatio, Qt::SmoothTransformation));

	QFont font = painter.font();

    int textAreaHeight = height * 0.20;
    int fontSize = textAreaHeight / 3;
    font.setPixelSize(fontSize);
	font.setBold(true);

	painter.setFont(font);
	painter.setPen(Qt::black);

    int textHeight = fontSize * 2.5;
	QRect textRect(2, height - textAreaHeight, width - 4, textAreaHeight);

    painter.drawText(textRect, Qt::AlignCenter | Qt::TextWordWrap, text);

    painter.end();
    return textPixmap;
}

void MainWindow::showFloatingText(const QString& text, const QString& colorStyle)
{
    QList<QLabel*> existingLabels = this->findChildren<QLabel*>("hintLabel");
    qDeleteAll(existingLabels);

    QLabel* label = new QLabel(this);
    label->setObjectName("hintLabel");
    label->setText(text);
    label->setStyleSheet(colorStyle + "background-color: rgba(0,0,0,180); padding: 5px; border-radius: 5px;");
    label->adjustSize();

    int x, y;
    if (m_btnHint && m_btnHint->isVisible()) {
        int spacing = 15;
        x = m_btnHint->x() + m_btnHint->width() + spacing;
        y = m_btnHint->y() + (m_btnHint->height() - label->height()) / 2;
    }
    else {
        x = (this->width() - label->width()) / 2;
        y = (this->height() - label->height()) / 2;
    }

    label->move(x, y);
    label->show();
    label->raise();

    QTimer::singleShot(3500, label, &QLabel::deleteLater);
}
void MainWindow::showActionDialog(int cardIndex)
{
    auto cardView = m_game.getGameState().getCardView(cardIndex);
    if (!cardView.has_value()) return;
    const Card& card = cardView->get();
    bool actionTaken = false;

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
            actionTaken = true;
            dialog.accept(); 
        }
        else {
            dialog.reject(); 
        }
        });

	// DISCARD FOR COINS
    connect(btnDiscard, &QPushButton::clicked, [&]() {
        bool success = m_game.executeAction(cardIndex, PlayerAction::DISCARD_FOR_COINS);
        if (success) {
            dialog.accept();
            actionTaken = true;
        }
        else
            dialog.reject();
        });

	// CONSTRUCT WONDER
    /*
    connect(btnWonder, &QPushButton::clicked, [&]() {
        std::string currentPlayerName = m_game.getGameState().getCurrentPlayer().getName();
        int wonderIndex = selectWonderIndex(currentPlayerName);

        if (wonderIndex != -1) { 
            bool success = m_game.executeAction(cardIndex, PlayerAction::CONSTRUCT_WONDER, wonderIndex);
            if (success) {
                actionTaken = true;
                dialog.accept();
            }
        }
        });
        */

    // afisam fereastra
    dialog.exec();

    if (actionTaken) {
        onStateUpdated(); 
        QApplication::processEvents();
        while (!m_game.isGameOver() && m_game.getGameState().getCurrentPlayer().isAI())
        {
            QThread::msleep(1000);
            AIController ai(m_aiDifficulty);
            AIMove move = ai.decideMove(m_game.getGameState());
            bool aiSuccess = m_game.executeAction(move.cardIndex, move.action, move.wonderIndex);

            if (!aiSuccess) {
                qDebug() << "AI Move failed, forcing discard";
                const auto& pyramid = m_game.getGameState().getPyramid();
                for (const auto& node : pyramid) {
                    if (!node.m_isRemoved && m_game.getGameState().isCardAccessible(node.m_index)) {
                        m_game.executeAction(node.m_index, PlayerAction::DISCARD_FOR_COINS);
                        break;
                    }
                }
            }
            onStateUpdated();
            QApplication::processEvents();
        }
    }
}

void MainWindow::drawProgressTokens()
{
    for (QPushButton* btn : m_progressTokenButtons) {
        btn->deleteLater();
    }
    m_progressTokenButtons.clear();

	const auto& tokens = m_game.getGameState().getAvailableTokens();

    if (tokens.empty() || !ui->labelMilitaryBoard->isVisible()) return;

    int boardW = ui->labelMilitaryBoard->width(); 
    int boardH = ui->labelMilitaryBoard->height();

    int tokenSize = boardW * 0.28;
    int xPos = boardW * 0.67;

    std::vector<double> yRatios = { 0.25, 0.35, 0.46, 0.56, 0.67 };

    for (size_t i = 0; i < tokens.size() && i < yRatios.size(); ++i) {
        const auto& token = tokens[i];

        QPushButton* btn = new QPushButton(ui->labelMilitaryBoard);
        int yPos = boardH * yRatios[i];
        btn->setGeometry(xPos, yPos, tokenSize, tokenSize);

        QString imgPath = QString::fromStdString(token->getImagePath());
        QPixmap pix(imgPath);

        if (!pix.isNull()) {
            btn->setIcon(QIcon(pix));
            btn->setIconSize(QSize(tokenSize, tokenSize));
            btn->setStyleSheet(
                "QPushButton { "
                "  border: none; "
                "  background-color: transparent; "
                "  border-radius: " + QString::number(tokenSize / 2) + "px;"
                "}"
                "QPushButton:hover { border: 2px solid white; }"
            );
        }
        else {
            btn->setText(QString::fromStdString(token->getName().substr(0, 2)));
            btn->setStyleSheet("background-color: green; color: white; border-radius: 10px; font-size: 8px;");
        }

        btn->setToolTip(QString::fromStdString(token->getName() + ":\n" + token->getDescription()));

        btn->show();
        m_progressTokenButtons.push_back(btn);
    }
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
            QPixmap labeledPix = addTextToImage(pix, QString::fromStdString(wonder->getName()), wonderW, wonderH);

            btn->setIcon(QIcon(labeledPix));
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
            
            if (success) {
                onStateUpdated();
                QApplication::processEvents();
                while (!m_game.isGameOver() &&
                    m_game.getGameState().getCurrentPhase() == GamePhase::DRAFTING &&
                    m_game.getGameState().getCurrentPlayer().isAI())
                {
                    QThread::msleep(1000);
                    AIController ai(m_aiDifficulty);
                    int bestIndex = ai.pickWonder(m_game.getGameState());
                    bool aiSuccess = m_game.pickWonder(bestIndex);
                    if (!aiSuccess) {
                        m_game.pickWonder(0);
                    }
                    onStateUpdated();
                    QApplication::processEvents();
                }
            }
            else {
                QMessageBox::warning(this, "Warning", "You can't choose this Wonder now.");
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

void MainWindow::highlightCardUI(int cardIndex){
    for (QPushButton* btn : m_cardButtons) {
        int id = btn->property("myCardIndex").toInt();
        if (id == cardIndex) {
            btn->setStyleSheet(
                "QPushButton { "
                "  border: 8px solid #32CD32; "
                "  border-radius: 10px; "
                "  background-color: rgba(50, 205, 50, 150); "
                "}"
            );
            btn->raise();
        }
        else {
            btn->setStyleSheet(
                "QPushButton { border: 1px solid rgba(0,0,0,150); background-color: transparent; border-radius: 5px; }"
                "QPushButton:hover { border: 2px solid yellow; }"
            );
        }
    }
}

void MainWindow::resizeEvent(QResizeEvent* event)
{
    QMainWindow::resizeEvent(event); 

    setupLayouts();
    if (m_btnHint) {
        int btnW = 90;
        int btnH = 35;
        int marginX = 30;
        int marginY = 40;

        int xPos = marginX;
        int yPos = this->height() - btnH - marginY;

        m_btnHint->setGeometry(xPos, yPos, btnW, btnH);
        m_btnHint->raise();
    }
    if (ui->stackedWidget->currentIndex() == 1 && m_game.hasGameStarted()) 
    {
        updateMilitaryTrack(); 
        drawProgressTokens();
    }
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

    int panelW = panel->width();
    int panelH = panel->height();

    int pawnW = panelW * 0.4;
    int pawnH = pawnW / 2;
	ui->labelMilitaryLead->resize(pawnW, pawnH);

	int leadX = (panelW - pawnW) / 2 - panelW * 0.15;

    double percentage = (militaryPosition + 9.0) / 18.0;
    int availableHeight = panelH * 0.90;
    int topMargin = panelH * 0.05;

    int centerY = topMargin + (availableHeight * (1.0 - percentage));

    int leadY = centerY - (pawnH / 2);

    ui->labelMilitaryLead->move(leadX, leadY);
	ui->labelMilitaryLead->raise();
}

void MainWindow::setupLayouts()
{
    const QSize containerSize = ui->cardContainer->size();
    qDebug() << "SetupLayouts Container Size:" << containerSize;

    if (!containerSize.isValid() || containerSize.width() <= 0) return;

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

        QPixmap finalPixmap;

        if (node.m_isFaceUp) {
            auto cardViewOpt = gameState.getCardView(node.m_index);
            const Card& card = cardViewOpt->get();

            finalPixmap = addTextToImage(cardPixmap, QString::fromStdString(card.getName()), cardW, cardH);
        }
        else{
            finalPixmap = cardPixmap.scaled(cardW, cardH, Qt::KeepAspectRatio, Qt::SmoothTransformation);
        }

        QPushButton* cardButton = new QPushButton(ui->cardContainer);
        const CardPosition& pos = (*currentLayout)[node.m_index];

        cardButton->setProperty("myCardIndex", node.m_index);
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