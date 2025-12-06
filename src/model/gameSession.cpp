#include "gameSession.h"
#include "playerModel.h"
#include "networkController.h"
#include <QDebug>

GameSession::GameSession(QObject* parent)
    : QObject(parent)
{
    localPlayer = new PlayerModel("Player", true, 1);
    network = nullptr;
}

void GameSession::startGame(bool serverMode)
{
    if(serverMode && !network) {
        network = new NetworkController(true, "", this);
        qDebug() << "Server started!";
    }

    emit gameStarted();
}
void GameSession::startNetworkGame(const QString& host)
{
    if(!network) {
        network = new NetworkController(false, host, this);
        network->connectToServer();
    } else {
        qDebug() << "[GameSession] Network already exists, skipping connect";
    }
}
