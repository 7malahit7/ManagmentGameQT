#pragma once

#include "networkController.h"

class ClientController : public NetworkController
{
    Q_OBJECT
signals:
    void connected();
private slots:
    void onDataReceived() override;
public slots:
    void sendChatMessage(const QJsonDocument &msg) override;
    void broadcast(const QJsonDocument &msg) override;
public:
    ClientController(const QString &host, QObject* parent = nullptr);
    void connectToServer();

private:
    QString host;
    QTcpSocket* socket = nullptr;

};
