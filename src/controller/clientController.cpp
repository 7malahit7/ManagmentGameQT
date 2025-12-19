#include "clientController.h"
#include <QDebug>
#include <QJsonObject>
#include <QJsonArray>
ClientController::ClientController(PlayerModel* player, const QString &host, QObject *parent)
    : NetworkController(player, parent), host{host}
{}

void ClientController::connectToServer()
{
    if (socket) {
        qDebug() << "[ClientController] connectToServer called twice, ignoring";
        return;
    }

    socket = new QTcpSocket(this);

    if (host.isEmpty())
        host = "127.1";

    qDebug() << "[ClientController] Trying to connect to host" << host;

    connect(socket, &QTcpSocket::connected, this, [this]() {
        qDebug() << "[ClientController] Connected to server!";

        connect(socket, &QTcpSocket::readyRead,
                this, &ClientController::onDataReceived);

        sendNameToServer();   // ← здесь и ТОЛЬКО здесь
        emit connected();
    });

    connect(socket, &QTcpSocket::errorOccurred,
            this, [](QAbstractSocket::SocketError e){
                qDebug() << "[ClientController] Socket error:" << e;
            });

    socket->connectToHost(host, 7777);
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
    QTcpSocket* socket = qobject_cast<QTcpSocket*>(sender());
    if (!socket) return;

    m_buffer += socket->readAll();
    processIncomingBuffer();
}
void ClientController::processIncomingBuffer()
{
    while (true) {
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

        if (type == "players_info") {
            handlePlayersInfo(root);
        }
        else if (type == "chat_message") {
            emit sendMessageToChatController(doc, MessageKind::UserMessage);
        }
        else if (type == "system_message") {
            emit sendMessageToChatController(doc, MessageKind::SystemMessage);
        }
        else if (type == "assign_id") {
            localPlayer->setId(root["id"].toInt());
        }
    }
}
void ClientController::handlePlayersInfo(const QJsonObject& root)
{
    playersInfo.clear();

    const QJsonArray arr = root["players"].toArray();
    for (const auto& v : arr) {
        PlayerModel model = PlayerModel::fromJson(v.toObject());
        if (model.getName() == localPlayer->getName()) {
            *localPlayer = model;
        }
        playersInfo.append(model);
    }

    emit updatePlayers(playersInfo);
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






