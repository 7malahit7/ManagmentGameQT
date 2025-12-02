#pragma once

#include<QMainWindow>
#include<QStackedWidget>

#include "mainGameWindow/mainGameScreen.h"
#include "menu/menu.h"

class MainWindow : public QMainWindow
{
    Q_OBJECT

private:
    QStackedWidget* screenSwitcher;
    MainGameWindow* gameScreen;
    MenuWidget* menuScreen;
public:
    explicit MainWindow(const QVector<PlayerWidget*>& players,QWidget* parent = nullptr);
    ~MainWindow() = default;

private slots:
    void switchToGameScreen();
signals:


};
