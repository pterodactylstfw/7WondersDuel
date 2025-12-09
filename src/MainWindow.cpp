#include "MainWindow.h"
#include <QLabel>
#include <QFont>

MainWindow::MainWindow(QWidget* parent)
    : QMainWindow(parent)
{
    setWindowTitle("7 Wonders Duel - Qt Edition");
    resize(1280, 720);

    QLabel* label = new QLabel("TEST FEREASTRA", this);


    QFont font = label->font();
    font.setPointSize(40);
    font.setBold(true);
    label->setFont(font);

    // 3. Aliniem textul la mijloc
    label->setAlignment(Qt::AlignCenter);

    this->setCentralWidget(label);
}

MainWindow::~MainWindow()
{
}