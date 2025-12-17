#pragma once

#include "networkController.h"

class ServerController : public NetworkController
{
    Q_OBJECT
signals:
private slots:
    void onNewConnection();
    void onDataReceived() override;
public:
    ServerController(QObject* parent = nullptr);
    void sendChatMessageToServerOrBroadcast(const QJsonDocument &msg) override;
    void startServer();
private:
    void sendNewClientToChat();
    QVector<QTcpSocket*> clients;
    QTcpServer* server = nullptr;


};
