#pragma once
#include <QMainWindow>
#include <QStackedWidget>
#include "mainController.h"
#include "mainGameScreen.h"
#include "menuWidget.h"

class MainWindow : public QMainWindow
{
    Q_OBJECT
public:
    explicit MainWindow(MainController* controller, QWidget* parent = nullptr);

private:
    QStackedWidget* screenSwitcher;
    MainGameScreen* gameScreen;
    MenuWidget* menuScreen;
};
