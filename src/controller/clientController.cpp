#include "clientController.h"
#include <QDebug>
#include <QJsonObject>
ClientController::ClientController(const QString &host, QObject *parent)
    : NetworkController(parent), host{host}
{
    connectToServer();
}

void ClientController::connectToServer()
{

    // if(socket && (socket->state() == QAbstractSocket::ConnectedState ||
    //                socket->state() == QAbstractSocket::ConnectingState)) {
    //     qDebug() << "[NetworkController] Already connecting/connected";
    //     return; // не создаём новый сокет
    // }

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
}


void ClientController::onDataReceived()
{
    qDebug() << "[ClientController] Data Receied!";
    QTcpSocket* senderSocket = qobject_cast<QTcpSocket*>(sender());
    if(!senderSocket) return;

    QJsonDocument doc = QJsonDocument::fromJson(senderSocket->readAll());
     QString type = doc.object()["type"].toString();
    if(type == "chat_message"){
        qDebug() << "[ClientController] Message Recevied!";
        emit messageReceived(doc);
    }
}

void ClientController::sendChatMessage(const QJsonDocument &msg)
{
    emit messageReceived(msg);
    if(socket) {
        QByteArray payload = msg.toJson(QJsonDocument::Compact);
        payload.append('\n');
        socket->write(payload);
        qDebug() << "[ClientController] Message sent to server";
    }
}

void ClientController::broadcast(const QJsonDocument &msg)
{
    qWarning() << "broadcast called on client!!";
}




