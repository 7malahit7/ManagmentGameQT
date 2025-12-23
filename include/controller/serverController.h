#pragma once

#include "networkController.h"
#include "playerModel.h"

#include <QTcpServer>
#include <QTcpSocket>
#include <QMap>
#include <QVector>
#include <QHash>

class ServerController : public NetworkController
{
    Q_OBJECT

signals:
    void updatePlayers(const QVector<PlayerModel>& playersInfo);
    void playerReadyReceived(int playerId);

private slots:
    void onNewConnection();
    void onDataReceived() override;

public slots:
    void broadcast(const QJsonDocument& msg, MessageKind kind) override;
    void sendChatMessage(const QJsonDocument& msg, MessageKind kind) override;
    void sendReady(const QJsonDocument& doc) override;


    void emitInitialPlayers();

public:
    explicit ServerController(PlayerModel* player, QObject* parent = nullptr);
    void startServer();

private:
    QTcpServer* server = nullptr;
    quint8 nextId = 1;

    QHash<QTcpSocket*, QByteArray> m_buffers;
    QMap<QTcpSocket*, PlayerModel*> socketToPlayer;

    QVector<PlayerModel> playersInfo;

private:
    void processIncomingBuffer(QTcpSocket* socket);
    void handleNewClient(QTcpSocket* socket, const QJsonObject& root);
    void assignId(QTcpSocket* socket, int id);
    void broadcastPlayersInfo();
    QJsonDocument sendNewClientToChat(const QString& name);
    void sendAssignId(QTcpSocket *socket, int id);
};
