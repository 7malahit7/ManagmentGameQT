#pragma once

#include "networkController.h"

class ClientController : public NetworkController
{
    Q_OBJECT
signals:
    void connected();
private slots:
    void onDataReceived() override;
public:
    ClientController(const QString &host, QObject* parent = nullptr);
    void sendChatMessageToServerOrBroadcast(const QJsonDocument &msg) override;
    void connectToServer();

private:
    QString host;
    QTcpSocket* socket = nullptr;

};
