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

    void onPlayerReadyFromNetwork(int playerId);
private slots:
    // MODEL >> UI
    void onMonthChanged(int month);
    void onStateChanged(GameState state);
    void onMarketChanged(const MarketState& market);
    // NETWORK >> UI
    void updatePlayersOnScreen(const QVector<PlayerModel>& players);
    // UI >> MODEL / NETWORK
    void onReadyClicked();

private:
    void prepareLocalPlayer(const QString& name, bool isServer);
    void setupServerNetwork(PlayerModel& player);
    void setupClientNetwork(PlayerModel& player, const QString& host);
    void setupChat(bool isServer,
                   void (NetworkController::*sendFunc)(const QJsonDocument&, MessageKind));
    void showGameScreen();

private:
    GameModel* m_model = nullptr;
    NetworkController* m_network = nullptr;

    MenuWidget* m_menu = nullptr;
    MainGameScreen* m_gameScreen = nullptr;
    ChatController* m_chatController = nullptr;
};
