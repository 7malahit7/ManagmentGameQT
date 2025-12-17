#pragma once
#include <QObject>
#include "gameSession.h"
#include "menuWidget.h"
#include "mainGameScreen.h"
#include "chatController.h"

class MainController : public QObject
{
    Q_OBJECT
public:
    explicit MainController(QObject* parent = nullptr);

    MenuWidget* getMenu() const { return menu; }
    MainGameScreen* getGameScreen() const { return mainGameScreen; }

signals:
    void gameScreenRequested();

private:
    GameSession* session;
    MenuWidget* menu;
    MainGameScreen* mainGameScreen;
    ChatController* chatController;
    bool isServerMode;

    void initChatController(bool isServer);
};
