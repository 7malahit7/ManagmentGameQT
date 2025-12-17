#pragma once
#include <QObject>
#include <QJsonDocument>

class ChatController : public QObject
{
    Q_OBJECT
public:
    explicit ChatController(quint8 localId, const QString& playerName, bool isServerMode = false, QObject* parent = nullptr);

    void sendChatMessage(const QString &text);
    void onNetworkMessage(const QJsonDocument &msg);

signals:
    void sendMessageToNetwork(const QJsonDocument &msg);
    void newMessageFromNetwork(const QString &text);

private:
    quint8 localId;
    bool isServerMode;
    QString playerName;
};
