#pragma once
#include <QObject>
#include <QTcpServer>
#include <QTcpSocket>
#include <QJsonDocument>
#include <QVector>
#include "playerModel.h"
class NetworkController : public QObject
{
    Q_OBJECT
public:
    explicit NetworkController(PlayerModel* player, QObject* parent = nullptr);
signals:
    void messageReceived(const QJsonDocument &msg, bool isSystemMessage);
    void sendToNetwork();
public slots:
    virtual void onDataReceived() = 0;

    virtual void sendChatMessage(const QJsonDocument &msg, bool isSystem) = 0;
    virtual void broadcast(const QJsonDocument &msg, bool isSystem) = 0;
protected:
    PlayerModel *localPlayer;
};

