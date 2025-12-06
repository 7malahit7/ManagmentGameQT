#include "networkController.h"
#include <QDebug>

NetworkController::NetworkController(bool serverMode, const QString &host, QObject *parent)
    : QObject(parent), isServer(serverMode), host(host)
{
    qDebug() << "[NetworkController] Created, serverMode=" << isServer << ", host=" << host;
    if(isServer) startServer();
    else connectToServer();
}

void NetworkController::startServer()
{
    server = new QTcpServer(this);
    if(!server->listen(QHostAddress::Any, 7777)) {
        qWarning() << "[NetworkController] Server failed:" << server->serverError();
        return;
    }
    connect(server, &QTcpServer::newConnection, this, &NetworkController::onNewConnection);
    qDebug() << "[NetworkController] Server started!";
}

void NetworkController::connectToServer()
{
    if(socket && (socket->state() == QAbstractSocket::ConnectedState ||
                   socket->state() == QAbstractSocket::ConnectingState)) {
        qDebug() << "[NetworkController] Already connecting/connected";
        return; // не создаём новый сокет
    }

    socket = new QTcpSocket(this);
    qDebug() << "[NetworkController] Trying to connect to host" << host;

    connect(socket, &QTcpSocket::readyRead, this, &NetworkController::onDataReceived);
    connect(socket, &QTcpSocket::connected, this, [this](){
        qDebug() << "[NetworkController] Connected to server!";
        emit connected();
    });
    connect(socket, &QTcpSocket::errorOccurred, [](QAbstractSocket::SocketError e){
        qDebug() << "[NetworkController] Socket error:" << e;
    });

    socket->connectToHost(host, 7777);
}

void NetworkController::onNewConnection()
{
    QTcpSocket* clientSocket = server->nextPendingConnection();
    clients.append(clientSocket);

    qDebug() << "[NetworkController] Client connected! Total clients:" << clients.size();

    connect(clientSocket, &QTcpSocket::readyRead, this, &NetworkController::onDataReceived);
}

void NetworkController::onDataReceived()
{
    QTcpSocket* senderSocket = qobject_cast<QTcpSocket*>(sender());
    if(!senderSocket) return;

    QByteArray data = senderSocket->readAll();
    QJsonDocument doc = QJsonDocument::fromJson(data);

    qDebug() << "[NetworkController] Message received:" << data;

    emit messageReceived(doc);

    if(isServer){
        for(QTcpSocket* client : clients){
            if(client != senderSocket){
                client->write(doc.toJson());
                qDebug() << "[NetworkController] Broadcasted message to client";
            }
        }
    }
}

void NetworkController::sendMessageToServerOrBroadcast(const QJsonDocument &msg)
{
    if(isServer){
        for(QTcpSocket* client : clients){
            client->write(msg.toJson());
            qDebug() << "[NetworkController] Server sent message to client";
        }
    } else {
        if(socket) {
            socket->write(msg.toJson());
            qDebug() << "[NetworkController] Client sent message to server:" << msg.toJson();
        }
    }
}
