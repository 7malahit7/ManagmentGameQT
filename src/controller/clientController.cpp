#include "clientController.h"

#include <QDebug>
#include <QJsonObject>
#include <QJsonArray>
#include <QTcpSocket>

ClientController::ClientController(
    PlayerModel* player,
    const QString& host,
    QObject* parent
    )
    : NetworkController(player, parent),
    host{host}
{
}


void ClientController::connectToServer()
{
    if (socket)
    {
        qDebug() << "[ClientController] connectToServer called twice, ignoring";
        return;
    }

    socket = new QTcpSocket(this);

    if (host.isEmpty())
        host = "127.0.0.1";

    qDebug() << "[ClientController] Trying to connect to host" << host;

    connect(socket, &QTcpSocket::connected, this, [this]() {
        qDebug() << "[ClientController] Connected to server!";

        connect(socket, &QTcpSocket::readyRead,
                this, &ClientController::onDataReceived);

        sendNameToServer();
        emit connected();
    });

    connect(socket, &QTcpSocket::errorOccurred,
            this, [](QAbstractSocket::SocketError e){
                qDebug() << "[ClientController] Socket error:" << e;
            });

    socket->connectToHost(host, 7777);
}

void ClientController::sendChatMessage(
    const QJsonDocument& msg,
    MessageKind
    )
{
    if (!socket)
        return;

    socket->write(messageToSendingByteArray(msg));
    qDebug() << "[ClientController] Message sent to server";
}

void ClientController::broadcast(
    const QJsonDocument&,
    MessageKind
    )
{
    qWarning() << "[ClientController] broadcast called on client!!";
}

void ClientController::sendReady(const QJsonDocument& doc)
{
    if (!socket)
        return;

    socket->write(messageToSendingByteArray(doc));
    qDebug() << "[ClientController] READY sent to server";
}

void ClientController::sendNameToServer()
{
    QJsonObject obj;
    obj["type"] = "new_client";
    obj["name"] = m_player->getName();

    socket->write(messageToSendingByteArray(obj));
}

void ClientController::onDataReceived()
{
    if (!socket)
        return;

    m_buffer += socket->readAll();
    processIncomingBuffer();
}

void ClientController::processIncomingBuffer()
{
    while (true)
    {
        int idx = m_buffer.indexOf('\n');
        if (idx == -1)
            break;

        QByteArray oneMessage = m_buffer.left(idx);
        m_buffer.remove(0, idx + 1);

        QJsonDocument doc = QJsonDocument::fromJson(oneMessage);
        if (!doc.isObject())
            continue;

        QJsonObject root = doc.object();
        QString type = root["type"].toString();

        if (type == "players_info")
        {
            handlePlayersInfo(root);
        }
        else if (type == "chat_message")
        {
            emit sendMessageToChatController(doc, MessageKind::UserMessage);
        }
        else if (type == "system_message")
        {
            emit sendMessageToChatController(doc, MessageKind::SystemMessage);
        }
        else if (type == "assign_id")
        {
            m_player->setId(root["id"].toInt());
        }
    }
}

void ClientController::handlePlayersInfo(const QJsonObject& root)
{
    playersInfo.clear();

    const QJsonArray arr = root["players"].toArray();
    for (const auto& v : arr)
    {
        PlayerModel model = PlayerModel::fromJson(v.toObject());

        if (model.getId() == m_player->getId() && m_player->getId() != 0)
        {
            *m_player = model;
        }

        playersInfo.append(model);
    }

    emit updatePlayers(playersInfo);
}
