#include "playerModel.h"
#include <QJsonObject>

PlayerModel::PlayerModel(const QString& name, bool isServer, quint8 id)
    : name(name), isServer(isServer), id(id)
{
}

QString PlayerModel::getName() const { return name; }
bool PlayerModel::getIsServer() const { return isServer; }
quint8 PlayerModel::getId() const { return id; }

void PlayerModel::setName(const QString &name) { this->name = name; }
void PlayerModel::setIsServer(bool newIsServer) { isServer = newIsServer; }
void PlayerModel::setId(quint8 newId) { id = newId; }

QJsonDocument PlayerModel::toJson() const
{
    QJsonObject jsonObject;
    jsonObject["id"] = id;
    jsonObject["name"] = name;
    jsonObject["isServer"] = isServer;
    return QJsonDocument(jsonObject);
}

void PlayerModel::assignFromJsonId(const QString &jsonString)
{
    QJsonDocument doc = QJsonDocument::fromJson(jsonString.toUtf8());
    QJsonObject obj = doc.object();
    this->id = static_cast<quint8>(obj["id"].toInt());
}


