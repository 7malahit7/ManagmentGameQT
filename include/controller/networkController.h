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
    explicit NetworkController(bool serverMode = true, const QString &host = "", QObject* parent = nullptr);

    void startServer();
    void connectToServer();
    void sendMessageToServerOrBroadcast(const QJsonDocument &msg);

signals:
    void connected();
    void messageReceived(const QJsonDocument &msg);

private slots:
    void onNewConnection();
    void onDataReceived();

private:
    bool isServer;
    QString host;
    QTcpServer* server = nullptr;
    QTcpSocket* socket = nullptr;
    QVector<QTcpSocket*> clients;
};
