#include "networkController.h"

NetworkController::NetworkController(PlayerModel* player, QObject* parent)
    : QObject(parent),
    m_player(player)
{
}

QByteArray NetworkController::messageToSendingByteArray(
    const QJsonObject& obj
    ) const
{
    return QJsonDocument(obj).toJson(QJsonDocument::Compact) + '\n';
}

QByteArray NetworkController::messageToSendingByteArray(
    const QJsonDocument& doc
    ) const
{
    return doc.toJson(QJsonDocument::Compact) + '\n';
}
