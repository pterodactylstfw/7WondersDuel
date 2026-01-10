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
#include <QNetworkInterface>

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

	m_netClient = new NetworkClient(this);

	ui->labelMilitaryBoard->setPixmap(QPixmap(":/assets/military_board.png"));
	ui->labelMilitaryLead->setPixmap(QPixmap(":/assets/military_lead.png"));

	ui->verticalLayout_6->removeWidget(ui->labelMilitaryLead);
	ui->labelMilitaryLead->setParent(ui->militaryPanel);
	ui->labelMilitaryLead->raise();
	ui->labelMilitaryLead->show();

	ui->stackedWidget->setCurrentIndex(0);

	connect(ui->btnStart, &QPushButton::clicked, this, &MainWindow::onBtnStartClicked);

	connect(m_netClient, &NetworkClient::identityReceived, this, [this](int index, const QString& name) {
		this->m_myIndex = index;
		QMessageBox::information(this, "Game Started",
			"You are " + name + "!\nThe game will begin shortly.");

		this->setWindowTitle("7 Wonders Duel - " + name);

		ui->stackedWidget->setCurrentIndex(1);

		ui->stackedWidget->update();
		ui->cardContainer->update();
		QTimer::singleShot(100, this, [this]() {
			setupLayouts();     // Calculam coordonatele pe baza marimii actuale
			updateGameUI();     // Desenam cartile si minunile
			});

		//QApplication::processEvents();
		ui->actionSave->setEnabled(true);
		});

	connect(m_netClient, &NetworkClient::stateReceived, this, [this](const GameState& newState) {

		json j;
		to_json(j, newState);

		// Init pointerul daca e null
		if (!m_networkState) {
			m_networkState = std::make_unique<GameState>();
		}

		from_json(j, *m_networkState);

		onStateUpdated();
		});

	// Cand primim eroare (de ex. "Opponent disconnected")
	connect(m_netClient, &NetworkClient::errorOccurred, this, [this](const QString& msg) {
		QMessageBox::critical(this, "Connection Error", msg);

		// Ne intoarcem la meniu
		cleanupVisuals();
		ui->stackedWidget->setCurrentIndex(0);
		ui->btnStart->setEnabled(true);
		m_isOnlineMode = false;

		// Resetam conexiunea
		m_netClient->disconnect();
		});

	// Cand serverul pica de tot
	connect(m_netClient, &NetworkClient::disconnected, this, [this]() {
		if (m_isOnlineMode) {
			QMessageBox::warning(this, "Disconnected", "Lost connection to server.");
			cleanupVisuals();
			ui->stackedWidget->setCurrentIndex(0);
			ui->btnStart->setEnabled(true);
			m_isOnlineMode = false;
		}
		});

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
	m_btnHint->hide();

	connect(ui->actShowIP, &QAction::triggered, this, [this]() {
		QString ipAddress;
		const QHostAddress& localhost = QHostAddress(QHostAddress::LocalHost);
		for (const QHostAddress& address : QNetworkInterface::allAddresses()) {
			if (address.protocol() == QAbstractSocket::IPv4Protocol && address != localhost) {
				ipAddress = address.toString();
			}
		}

		// Lista completa (mai sigur)
		/*QString allIPs;
		for (const QHostAddress& address : QNetworkInterface::allAddresses()) {
			if (address.protocol() == QAbstractSocket::IPv4Protocol && address != localhost)
				allIPs += address.toString() + "\n";
		}*/

		QMessageBox::information(this, "My IP Address",
			"Share this IP with Player 2:\n\n" + (ipAddress.isEmpty() ? "No network found" : ipAddress));
		});

	connect(ui->actForceWin, &QAction::triggered, this, [this]() {
		if (!m_isOnlineMode && m_game.hasGameStarted()) {
			m_game.debugTriggerVictory();
		}
		else {
			QMessageBox::warning(this, "Cheat", "Cheats disabled in Online Mode or Menu.");
		}
		});

	connect(ui->actionAbout, &QAction::triggered, this, [this]() {
		QMessageBox::about(this, "About 7 Wonders Duel",
			"<h3>7 Wonders Duel - C++ Project</h3>"
			"<p><b>Version:</b> 1.0 (Release)</p>"
			"<p>Built with <b>Qt 6</b> and <b>Modern C++</b>.</p>"
			"<hr>"
			"<p><b>Developers:</b></p>"
			"<ul>"
			"<li>Rebeka</li>"
			"<li>Cristian</li>"
			"<li>Raul</li>"
			"<li>Denisa</li>"
			"</ul>"
			"<p><i>Project for Modern C++ Course 2025.</i></p>"
		);
		});

	this->setWindowTitle("7 Wonders Duel");
}

