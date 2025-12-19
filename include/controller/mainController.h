#pragma once

#include <QObject>
#include "gameModel.h"

class NetworkController;
class ChatController;
class MenuWidget;
class MainGameScreen;

class MainController : public QObject
{
    Q_OBJECT
public:
    explicit MainController(GameModel* model, QObject* parent = nullptr);

    MenuWidget* menu() const;
    MainGameScreen* gameScreen() const;
    void startServer(const QString& playerName);
    void connectToServer(const QString& playerName, const QString& host);
signals:
    void gameScreenRequested();

private:
    GameModel* m_model;               // MVC: Model (НЕ владеем)
    NetworkController* m_network;     // Controller управляет сетью

    MenuWidget* m_menu;               // View
    MainGameScreen* m_gameScreen;     // View

    ChatController* m_chatController;
};
