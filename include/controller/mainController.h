#pragma once

#include <QObject>
#include <QVector>

#include "gameModel.h"
#include "message_types.h"
#include "playerModel.h"

class NetworkController;
class ServerController;
class ClientController;
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

signals:
    void gameScreenRequested();

public slots:
    void startServer(const QString& playerName);
    void connectToServer(const QString& playerName, const QString& host);

private slots:
    void updatePlayersOnScreen(const QVector<PlayerModel>& players);

private:
    void prepareLocalPlayer(const QString& name, bool isServer);
    void setupServerNetwork(PlayerModel& player);
    void setupClientNetwork(PlayerModel& player, const QString& host);
    void setupChat(bool isServer, void (NetworkController::*sendFunc)( const QJsonDocument&, MessageKind));


    GameModel* m_model;
    NetworkController* m_network;


    MenuWidget* m_menu;
    MainGameScreen* m_gameScreen;


    ChatController* m_chatController;
    void showGameScreen();
};
