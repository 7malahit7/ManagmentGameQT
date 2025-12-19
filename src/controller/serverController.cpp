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
    QTcpSocket* socket = qobject_cast<QTcpSocket*>(sender());
    if (!socket) return;

    m_buffers[socket] += socket->readAll();
    processIncomingBuffer(socket);
}
void ServerController::processIncomingBuffer(QTcpSocket* socket)
{
    QByteArray& buffer = m_buffers[socket];

    while (true) {
        int idx = buffer.indexOf('\n');
        if (idx == -1)
            break;

        QByteArray oneMessage = buffer.left(idx);
        buffer.remove(0, idx + 1);

        QJsonDocument doc = QJsonDocument::fromJson(oneMessage);
        if (!doc.isObject())
            continue;

        QJsonObject root = doc.object();
        QString type = root["type"].toString();

        if (type == "chat_message") {
            broadcast(doc, MessageKind::UserMessage);
        }
        else if (type == "new_client") {
            handleNewClient(socket, root);
        }
    }
}
void ServerController::handleNewClient(QTcpSocket* socket, const QJsonObject& root)
{
    QString name = root["name"].toString();
    socketToPlayer[socket]->setName(name);

    broadcast(sendNewClientToChat(name), MessageKind::SystemMessage);

    playersInfo.append(PlayerModel(name, false, nextId));
    emit updatePlayers(playersInfo);
    broadcastPlayersInfo();
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
    socketToPlayer[clientSocket] = new PlayerModel("", false, id);
    m_buffers[clientSocket] = QByteArray();   // ← ВАЖНО

    qDebug() << "[ServerController] Client connected! Assigned id =" << id;

    connect(clientSocket, &QTcpSocket::readyRead,
            this, &ServerController::onDataReceived);

    connect(clientSocket, &QTcpSocket::disconnected, this, [this, clientSocket]() {
        m_buffers.remove(clientSocket);
        socketToPlayer.remove(clientSocket);
        clientSocket->deleteLater();
    });
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






