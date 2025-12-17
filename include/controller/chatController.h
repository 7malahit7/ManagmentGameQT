#pragma once
#include <QObject>
#include <QJsonDocument>

class ChatController : public QObject
{
    Q_OBJECT
public:
    explicit ChatController(quint8 localId, const QString& playerName, bool isServerMode = false, QObject* parent = nullptr);

    void sendChatMessage(const QString &text);
    void onNetworkMessage(const QJsonDocument &msg, bool isSystemMessage);

signals:
    void sendMessageToNetwork(const QJsonDocument &msg, bool isSystemMessage);
    void newMessageFromNetwork(const QString &text, bool isSystemMessage);

private:
    quint8 localId;
    bool isServerMode;
    QString playerName;
};
