#include "serverController.h"

#include <QJsonArray>
#include <QJsonObject>
#include <QDebug>

ServerController::ServerController(PlayerModel* player, QObject* parent)
    : NetworkController(player, parent)
{
    nextId = 1;

    m_player->setId(1);
    m_player->setIsServer(true);

    playersInfo.append(*m_player);

    startServer();
}

void ServerController::startServer()
{
    server = new QTcpServer(this);

    if (!server->listen(QHostAddress::Any, 7777))
    {
        qWarning() << "[ServerController] Server failed:"
                   << server->serverError();
        return;
    }

    connect(server, &QTcpServer::newConnection,
            this, &ServerController::onNewConnection);

    qDebug() << "[ServerController] Server started!";
}

void ServerController::onNewConnection()
{
    QTcpSocket* clientSocket = server->nextPendingConnection();
    if (!clientSocket)
        return;

    quint8 id = ++nextId;

    PlayerModel* newPlayer = new PlayerModel("", false, id);
    socketToPlayer[clientSocket] = newPlayer;
    m_buffers[clientSocket] = QByteArray();

    qDebug() << "[ServerController] Client connected! Assigned id =" << id;

    connect(clientSocket, &QTcpSocket::readyRead,
            this, &ServerController::onDataReceived);

    connect(clientSocket, &QTcpSocket::disconnected,
            this, [this, clientSocket]() {
                m_buffers.remove(clientSocket);
                delete socketToPlayer.take(clientSocket);
                clientSocket->deleteLater();
            });
    sendAssignId(clientSocket, id);
}

void ServerController::onDataReceived()
{
    QTcpSocket* socket = qobject_cast<QTcpSocket*>(sender());
    if (!socket)
        return;

    m_buffers[socket] += socket->readAll();
    processIncomingBuffer(socket);
}

void ServerController::processIncomingBuffer(QTcpSocket* socket)
{
    QByteArray& buffer = m_buffers[socket];

    while (true)
    {
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

        if (type == "chat_message")
        {
            broadcast(doc, MessageKind::UserMessage);
        }
        else if (type == "system_message")
        {
            broadcast(doc, MessageKind::SystemMessage);
        }
        else if (type == "new_client")
        {
            handleNewClient(socket, root);
        }
        else if (type == "player_ready")
        {
            PlayerModel* p = socketToPlayer.value(socket, nullptr);
            if (!p)
                return;

            qDebug() << "[ServerController] player READY received id =" << p->getId();

            emit playerReadyReceived(p->getId());
        }
    }
}


void ServerController::handleNewClient(
    QTcpSocket* socket,
    const QJsonObject& root
    )
{
    PlayerModel* player = socketToPlayer.value(socket, nullptr);
    if (!player)
        return;

    QString name = root["name"].toString();
    player->setName(name);

    playersInfo.append(*player);

    broadcast(sendNewClientToChat(name), MessageKind::SystemMessage);

    emit updatePlayers(playersInfo);
    broadcastPlayersInfo();
}

void ServerController::broadcast(
    const QJsonDocument& msg,
    MessageKind kind
    )
{
    sendChatMessage(msg, kind);

    for (QTcpSocket* client : socketToPlayer.keys())
    {
        client->write(messageToSendingByteArray(msg));
    }
}

void ServerController::sendChatMessage(
    const QJsonDocument& msg,
    MessageKind kind
    )
{
    emit sendMessageToChatController(msg, kind);
}

void ServerController::sendReady(const QJsonDocument& doc)
{
    Q_UNUSED(doc) // пока что не шлём себя готовым

}


void ServerController::emitInitialPlayers()
{
    emit updatePlayers(playersInfo);
    broadcastPlayersInfo();
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

    for (QTcpSocket* client : socketToPlayer.keys())
    {
        client->write(messageToSendingByteArray(doc));
    }
}

QJsonDocument ServerController::sendNewClientToChat(const QString& name)
{
    QJsonObject obj;
    obj["type"] = "system_message";
    obj["text"] =
        QString("<span style=\"color:yellow;\">Подключился %1</span>")
            .arg(name);

    return QJsonDocument(obj);
}

void ServerController::sendAssignId(QTcpSocket* socket, int id)
{
    QJsonObject obj;
    obj["type"] = "assign_id";
    obj["id"] = id;

    socket->write(messageToSendingByteArray(QJsonDocument(obj)));
}
