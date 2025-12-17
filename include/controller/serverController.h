#pragma once

#include "networkController.h"
#include <QMap>
class ServerController : public NetworkController
{
    Q_OBJECT
signals:
private slots:
    void onNewConnection();
    void onDataReceived() override;
    void sendChatMessage(const QJsonDocument &msg) override;
public slots:
    void broadcast(const QJsonDocument &msg) override;
public:
    ServerController(QObject* parent = nullptr);
    void startServer();
private:
    void sendNewClientToChat();
     QVector<QTcpSocket*> clients;
    QTcpServer* server = nullptr;

    quint8 nextId = 2;
    QMap<QTcpSocket*, quint8> socketToId;
    QMap<quint8, QString> idToName;
};
