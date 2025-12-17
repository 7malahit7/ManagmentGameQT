#include "serverController.h"
#include<QJsonObject>
ServerController::ServerController(QObject *parent)
    : NetworkController(parent)
{
    startServer();
}
void ServerController::startServer()
{
    server = new QTcpServer(this);
    if(!server->listen(QHostAddress::Any, 7777)) {
        qWarning() << "[ServerController] Server failed:" << server->serverError();
        return;
    }
    connect(server, &QTcpServer::newConnection, this, &ServerController::onNewConnection);
    qDebug() << "[ServerController] Server started!";
}

void ServerController::sendNewClientToChat()
{

}


void ServerController::onNewConnection()
{
    QTcpSocket* clientSocket = server->nextPendingConnection();
    clients.append(clientSocket);

    qDebug() << "[ServerController] Client connected! Total clients:" << clients.size();

    connect(clientSocket, &QTcpSocket::readyRead, this, &ServerController::onDataReceived);
}

void ServerController::onDataReceived()
{
    QTcpSocket* senderSocket = qobject_cast<QTcpSocket*>(sender());
    if(!senderSocket) return;

    QJsonDocument doc = QJsonDocument::fromJson(senderSocket->readAll());

    QString type = doc.object()["type"].toString();
    if(type == "chat_message"){
        emit messageReceived(doc);
        for(QTcpSocket* client : clients){
            if(client != senderSocket){
                client->write(doc.toJson());
                qDebug() << "[ServerController] Broadcasted message to client";
            }
        }
    }
}
void ServerController::sendChatMessageToServerOrBroadcast(const QJsonDocument &msg)
{
    emit messageReceived(msg);
    qDebug() << "[ServerController] Broadcasting message";
    for(QTcpSocket* client : clients){
        qDebug() << "[ServerController] Making payload!";
        QByteArray payload = msg.toJson(QJsonDocument::Compact);
        payload.append('\n');
        client->write(payload);
    }

}
