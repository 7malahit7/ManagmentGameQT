#include "clientController.h"
#include <QDebug>
#include <QJsonObject>
#include <QJsonArray>
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
    socket->write(messageToSendingByteArray(obj));
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
        emit sendMessageToChatController(doc, MessageKind::UserMessage);
    }
    else if(type == "system_message")
    {
        emit sendMessageToChatController(doc, MessageKind::SystemMessage);
    }
    else if(type == "assign_id")
    {
        localPlayer->setId(doc.object()["id"].toInt());
    }
    else if(type == "players_info")
    {
        playersInfo.clear();
        const QJsonArray playersArray = doc.object().value("players").toArray();
        for (const auto& playerValue : playersArray)
        {
            const PlayerModel model = PlayerModel::fromJson(playerValue.toObject());
            if (model.getName() == localPlayer->getName())
            {
                localPlayer->setId(model.getId());
                localPlayer->setLevel(model.getLevel());
                localPlayer->setBalance(model.getBalance());
                localPlayer->setEgp(model.getEgp());
                localPlayer->setEsm(model.getEsm());
                localPlayer->setStatus(model.getStatus());
            }
            playersInfo.append(model);
        }

        emit updatePlayers(playersInfo);
    }
}
void ClientController::sendChatMessage(const QJsonDocument &msg, MessageKind isSystem)
{
    if(socket) {
        socket->write(messageToSendingByteArray(msg));
        qDebug() << "[ClientController] Message sent to server";
    }
}

void ClientController::broadcast(const QJsonDocument &msg, MessageKind isSystem)
{
    qWarning() << "broadcast called on client!!";
}






