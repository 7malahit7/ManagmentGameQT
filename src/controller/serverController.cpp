#include "serverController.h"
#include<QJsonObject>
ServerController::ServerController(PlayerModel* player, QObject *parent)
    : NetworkController(player, parent)
{
    nextId = 1;
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


void ServerController::onNewConnection()
{
    QTcpSocket* clientSocket = server->nextPendingConnection();


    quint8 id = ++nextId;
    socketToPlayer[clientSocket] = new PlayerModel("",false,id);
    qDebug() << "[ServerController] Client connected! Total clients:" << socketToPlayer.size();
    qDebug() << "[ServerController] Client connected! Assigned id =" << id;

    connect(clientSocket, &QTcpSocket::readyRead, this, &ServerController::onDataReceived);
}
QJsonDocument ServerController::sendNewClientToChat(const QString& message)
{
    QJsonObject obj;
    obj["type"] = "system_message";
    obj["text"] = message;
    QJsonDocument doc(obj);
    return doc;
}

void ServerController::broadcast(const QJsonDocument &msg, bool isSystem)
{
    sendChatMessage(msg, isSystem);
    qDebug() << "[ServerController] Broadcasting message";
    for(QTcpSocket* client : socketToPlayer.keys()){
        qDebug() << "[ServerController] Making payload!";
        QByteArray payload = msg.toJson(QJsonDocument::Compact);
        payload.append('\n');
        client->write(payload);
    }
}

void ServerController::sendChatMessage(const QJsonDocument &msg, bool isSystem)
{
    emit messageReceived(msg, isSystem);
}




void ServerController::onDataReceived()
{
    QTcpSocket* senderSocket = qobject_cast<QTcpSocket*>(sender());
    if(!senderSocket) return;

    QJsonDocument doc = QJsonDocument::fromJson(senderSocket->readAll());

    QString type = doc.object()["type"].toString();
    if(type == "chat_message"){
        broadcast(doc,false);
    }
    else if(type == "new_client")
    {
        QString name = doc.object()["name"].toString();
        socketToPlayer[senderSocket]->setName(name);
        broadcast(sendNewClientToChat(QString("<span style=\"color:green;\">Подключился %1</span>").arg(name)), true);
    }
}




