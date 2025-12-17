#include "gameSession.h"
#include "playerModel.h"
#include "clientController.h"
#include "serverController.h"

#include <QDebug>

GameSession::GameSession(QObject* parent)
    : QObject(parent)
{
    localPlayer = new PlayerModel("unknown", false, 0);
    network = nullptr;
}

void GameSession::startGameAsHost(bool serverMode)
{
    if(!network)
    {
        network = new ServerController(localPlayer, this);
    }

    emit gameStarted();
}
void GameSession::connectToGame(const QString& host)
{
    if(!network)
    {
        network = new ClientController(localPlayer, host, this);
    }
}
