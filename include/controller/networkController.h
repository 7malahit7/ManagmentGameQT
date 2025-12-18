#pragma once
#include <QObject>
#include <QTcpServer>
#include <QTcpSocket>
#include <QJsonDocument>
#include <QVector>
#include "playerModel.h"
#include "message_types.h"
class NetworkController : public QObject
{
    Q_OBJECT
protected:
    PlayerModel *localPlayer;
    QByteArray messageToSendingByteArray(const QJsonObject &obj);
    QByteArray messageToSendingByteArray(const QJsonDocument &obj);
public:
    explicit NetworkController(PlayerModel* player, QObject* parent = nullptr);
signals:
    void sendMessageToChatController(const QJsonDocument &msg, MessageKind isSystemMessage);
    void sendToNetwork();
public slots:
    virtual void onDataReceived() = 0;
    virtual void sendChatMessage(const QJsonDocument &msg, MessageKind isSystem) = 0;
    virtual void broadcast(const QJsonDocument &msg, MessageKind isSystem) = 0;

};

