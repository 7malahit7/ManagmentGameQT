#include "networkController.h"
#include <QDebug>

NetworkController::NetworkController(PlayerModel* player, QObject *parent)
    : QObject(parent), localPlayer{player}
{
    connect(this, &NetworkController::sendToNetwork, this, &NetworkController::onDataReceived);
}
