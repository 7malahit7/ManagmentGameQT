#include "serverController.h"

#include <QJsonArray>
#include <QJsonObject>
ServerController::ServerController(PlayerModel* player, QObject *parent)
    : NetworkController(player, parent)
{
    nextId = 1;
    playersInfo.append(PlayerModel(localPlayer->getName(), true, localPlayer->getId()));

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

void ServerController::onDataReceived()
{
    QTcpSocket* senderSocket = qobject_cast<QTcpSocket*>(sender());
    if(!senderSocket) return;

    QJsonDocument doc = QJsonDocument::fromJson(senderSocket->readAll());

    QString type = doc.object()["type"].toString();
    if(type == "chat_message"){
        broadcast(doc,MessageKind::UserMessage);
    }
    else if(type == "new_client")
    {
        QString name = doc.object()["name"].toString();
        socketToPlayer[senderSocket]->setName(name);

        broadcast(sendNewClientToChat(name), MessageKind::SystemMessage);

        playersInfo.append(PlayerModel(name, false ,nextId));
        emit updatePlayers(playersInfo);

        broadcastPlayersInfo();
    }
}

void ServerController::sendChatMessage(const QJsonDocument &msg, MessageKind isSystem)
{
    emit sendMessageToChatController(msg, isSystem);
}

void ServerController::emitInitialPlayers()
{
    emit updatePlayers(playersInfo);

    broadcastPlayersInfo();
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


void ServerController::broadcast(const QJsonDocument &msg, MessageKind isSystem)
{
    sendChatMessage(msg, isSystem);
    qDebug() << "[ServerController] Broadcasting message";
    for(QTcpSocket* client : socketToPlayer.keys()){
        client->write(messageToSendingByteArray(msg));
    }
}
void ServerController::broadcastPlayersInfo()
{
    QJsonArray playersArray;
    for (const auto& player : playersInfo)
    {
        playersArray.append(player.toJsonObject());
    }

    QJsonObject obj;
    obj["type"] = "players_info";
    obj["players"] = playersArray;

    QJsonDocument doc(obj);
    qDebug() << "[ServerController] Broadcasting players info";

    for(QTcpSocket* client : socketToPlayer.keys()){
        client->write(messageToSendingByteArray(doc));
    }
}

QJsonDocument ServerController::sendNewClientToChat(const QString& name)
{
    QJsonObject obj;
    obj["type"] = "system_message";
    obj["text"] = QString("<span style=\"color:yellow;\">Подключился %1</span>").arg(name);
    QJsonDocument doc(obj);
    return doc;
}






