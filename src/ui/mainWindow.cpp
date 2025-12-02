#include "mainWindow.h"



MainWindow::MainWindow(const QVector<PlayerWidget*>& players,QWidget* parent)
{

    setWindowTitle("ManagmentGame");

    screenSwitcher = new QStackedWidget(this);
    gameScreen = new MainGameWindow(players,this);
    menuScreen = new MenuWidget(this);

    setCentralWidget(screenSwitcher);

    screenSwitcher->addWidget(gameScreen);
    screenSwitcher->addWidget(menuScreen);
    screenSwitcher->setCurrentWidget(menuScreen);

    setMinimumSize(360, 320);
    setMaximumSize(360, 320);
    resize(360, 320);

    QFont font1("Segoe UI", 14);
    setFont(font1);

    show();

    connect(menuScreen, &MenuWidget::switchToGameScreen, this, &MainWindow::switchToGameScreen);
}

void MainWindow::switchToGameScreen()
{
    setMinimumSize(800, 800);
    setMaximumSize(QWIDGETSIZE_MAX, QWIDGETSIZE_MAX);
    showMaximized();
    screenSwitcher->setCurrentWidget(gameScreen);
}
