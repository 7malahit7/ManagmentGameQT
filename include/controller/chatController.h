#pragma once
#include <QObject>
#include <QJsonDocument>

class ChatController : public QObject
{
    Q_OBJECT
public:
    explicit ChatController(quint8 localId, bool isServerMode = false, QObject* parent = nullptr);

    void onLocalMessage(const QString &text);
    void onNetworkMessage(const QJsonDocument &msg);

signals:
    void sendMessage(const QJsonDocument &msg);
    void newMessage(const QString &text);

private:
    quint8 localId;
    bool isServerMode;
};
