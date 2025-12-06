#include "chatController.h"
#include <QDebug>
#include <QJsonObject>
ChatController::ChatController(quint8 localId, bool isServerMode, QObject *parent)
    : QObject(parent), localId(localId), isServerMode(isServerMode) {}

void ChatController::onLocalMessage(const QString &text)
{
    qDebug() << "[ChatController] Local message:" << text;

    QJsonObject obj;
    obj["id"] = localId;
    obj["text"] = text;
    QJsonDocument msg(obj);

    emit sendMessage(msg);
    emit newMessage(text);
}

void ChatController::onNetworkMessage(const QJsonDocument &msg)
{
    QJsonObject obj = msg.object();
    quint8 id = obj["id"].toInt();
    QString text = obj["text"].toString();

    qDebug() << "[ChatController] Network message received:" << text << ", id:" << id;

    if(!isServerMode && id == localId) {
        qDebug() << "[ChatController] Ignored own message (client)";
        return;
    }

    emit newMessage(text);
}
