#pragma once
#include <QObject>
#include <QTcpServer>
#include <QTcpSocket>
#include <QJsonDocument>
#include <QVector>

class NetworkController : public QObject
{
    Q_OBJECT
public:
    explicit NetworkController(QObject* parent = nullptr);
    virtual void sendChatMessageToServerOrBroadcast(const QJsonDocument &msg) = 0;
signals:
    void messageReceived(const QJsonDocument &msg);
    void sendToNetwork();
public slots:
    virtual void onDataReceived() = 0;
};
