#include "chatController.h"
#include <QDebug>
#include <QJsonObject>

ChatController::ChatController(quint8 localId, const QString& playerName, bool isServerMode, QObject *parent)
    : QObject(parent), localId(localId), isServerMode(isServerMode), playerName(playerName)
{
}

void ChatController::sendChatMessage(const QString &text)
{
    if(text.isEmpty()) return;

    QJsonObject obj;
    obj["type"] = "chat_message";
    obj["name"] = playerName;
    obj["text"] = text;

    QJsonDocument msg(obj);
    qDebug() << "[ChatController] message sent to Network";
    emit sendMessageToNetwork(msg, MessageKind::UserMessage); // отправка через сеть
}

void ChatController::onNetworkMessage(const QJsonDocument &msg, MessageKind isSystemMessage)
{
    qDebug() << "[ChatController] Network Message Received, sending to chat";
    QJsonObject obj = msg.object();
    QString senderName = obj["name"].toString();
    QString text = obj["text"].toString();
    emit newMessageFromNetwork(QString("%1%2 %3").arg(senderName, isSystemMessage ? ' ' : ':' ,text), isSystemMessage); // отображение сообщения с именем
}
