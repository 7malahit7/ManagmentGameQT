#include "clientController.h"
#include <QDebug>
#include <QJsonObject>
ClientController::ClientController(PlayerModel* player, const QString &host, QObject *parent)
    : NetworkController(player, parent), host{host}
{
    connectToServer();
}

void ClientController::connectToServer()
{

    if(socket && (socket->state() == QAbstractSocket::ConnectedState ||
                   socket->state() == QAbstractSocket::ConnectingState)) {
        qDebug() << "[NetworkController] Already connecting/connected";
        return;
    }

    socket = new QTcpSocket(this);
    qDebug() << "[ClientController] Trying to connect to host" << host;


    connect(socket, &QTcpSocket::connected, this, [this](){
        connect(socket, &QTcpSocket::readyRead, this, &ClientController::onDataReceived);
        qDebug() << "[ClientController] Connected to server!";
        emit connected();
    });
    connect(socket, &QTcpSocket::errorOccurred, [](QAbstractSocket::SocketError e){
        qDebug() << "[ClientController] Socket error:" << e;
    });
    if(host.isEmpty()) host = "127.1";
    socket->connectToHost(host, 7777);
    sendNameToServer();
}

void ClientController::sendNameToServer()
{
    QJsonObject obj;
    obj["type"] = "new_client";
    obj["name"] = localPlayer->getName();
    QJsonDocument msg(obj);
    QByteArray payload = msg.toJson(QJsonDocument::Compact);
    payload.append('\n');
    socket->write(payload);
}


void ClientController::onDataReceived()
{
    qDebug() << "[ClientController] Data Received!";
    QTcpSocket* senderSocket = qobject_cast<QTcpSocket*>(sender());
    if(!senderSocket) return;

    QJsonDocument doc = QJsonDocument::fromJson(senderSocket->readAll());
    QString type = doc.object()["type"].toString();
    if(type == "chat_message"){
        qDebug() << "[ClientController] Message Recevied!";
        emit messageReceived(doc, false);
    }
    else if(type == "system_message")
    {
        emit messageReceived(doc, true);
    }
}

void ClientController::sendChatMessage(const QJsonDocument &msg, bool isSystem)
{
    if(socket) {
        QByteArray payload = msg.toJson(QJsonDocument::Compact);
        payload.append('\n');
        socket->write(payload);
        qDebug() << "[ClientController] Message sent to server";
    }
}

void ClientController::broadcast(const QJsonDocument &msg, bool isSystem)
{
    qWarning() << "broadcast called on client!!";
}




