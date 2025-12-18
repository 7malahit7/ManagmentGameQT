#pragma once

#include "networkController.h"
#include "playerModel.h"
#include <QMap>
#include <QVector>
class ServerController : public NetworkController
{
    Q_OBJECT
signals:
    void updatePlayers(const QVector<PlayerModel>& playersInfo);
private slots:
    void onNewConnection();
    void onDataReceived() override;

public slots:
    void broadcast(const QJsonDocument &msg, MessageKind isSystem) override;
    void sendChatMessage(const QJsonDocument &msg, MessageKind isSystem) override;
    void emitInitialPlayers();
public:
    ServerController(PlayerModel* player, QObject* parent = nullptr);
    void startServer();
private:
    QJsonDocument sendNewClientToChat(const QString& name);
    void assignId(QTcpSocket* socket,int id);
    QTcpServer* server = nullptr;

    quint8 nextId = 1;
    QMap<QTcpSocket*, PlayerModel*> socketToPlayer;
    QVector<PlayerModel> playersInfo;

    void broadcastPlayerInfo();
};
