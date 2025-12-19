#include "mainWindow.h"
#include <QFont>

MainWindow::MainWindow(MainController* controller, QWidget* parent)
    : QMainWindow(parent)
{
    menuScreen = controller->menu();
    gameScreen = controller->gameScreen();

    screenSwitcher = new QStackedWidget(this);
    screenSwitcher->addWidget(menuScreen);
    screenSwitcher->addWidget(gameScreen);
    setCentralWidget(screenSwitcher);

    setMinimumSize(400, 400);
    resize(400, 400);
    setWindowTitle("Меню");
    QFont font1("Segoe UI", 14);
    setFont(font1);
    connect(controller, &MainController::gameScreenRequested, [this]() {
        screenSwitcher->setCurrentWidget(gameScreen);

        setMaximumSize(1920,1080);
        showMaximized();
    });
}