void MainWindow::onMessage(const std::string& message) {
	if (m_game.isGameOver() || (m_networkState && m_networkState->isGameOver())) {
		return;
	}
	qDebug() << "Message:" << QString::fromStdString(message);
	QString text = QString::fromStdString(message);
	showFloatingText(text, "color: #ffffff; font-weight: bold; font-size: 16px;");
}

void MainWindow::onError(const std::string& error) {
	qDebug() << "Error:" << QString::fromStdString(error);
}

void MainWindow::onStateUpdated() {
	if (m_isOnlineMode) {
		if (!m_networkState) return;
	}
	else {
		if (!m_game.hasGameStarted()) {
			if (m_btnHint) m_btnHint->hide();
			return;
		}
	}

	const auto& state = getCurrentGameState();

	if (state.isGameOver()) {
		if (m_btnHint) m_btnHint->hide();
		showGameOverScreen();
		return;
	}

	if (m_isOnlineMode) {
		if (state.getCurrentPlayerIndex() == m_myIndex) {
			ui->cardContainer->setEnabled(true);
			ui->label->setText("YOUR TURN!");
		}
		else {
			ui->cardContainer->setEnabled(false);
			ui->label->setText("Opponent's Turn...");
		}
	}

	updateGameUI();
	drawProgressTokens();

	updatePlayerArea(state.getCurrentPlayer(), ui->playerWonders, ui->playerCards);
	updatePlayerArea(state.getOpponent(), ui->opponentWonders, ui->opponentCards);

	if (state.getCurrentPhase() == GamePhase::DRAFTING) {
		drawDraftBoard();
		if (m_btnHint) m_btnHint->hide();
	}
	else {
		updateCardStructures();
		this->setWindowTitle("7 Wonders Duel - Age " + QString::number(state.getCurrentAge()));

		if (m_btnHint) {
			bool showHint = false;
			if (m_isOnlineMode) {
				if (state.getCurrentPlayerIndex() == m_myIndex) showHint = true;
			}
			else {
				if (!state.getCurrentPlayer().isAI()) showHint = true;
			}

			if (showHint) m_btnHint->show();
			else m_btnHint->hide();
		}
	}
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

int MainWindow::askWonderSelection(const std::array<std::unique_ptr<Wonder>, GameConstants::WONDERS_PER_PLAYER>& wonders, const std::string& playerName) {
	if (m_myWonderButtons.empty()) return -1;

	QEventLoop loop;
	int selectedIndex = -1;
	std::vector<QMetaObject::Connection> connections;


	QString originalText = ui->label->text();
	ui->label->setText("SELECT A WONDER TO BUILD (Click on the image)");
	ui->label->setStyleSheet("color: orange; font-weight: bold;");

	for (size_t i = 0; i < m_myWonderButtons.size(); ++i) {
		QPushButton* btn = m_myWonderButtons[i];

		if (btn && wonders[i] && !wonders[i]->isBuilt()) {

			btn->setStyleSheet("border: 3px solid #00FF00; border-radius: 5px;");
			btn->setCursor(Qt::PointingHandCursor);

			auto conn = connect(btn, &QPushButton::clicked, [&, i]() {
				selectedIndex = static_cast<int>(i);
				loop.quit();
				});
			connections.push_back(conn);
		}
		else if (btn) {
			btn->setDisabled(true);
			btn->setStyleSheet("opacity: 0.3; background-color: rgba(0,0,0,100);");
		}
	}

	loop.exec();

	for (auto conn : connections) disconnect(conn);

	ui->label->setText(originalText);
	ui->label->setStyleSheet("");

	updatePlayerArea(getCurrentGameState().getCurrentPlayer(), ui->playerWonders, ui->playerCards);

	return selectedIndex;
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
	modes << "Player vs Player (Local)" << "Player vs AI" << "Online Multiplayer"; // <--- Optiune noua

	bool okMode;
	QString mode = QInputDialog::getItem(this, "Game Mode",
		"Select Game Mode:",
		modes, 0, false, &okMode);

	if (!okMode || mode.isEmpty()) return;

	if (mode == "Online Multiplayer") {
		m_isOnlineMode = true;

		bool ok;
		QString ipAddress = QInputDialog::getText(this, "Connect to Server",
			"Enter Server IP Address:\n(Use '127.0.0.1' if server is on this PC)",
			QLineEdit::Normal, "127.0.0.1", &ok);

		if (!ok || ipAddress.isEmpty()) return;

		// Incercam conectarea
		m_netClient->connectToServer(ipAddress, 12345);

		ui->label->setText("Connecting to " + ipAddress + "...");

		ui->btnStart->setEnabled(false);
		return;
	}

	m_isOnlineMode = false; // Ne asiguram ca e false

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

	setupLayouts();

	m_game.startNewGame(player1Name.toStdString(), player2Name.toStdString());

	if (isVsAI) {
		const_cast<GameState&>(getCurrentGameState()).getPlayers()[1]->setAI(true);
	}

	ui->actionSave->setEnabled(true);
	ui->actionLoad->setVisible(true);

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


const GameState& MainWindow::getCurrentGameState() const {
	if (m_isOnlineMode) {
		if (m_networkState)
			return *m_networkState;
		return m_game.getGameState();
	}
	else
		return m_game.getGameState();
}

void MainWindow::onBtnHintClicked() {
	const auto& state = getCurrentGameState();

	if (m_isOnlineMode) {
		if (!m_networkState) return;
	}
	else {
		if (!m_game.hasGameStarted()) return;
	}

	if (state.isGameOver()) return;

	if (m_isOnlineMode) {
		if (state.getCurrentPlayerIndex() != m_myIndex) {
			showFloatingText("Wait for your turn!", "color: red; font-size: 20px;");
			return;
		}
	}
	else {
		if (state.getCurrentPlayer().isAI()) {
			showFloatingText("Wait for AI turn!", "color: red; font-size: 20px;");
			return;
		}
	}

	AIController hintAI(AIDifficulty::HARD);
	AIMove bestMove = hintAI.decideMove(state);

	if (bestMove.cardIndex != -1) {
		highlightCardUI(bestMove.cardIndex);
		QString actionText;
		switch (bestMove.action) {
		case PlayerAction::CONSTRUCT_BUILDING:actionText = "Build this!"; break;
		case PlayerAction::DISCARD_FOR_COINS:actionText = "Sell this!"; break;
		case PlayerAction::CONSTRUCT_WONDER:actionText = "Build Wonder!"; break;
		}
		showHintText(actionText);
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
	if (wonderH > maxH) { wonderH = maxH; wonderW = wonderH / 0.66; }

	int cardW = this->width() * 0.04;
	int cardH = cardW * 1.5;
	if (cardH > maxH) { cardH = maxH; cardW = cardH / 1.5; }

	bool isCurrentPlayer = (wondersArea == ui->playerWonders);
	if (isCurrentPlayer) m_myWonderButtons.clear();

	fillWondersArea(player, wondersArea, wonderW, wonderH, isCurrentPlayer);

	qDeleteAll(cityArea->findChildren<QWidget*>(QString(), Qt::FindDirectChildrenOnly));

	if (!cityArea->layout()) {
		QHBoxLayout* l = new QHBoxLayout(cityArea);
		l->setSpacing(cardW * 0.1);
		l->setContentsMargins(0, 0, 0, 0);
	}

	std::vector<CardColor> colors = {
		CardColor::BROWN, CardColor::GREY, CardColor::BLUE,
		CardColor::GREEN, CardColor::YELLOW, CardColor::RED, CardColor::PURPLE
	};

	for (const auto& color : colors) {
		auto cards = player.getCardsOfType(color);
		if (!cards.empty()) {
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

void MainWindow::fillWondersArea(const Player& player, QWidget* container, int width, int height, bool isInteractive)
{
	qDeleteAll(container->findChildren<QWidget*>(QString(), Qt::FindDirectChildrenOnly));

	if (!container->layout()) {
		QHBoxLayout* l = new QHBoxLayout(container);
		l->setSpacing(10);
		l->setContentsMargins(0, 0, 0, 0);
	}

	const auto& wonders = player.getWonders();
	for (size_t i = 0; i < wonders.size(); ++i) {
		const auto& wonder = wonders[i];

		if (!wonder) {
			if (isInteractive) m_myWonderButtons.push_back(nullptr);
			continue;
		}

		QPushButton* btn = new QPushButton(container);
		btn->setFixedSize(width, height);

		QString imgPath = QString::fromStdString(wonder->getImagePath());
		QPixmap pix(imgPath);

		if (!pix.isNull()) {
			QPixmap labeledPix = addTextToImage(pix, QString::fromStdString(wonder->getName()), width, height);
			btn->setIcon(QIcon(labeledPix));
			btn->setIconSize(QSize(width - 6, height - 6));
		}
		else {
			btn->setText(QString::fromStdString(wonder->getName()));
		}

		if (wonder->isBuilt()) {
			btn->setStyleSheet(
				"QPushButton { "
				"  border: 4px solid #FFD700; "      // Gold Border
				"  background-color: black; "
				"  border-radius: 6px; "
				"}"
				"QPushButton:hover { border: 4px solid white; }" // Hover effect
			);
			btn->setToolTip("BUILT: " + QString::fromStdString(wonder->getDescription()));
		}
		else {
			btn->setStyleSheet(
				"QPushButton { "
				"  border: 2px dashed gray; "
				"  background-color: rgba(0, 0, 0, 50); "
				"  border-radius: 6px; "
				"}"
				"QPushButton:hover { border: 2px solid orange; }"
			);
			btn->setToolTip("COST: " + QString::fromStdString(wonder->getCost().toString()) +
				"\nEFFECT: " + QString::fromStdString(wonder->getDescription()));
		}
		container->layout()->addWidget(btn);

		if (isInteractive) {
			m_myWonderButtons.push_back(btn);
		}
	}

	const auto& builtWonders = player.getConstructedWonders();
	for (const auto& wonder : builtWonders) {
		if (!wonder) continue;

		QPushButton* btn = new QPushButton(container);
		btn->setFixedSize(width, height);

		QString imgPath = QString::fromStdString(wonder->getImagePath());
		QPixmap pix(imgPath);

		if (!pix.isNull()) {
			QPixmap labeledPix = addTextToImage(pix, QString::fromStdString(wonder->getName()), width, height);
			btn->setIcon(QIcon(labeledPix));
			btn->setIconSize(QSize(width - 6, height - 6));
		}
		else {
			btn->setText(QString::fromStdString(wonder->getName()));
		}

		btn->setStyleSheet(
			"QPushButton { border: 4px solid #FFD700; background-color: black; border-radius: 6px; }"
			"QPushButton:hover { border: 4px solid white; }"
		);
		btn->setToolTip("BUILT: " + QString::fromStdString(wonder->getDescription()));

		container->layout()->addWidget(btn);

	}
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

QWidget* MainWindow::createPlayerEndGameWidget(const Player& player, int totalScore, bool isWinner)
{
	QFrame* frame = new QFrame();
	frame->setStyleSheet("QFrame { background-color: rgba(0, 0, 0, 150); border-radius: 10px; border: 1px solid white; }");

	QVBoxLayout* mainLayout = new QVBoxLayout(frame);
	mainLayout->setSpacing(10);
	mainLayout->setContentsMargins(15, 15, 15, 15);

	QHBoxLayout* headerLayout = new QHBoxLayout();
	headerLayout->setSpacing(15);

	QLabel* nameLbl = new QLabel(QString::fromStdString(player.getName()));
	nameLbl->setStyleSheet("font-size: 24px; font-weight: bold; color: white; border: none; background: transparent;");
	headerLayout->addWidget(nameLbl);

	const QString styleTemplate =
		"background-image: url(%1);"
		"background-repeat: no-repeat;"
		"background-position: center;"
		"border-radius: 20px;"
		"border: none;"
		"color: white;"
		"font-size: 16px;"
		"font-weight: bold;"
		"qproperty-alignment: 'AlignCenter';";

	QLabel* playerCoins = new QLabel();
	playerCoins->setStyleSheet(styleTemplate.arg(":/assets/coins.png"));
	playerCoins->setText(QString::number(player.getCoins()));
	headerLayout->addWidget(playerCoins);

	QLabel* playerVP = new QLabel();
	playerVP->setStyleSheet(styleTemplate.arg(":/assets/victorypoints.png"));
	playerVP->setText(QString::number(totalScore));
	headerLayout->addWidget(playerVP);

	const auto& tokens = player.getProgressTokens();
	if (!tokens.empty()) {
		for (const auto& token : tokens) {
			QLabel* tokenLbl = new QLabel();
			tokenLbl->setFixedSize(40, 40);
			tokenLbl->setStyleSheet("border: none; background: transparent;");

			QPixmap tokenPix(QString::fromStdString(token->getImagePath()));
			if (!tokenPix.isNull()) {
				tokenLbl->setPixmap(tokenPix.scaled(40, 40, Qt::KeepAspectRatio, Qt::SmoothTransformation));
			}
			tokenLbl->setToolTip(QString::fromStdString(token->getName()));
			headerLayout->addWidget(tokenLbl);
		}
	}

	mainLayout->addLayout(headerLayout);

	QFrame* line = new QFrame();
	line->setFrameShape(QFrame::HLine);
	line->setStyleSheet("background-color: rgba(255, 255, 255, 100); max-height: 1px; border: none;");
	mainLayout->addWidget(line);

	QLabel* wondersTitle = new QLabel("Wonders:");
	wondersTitle->setStyleSheet("color: #DDD; font-size: 14px; font-weight: bold; border: none; background: transparent;");
	mainLayout->addWidget(wondersTitle);

	QWidget* wondersContainer = new QWidget();
	wondersContainer->setStyleSheet("background: transparent; border: none;");
	fillWondersArea(player, wondersContainer, 80, 53, false);
	mainLayout->addWidget(wondersContainer);

	QLabel* cityTitle = new QLabel("City Structure:");
	cityTitle->setStyleSheet("color: #DDD; font-size: 14px; font-weight: bold; border: none; background: transparent;");
	mainLayout->addWidget(cityTitle);

	QWidget* cardsContainer = new QWidget();
	cardsContainer->setStyleSheet("background: transparent; border: none;");
	QHBoxLayout* cardsLayout = new QHBoxLayout(cardsContainer);
	cardsLayout->setSpacing(5);
	cardsLayout->setContentsMargins(0, 0, 0, 0);
	cardsLayout->setAlignment(Qt::AlignLeft);

	std::vector<CardColor> colors = {
		CardColor::BROWN, CardColor::GREY, CardColor::BLUE,
		CardColor::GREEN, CardColor::YELLOW, CardColor::RED, CardColor::PURPLE
	};

	bool hasAnyCard = false;
	for (const auto& color : colors) {
		auto cards = player.getCardsOfType(color);
		if (!cards.empty()) {
			hasAnyCard = true;
			QWidget* col = createColorColumn(cards, 40, 60);
			cardsLayout->addWidget(col);
		}
	}

	if (!hasAnyCard) {
		QLabel* noCards = new QLabel("(No cards)");
		noCards->setStyleSheet("color: gray; border: none;");
		cardsLayout->addWidget(noCards);
	}

	cardsLayout->addStretch();
	mainLayout->addWidget(cardsContainer);

	return frame;
}

void MainWindow::showFloatingText(const QString& text, const QString& colorStyle)
{
	QLabel* label = new QLabel(this);
	label->setText(text);
	label->setStyleSheet(colorStyle + "background-color: rgba(0,0,0,180); padding: 5px; border-radius: 5px;");
	label->adjustSize();
	label->show();
	m_activeMessages.append(label);

	int messageIndex = m_activeMessages.size() - 1;
	int spacing = 40; // intre mesaje

	int startY = this->height() / 2;
	int x = this->width() * 0.10;
	int y = startY + (messageIndex * spacing);

	label->move(x, y);
	label->raise();

	QTimer::singleShot(2500, [this, label]() {
		removeMessageLabel(label);
		});
}

void MainWindow::nonBlockingWait(int milliseconds) {
	QEventLoop loop;
	QTimer::singleShot(milliseconds, &loop, &QEventLoop::quit);
	loop.exec();
}

void MainWindow::showActionDialog(int cardIndex)
{
	const GameState& currentState = getCurrentGameState();
	auto cardView = currentState.getCardView(cardIndex);
	if (!cardView.has_value()) return;
	const Card& card = cardView->get();

	bool actionTaken = false;
	bool shouldReopenDialog = true;

	while (shouldReopenDialog)
	{
		shouldReopenDialog = false;

		QDialog dialog(this);
		dialog.setWindowTitle("Action for: " + QString::fromStdString(card.getName()));
		dialog.setMinimumWidth(300);

		QVBoxLayout* layout = new QVBoxLayout(&dialog);

		QLabel* infoLabel = new QLabel("Choose an action:", &dialog);
		infoLabel->setAlignment(Qt::AlignCenter);
		layout->addWidget(infoLabel);

		QPushButton* btnBuild = new QPushButton("Construct Building", &dialog);
		btnBuild->setStyleSheet("background-color: #4CAF50; color: white; padding: 10px;");
		layout->addWidget(btnBuild);

		QPushButton* btnDiscard = new QPushButton("Discard for Coins", &dialog);
		btnDiscard->setStyleSheet("background-color: #f44336; color: white; padding: 10px;");
		layout->addWidget(btnDiscard);

		QPushButton* btnWonder = new QPushButton("Construct Wonder", &dialog);
		btnWonder->setStyleSheet("background-color: #FF9800; color: white; padding: 10px;");
		layout->addWidget(btnWonder);

		// Variabila pentru a stoca alegerea utilizatorului
		// 0 = Cancel, 1 = Build, 2 = Discard, 3 = Wonder
		int userChoice = 0;


		connect(btnBuild, &QPushButton::clicked, [&]() {
			userChoice = 1;
			dialog.accept(); // Inchide dialogul curat
			});

		connect(btnDiscard, &QPushButton::clicked, [&]() {
			userChoice = 2;
			dialog.accept();
			});

		connect(btnWonder, &QPushButton::clicked, [&]() {
			userChoice = 3;
			dialog.accept();
			});

		dialog.exec();


		// Daca userul a inchis fereastra din X, userChoice ramane 0 -> iesim din functie
		if (userChoice == 0) return;

		// CAZ 1: CONSTRUCT BUILDING
		if (userChoice == 1) {
			if (m_isOnlineMode) {
				m_netClient->sendAction(cardIndex, PlayerAction::CONSTRUCT_BUILDING);
				// Nu stim daca e success, dar inchidem meniul
			}
			else {
				bool success = m_game.executeAction(cardIndex, PlayerAction::CONSTRUCT_BUILDING);
				if (success) actionTaken = true;
				else {
					// Daca a esuat (nu are bani), redeschidem dialogul ca sa aleaga altceva
					// Sau poti afisa un mesaj de eroare si sa il lasi sa incerce altceva
					shouldReopenDialog = true;
				}
			}
		}

		// CAZ 2: DISCARD
		else if (userChoice == 2) {
			if (m_isOnlineMode) {
				m_netClient->sendAction(cardIndex, PlayerAction::DISCARD_FOR_COINS);
			}
			else {
				bool success = m_game.executeAction(cardIndex, PlayerAction::DISCARD_FOR_COINS);
				if (success) actionTaken = true;
				else shouldReopenDialog = true;
			}
		}

		// CAZ 3: CONSTRUCT WONDER
		else if (userChoice == 3) {
			const Player& me = currentState.getCurrentPlayer();

			// Acum dialogul este INCHIS, deci putem da click linistiti pe minuni
			// askWonderSelection va porni propriul EventLoop, ceea ce e OK acum
			int wonderIndex = askWonderSelection(me.getWonders(), me.getName());

			if (wonderIndex != -1) {
				// Userul a ales o minune valida
				if (m_isOnlineMode) {
					m_netClient->sendAction(cardIndex, PlayerAction::CONSTRUCT_WONDER, wonderIndex);
				}
				else {
					bool success = m_game.executeAction(cardIndex, PlayerAction::CONSTRUCT_WONDER, wonderIndex);
					if (success) {
						actionTaken = true;
					}
					else {
						QMessageBox::warning(this, "Error", "Cannot build this wonder (insufficient resources or requirements).");
						shouldReopenDialog = true; // Redeschidem meniul sa incerce altceva
					}
				}
			}
			else {
				// Userul a dat Cancel la selectia minunii (sau click gresit)
				// Vrem sa ii aratam meniul din nou
				shouldReopenDialog = true;
			}
		}
	} // End While

	if (actionTaken && !m_isOnlineMode) {
		onStateUpdated();
		QApplication::processEvents();

		while (!m_game.isGameOver() && getCurrentGameState().getCurrentPlayer().isAI())
		{
			QThread::msleep(1000);
			AIController ai(m_aiDifficulty);
			AIMove move = ai.decideMove(getCurrentGameState());
			bool aiSuccess = m_game.executeAction(move.cardIndex, move.action, move.wonderIndex);

			if (!aiSuccess) {
				qDebug() << "AI Move failed, forcing discard";
				const auto& pyramid = getCurrentGameState().getPyramid();
				for (const auto& node : pyramid) {
					if (!node.m_isRemoved && getCurrentGameState().isCardAccessible(node.m_index)) {
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

void MainWindow::showGameOverScreen()
{
	if (!QFile::exists(":/assets/game_over.png")) {
		qDebug() << "ERROR: Image :/assets/game_over.png NOT FOUND in resources!";
	}
	const auto& state = getCurrentGameState();

	for (QLabel* lbl : m_activeMessages) {
		lbl->deleteLater();
	}
	m_activeMessages.clear();

	if (ui->pageGameOver->layout()) {
		QLayoutItem* item;
		while ((item = ui->pageGameOver->layout()->takeAt(0)) != nullptr) {
			delete item->widget();
			delete item;
		}
		delete ui->pageGameOver->layout();
	}

	QVBoxLayout* mainLayout = new QVBoxLayout(ui->pageGameOver);
	mainLayout->setAlignment(Qt::AlignCenter);

	mainLayout->setContentsMargins(50, 50, 50, 100);

	int winnerIndex = -1;
	if (state.getWinnerIndex().has_value()) {
		winnerIndex = state.getWinnerIndex().value();
	}

	QString titleText = "GAME OVER";
	if (std::abs(state.getMilitaryPosition()) >= GameConstants::MILITARY_SUPREMACY_DISTANCE)
		titleText = "MILITARY VICTORY!";
	else if (winnerIndex != -1 && state.getPlayers()[winnerIndex]->hasScientificVictory())
		titleText = "SCIENTIFIC VICTORY!";
	else if (winnerIndex != -1)
		titleText = "CIVILIAN VICTORY!";

	QString winnerText;
	if (winnerIndex != -1) {
		std::string winnerName = state.getPlayers()[winnerIndex]->getName();
		winnerText = "THE WINNER IS: " + QString::fromStdString(winnerName);
	}
	else {
		winnerText = "IT'S A DRAW!";
	}

	QLabel* lblTitle = new QLabel(titleText);
	lblTitle->setStyleSheet(
		"color: #FF0000; "
		"font-size: 64px; "
		"font-weight: 900; "
		"font-family: 'Arial Black';"
		"text-transform: uppercase;"
		"margin-bottom: 10px;"
	);
	lblTitle->setAlignment(Qt::AlignCenter);
	mainLayout->addWidget(lblTitle);

	QLabel* lblWinner = new QLabel(winnerText);
	lblWinner->setStyleSheet(
		"color: #FFD700; "
		"font-size: 32px; "
		"font-weight: bold; "
		"margin-bottom: 50px;"

		"background-color: rgba(0, 0, 0, 80); "
		"padding: 2px 8px; "
		"border-radius: 10px;"
		"border: none;"
	);
	lblWinner->adjustSize();
	lblWinner->setAlignment(Qt::AlignCenter);
	mainLayout->addWidget(lblWinner);

	QHBoxLayout* playersLayout = new QHBoxLayout();
	playersLayout->setSpacing(20);

	const Player& player1 = state.getCurrentPlayer();
	const Player& player2 = state.getOpponent();

	int score1 = player1.getFinalScore(player2);
	int score2 = player2.getFinalScore(player1);

	bool p1Winner = (winnerIndex == 0);
	bool p2Winner = (winnerIndex == 1);

	// Adăugăm widget-ul pentru Player 1
	playersLayout->addWidget(createPlayerEndGameWidget(player1, score1, p1Winner));

	// Label VS
	QLabel* vsLbl = new QLabel("VS");
	vsLbl->setStyleSheet("font-size: 24px; font-weight: bold; color: gray;");
	playersLayout->addWidget(vsLbl);

	// Adăugăm widget-ul pentru Player 2
	playersLayout->addWidget(createPlayerEndGameWidget(player2, score2, p2Winner));

	mainLayout->addLayout(playersLayout);

	// 6. Buton de "Back to Menu"
	QPushButton* btnMenu = new QPushButton("Back to Menu");
	btnMenu->setFixedSize(200, 50);
	btnMenu->setStyleSheet(
		"QPushButton { background-color: #d32f2f; color: white; font-size: 16px; font-weight: bold; border-radius: 8px; }"
		"QPushButton:hover { background-color: #b71c1c; }"
	);
	connect(btnMenu, &QPushButton::clicked, this, [this]() {
		if (m_isOnlineMode && m_netClient) {
			m_netClient->disconnect(this);
			m_netClient->disconnectFromHost();

		}
		cleanupVisuals();
		ui->stackedWidget->setCurrentIndex(0);
		});

	mainLayout->addSpacing(30);
	mainLayout->addWidget(btnMenu, 0, Qt::AlignCenter);

	ui->stackedWidget->setCurrentWidget(ui->pageGameOver);
}

void MainWindow::drawProgressTokens()
{
	for (QPushButton* btn : m_progressTokenButtons) {
		btn->deleteLater();
	}
	m_progressTokenButtons.clear();

	const auto& tokens = getCurrentGameState().getAvailableTokens();

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

	const auto& gameState = getCurrentGameState();
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
			if (m_isOnlineMode) {
				m_netClient->sendPickWonder(i);

				ui->cardContainer->setEnabled(false);
				ui->label->setText("Waiting for server...");
			}
			else {
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
						while (!m_game.isGameOver() &&
							getCurrentGameState().getCurrentPhase() == GamePhase::DRAFTING &&
							getCurrentGameState().getCurrentPlayer().isAI())
						{
							QThread::msleep(1000);
							AIController ai(AIDifficulty::HARD);
							int bestIndex = ai.pickWonder(getCurrentGameState());
							bool aiSuccess = m_game.pickWonder(bestIndex);
							if (!aiSuccess) {
								m_game.pickWonder(0);
							}
							onStateUpdated();
							QApplication::processEvents();
						}
					}
				}
				else {
					QMessageBox::warning(this, "Warning", "You can't choose this Wonder now.");
				}
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

	if (m_btnHint) {
		m_btnHint->hide();
	}

	if (ui->playerWonders) qDeleteAll(ui->playerWonders->findChildren<QWidget*>(QString(), Qt::FindDirectChildrenOnly));
	if (ui->playerCards) qDeleteAll(ui->playerCards->findChildren<QWidget*>(QString(), Qt::FindDirectChildrenOnly));

	if (ui->opponentWonders) qDeleteAll(ui->opponentWonders->findChildren<QWidget*>(QString(), Qt::FindDirectChildrenOnly));
	if (ui->opponentCards) qDeleteAll(ui->opponentCards->findChildren<QWidget*>(QString(), Qt::FindDirectChildrenOnly));

	ui->labelCurrentPlayerName->setText("-");
	ui->labelOpponentName->setText("-");

}

void MainWindow::highlightCardUI(int cardIndex) {
	for (QPushButton* btn : m_cardButtons) {
		int id = btn->property("myCardIndex").toInt();
		if (id == cardIndex) {
			btn->setStyleSheet(
				"QPushButton { "
				"  border: 12px solid #32CD32; "
				"  border-radius: 8px; "
				"  background-color: rgba(50, 205, 50, 150); "
				"}"
			);
			btn->raise();
		}
		else {
			btn->setStyleSheet(
				"QPushButton { border: 2px solid rgba(0,0,0,150); background-color: transparent; border-radius: 5px; }"
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
	if (m_isOnlineMode) {
		if (!m_networkState) return;
	}
	else {
		if (!m_game.hasGameStarted()) return;
	}

	if (m_game.isGameOver()) return;


	const auto& gameState = getCurrentGameState();

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
	const auto& gameState = getCurrentGameState();
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
	for (int i = 0; i < 2; ++i)
		if (i == 0)
			m_age3Layout[9 + i] = { startX + static_cast<int>(1.5 * stepX) + i * stepX, startY_Age3 + 3 * verticalOverlap };
		else
			m_age3Layout[9 + i] = { startX + static_cast<int>(3.5 * stepX) + (i - 1) * stepX, startY_Age3 + 3 * verticalOverlap };

	// Randul 3 (4 carti)
	for (int i = 0; i < 4; ++i) m_age3Layout[11 + i] = { startX + stepX + i * stepX, startY_Age3 + 2 * verticalOverlap };

	// Randul 2 (3 carti)
	for (int i = 0; i < 3; ++i) m_age3Layout[15 + i] = { startX + stepX + (stepX / 2) + i * stepX, startY_Age3 + 1 * verticalOverlap };

	// Randul 1 (sus 2 carti)
	for (int i = 0; i < 2; ++i) m_age3Layout[18 + i] = { startX + (2 * stepX) + i * stepX, startY_Age3 };
}

void MainWindow::showHintText(const QString& text)
{
	QList<QLabel*> existingLabels = this->findChildren<QLabel*>("hintLabel");
	qDeleteAll(existingLabels);

	QLabel* label = new QLabel(this);
	label->setObjectName("hintLabel");
	label->setText(text);
	label->setStyleSheet("font-size: 20px; font-weight: bold; color: #FFFFFF; background-color: rgba(0,0,0,200); padding: 10px; border-radius: 8px; border: 2px");
	label->adjustSize();

	int x = (this->width() - label->width()) / 2;
	int y = this->height() - 75;

	label->move(x, y);
	label->show();
	label->raise();

	QTimer::singleShot(4000, label, &QLabel::deleteLater);
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

	const auto& gameState = getCurrentGameState();
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
		else {
			finalPixmap = cardPixmap.scaled(cardW, cardH, Qt::KeepAspectRatio, Qt::SmoothTransformation);
		}

		QPushButton* cardButton = new QPushButton(ui->cardContainer);
		const CardPosition& pos = (*currentLayout)[node.m_index];

		cardButton->setProperty("myCardIndex", node.m_index);
		cardButton->setGeometry(pos.x, pos.y, cardW, cardH);
		cardButton->setIcon(QIcon(finalPixmap));
		cardButton->setIconSize(QSize(cardW, cardH));
		cardButton->setText("");

		if (node.m_isFaceUp) {
			auto cardViewOpt = gameState.getCardView(node.m_index);
			if (cardViewOpt) {
				const Card& card = cardViewOpt->get();
				cardButton->setToolTip(
					QString::fromStdString(card.displayCardInfo())
				);
			}
		}

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

void MainWindow::removeMessageLabel(QLabel* label) {
	if (m_activeMessages.contains(label)) {
		m_activeMessages.removeOne(label);
		label->deleteLater();

		int startY = this->height() / 2;
		int spacing = 40;
		for (int i = 0; i < m_activeMessages.size(); ++i) {
			m_activeMessages[i]->move(m_activeMessages[i]->x(), startY + (i * spacing));
		}
	}
}

MainWindow::~MainWindow()
{
	delete ui;
}
