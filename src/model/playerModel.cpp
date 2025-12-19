#include "playerModel.h"
#include <QJsonObject>

PlayerModel::PlayerModel(const QString& name,
                         bool isServer,
                         quint8 id,
                         int level,
                         int balance,
                         int egp,
                         int esm,
                         const QString &status)
    : name(name),
    isServer(isServer),
    id(id),
    level(level),
    balance(balance),
    egp(egp),
    esm(esm),
    status(status)
{}

QString PlayerModel::getName() const { return name; }
bool PlayerModel::getIsServer() const { return isServer; }
quint8 PlayerModel::getId() const { return id; }
int PlayerModel::getLevel() const { return level; }
int PlayerModel::getBalance() const { return balance; }
int PlayerModel::getEgp() const { return egp; }
int PlayerModel::getEsm() const { return esm; }
QString PlayerModel::getStatus() const { return status; }

void PlayerModel::setName(const QString &name) { this->name = name; }
void PlayerModel::setIsServer(bool newIsServer) { isServer = newIsServer; }
void PlayerModel::setId(quint8 newId) { id = newId; }
void PlayerModel::setLevel(int newLevel) { level = newLevel; }
void PlayerModel::setBalance(int newBalance) { balance = newBalance; }
void PlayerModel::setEgp(int newEgp) { egp = newEgp; }
void PlayerModel::setEsm(int newEsm) { esm = newEsm; }
void PlayerModel::setStatus(const QString &newStatus) { status = newStatus; }

QJsonDocument PlayerModel::toJson() const
{
    return QJsonDocument(toJsonObject());
}

QJsonObject PlayerModel::toJsonObject() const
{
    QJsonObject jsonObject;
    jsonObject["id"] = static_cast<int>(id);
    jsonObject["name"] = name;
    jsonObject["isServer"] = isServer;
    jsonObject["level"] = level;
    jsonObject["balance"] = balance;
    jsonObject["egp"] = egp;
    jsonObject["esm"] = esm;
    jsonObject["status"] = status;
    return jsonObject;
}

PlayerModel PlayerModel::fromJson(const QJsonObject &obj)
{
    return PlayerModel(obj.value("name").toString(QStringLiteral("404")),
                       obj.value("isServer").toBool(),
                       static_cast<quint8>(obj.value("id").toInt(99)),
                       obj.value("level").toInt(404),
                       obj.value("balance").toInt(404),
                       obj.value("egp").toInt(404),
                       obj.value("esm").toInt(404),
                       obj.value("status").toString(QStringLiteral("404...")));
}
