#pragma once
#include <QObject>
#include <QJsonDocument>
#include "message_types.h"
class ChatController : public QObject
{
    Q_OBJECT
public:
    explicit ChatController(quint8 localId, const QString& playerName, bool isServerMode = false, QObject* parent = nullptr);

    void sendChatMessage(const QString &text);
    void onNetworkMessage(const QJsonDocument &msg, MessageKind isSystemMessage);

signals:
    void sendMessageToNetwork(const QJsonDocument &msg, MessageKind isSystemMessage);
    void newMessageFromNetwork(const QString &text, MessageKind isSystemMessage);

private:

    quint8 localId;
    bool isServerMode;
    QString playerName;
};
