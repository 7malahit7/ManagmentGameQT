#include "mainWindow.h"

MainWindow::MainWindow(MainController* controller, QWidget* parent)
    : QMainWindow(parent)
{
    menuScreen = controller->getMenu();
    gameScreen = controller->getGameScreen();

    screenSwitcher = new QStackedWidget(this);
    screenSwitcher->addWidget(menuScreen);
    screenSwitcher->addWidget(gameScreen);
    setCentralWidget(screenSwitcher);

    // Старт игры или подключение → смена экрана
    connect(controller, &MainController::gameScreenRequested, [this]() {
        screenSwitcher->setCurrentWidget(gameScreen);
    });
}
