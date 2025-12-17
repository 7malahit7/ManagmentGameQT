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
signals:
    void messageReceived(const QJsonDocument &msg);
    void sendToNetwork();
public slots:
    virtual void onDataReceived() = 0;

    virtual void sendChatMessage(const QJsonDocument &msg) = 0;
    virtual void broadcast(const QJsonDocument &msg) = 0;
};

