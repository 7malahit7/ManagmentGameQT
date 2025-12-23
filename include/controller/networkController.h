#pragma once

#include <QObject>
#include <QJsonDocument>
#include <QJsonObject>
#include <QByteArray>

#include "message_types.h"
#include "playerModel.h"

class NetworkController : public QObject
{
    Q_OBJECT
signals:
    void sendMessageToChatController(const QJsonDocument& doc,MessageKind kind);
public:
    explicit NetworkController(PlayerModel* player, QObject* parent = nullptr);
    virtual ~NetworkController() = default;

public slots:
    virtual void broadcast(const QJsonDocument&, MessageKind) = 0;
    virtual void sendChatMessage(const QJsonDocument&, MessageKind) = 0;
    virtual void sendReady(const QJsonDocument&) = 0;

protected slots:
    virtual void onDataReceived() = 0;

protected:
    QByteArray messageToSendingByteArray(const QJsonObject& obj) const;
    QByteArray messageToSendingByteArray(const QJsonDocument& doc) const;

protected:
    PlayerModel* m_player;
};
