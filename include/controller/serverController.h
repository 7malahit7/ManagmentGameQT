#pragma once

#include "networkController.h"
#include "playerModel.h"
#include <QMap>
class ServerController : public NetworkController
{
    Q_OBJECT
signals:
private slots:
    void onNewConnection();
    void onDataReceived() override;
    void sendChatMessage(const QJsonDocument &msg, bool isSystem) override;
public slots:
    void broadcast(const QJsonDocument &msg, bool isSystem) override;
public:
    ServerController(PlayerModel* player, QObject* parent = nullptr);
    void startServer();
private:
    QJsonDocument sendNewClientToChat(const QString& message);
    void assignId(QTcpSocket* socket,int id);
    QTcpServer* server = nullptr;

    quint8 nextId = 1;
    QMap<QTcpSocket*, PlayerModel*> socketToPlayer;
};
