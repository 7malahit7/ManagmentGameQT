#include "chatController.h"
#include <QDebug>
#include <QJsonObject>

ChatController::ChatController(quint8 localId, const QString& playerName, bool isServerMode, QObject *parent)
    : QObject(parent), localId(localId), isServerMode(isServerMode), playerName(playerName)
{
}

void ChatController::onLocalMessage(const QString &text)
{
    if(text.isEmpty()) return;

    qDebug() << "[ChatController] Local message:" << text;

    // создаём JSON-сообщение с id, именем и текстом
    QJsonObject obj;
    obj["id"] = localId;
    obj["name"] = playerName;
    obj["text"] = text;
    QJsonDocument msg(obj);

    emit sendMessage(msg); // отправка через сеть
    emit newMessage(QString("%1: %2").arg(playerName, text)); // отображение в локальном чате
}

void ChatController::onNetworkMessage(const QJsonDocument &msg)
{
    QJsonObject obj = msg.object();
    quint8 id = obj["id"].toInt();
    QString senderName = obj["name"].toString();
    QString text = obj["text"].toString();

    qDebug() << "[ChatController] Network message received from" << senderName << ":" << text;

    // клиент не должен отображать свои же сообщения, отправленные на сервер
    if(!isServerMode && id == localId) {
        qDebug() << "[ChatController] Ignored own message (client)";
        return;
    }

    emit newMessage(QString("%1: %2").arg(senderName, text)); // отображение сообщения с именем
}
