#include "MainWindow.h"
#include "./ui_MainWindow.h" 
#include <QFileDialog> 
#include <QMessageBox>

MainWindow::MainWindow(QWidget* parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this); 

	ui->stackedWidget->setCurrentIndex(0);

    connect(ui->btnStart, &QPushButton::clicked, this, &MainWindow::onBtnStartClicked);
    connect(ui->btnLoad, &QPushButton::clicked, this, &MainWindow::onBtnLoadClicked);
	connect(ui->btnExit, &QPushButton::clicked, this, &MainWindow::onBtnExitClicked);

    connect(ui->btnBack, &QPushButton::clicked, this, [this]() {
        ui->stackedWidget->setCurrentIndex(0); // Înapoi la meniu
        });

    this->setWindowTitle("7 Wonders Duel");
}

void MainWindow::onBtnStartClicked()
{
    // Aici vom porni logica jocului
    // m_game.startNewGame("Player 1", "AI");

    // Trecem la pagina de joc
    ui->stackedWidget->setCurrentIndex(1);
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
    }
    catch (const std::exception& e) {
        QMessageBox::critical(this, "Error", QString("Could not load game:\n").arg(e.what()));
    }
}

void MainWindow::onBtnExitClicked() {
    QApplication::quit();
}

MainWindow::~MainWindow()
{
    delete ui;
}