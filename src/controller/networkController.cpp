#include "networkController.h"
#include <QDebug>
#include <qjsonobject.h>

NetworkController::NetworkController(PlayerModel* player, QObject *parent)
    : QObject(parent), localPlayer{player}
{
    connect(this, &NetworkController::sendToNetwork, this, &NetworkController::onDataReceived);
}

QByteArray NetworkController::messageToSendingByteArray(const QJsonObject &obj)
{
    return QJsonDocument(obj).toJson(QJsonDocument::Compact) + '\n';
}
QByteArray NetworkController::messageToSendingByteArray(const QJsonDocument &obj)
{
    return obj.toJson(QJsonDocument::Compact) + '\n';
}
