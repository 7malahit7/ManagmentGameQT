#pragma once

#include <QString>
#include <QJsonDocument>
#include <QJsonObject>

class PlayerModel
{
public:
    explicit PlayerModel(const QString& name,
                         bool isServer,
                         quint8 id,
                         int level = 1,
                         int balance = 10000,
                         int egp = 10,
                         int esm = 10,
                         const QString& status = QStringLiteral("Ожидает..."));

    QString getName() const;
    bool getIsServer() const;
    quint8 getId() const;
    int getLevel() const;
    int getBalance() const;
    int getEgp() const;
    int getEsm() const;
    QString getStatus() const;

    void setName(const QString &name);
    void setIsServer(bool newIsServer);
    void setId(quint8 newId);
    void setLevel(int newLevel);
    void setBalance(int newBalance);
    void setEgp(int newEgp);
    void setEsm(int newEsm);
    void setStatus(const QString &newStatus);

    QJsonDocument toJson() const;
    QJsonObject toJsonObject() const;
    static PlayerModel fromJson(const QJsonObject &obj);


private:
    QString name;
    bool isServer;
    quint8 id;
    int level;
    int balance;
    int egp;
    int esm;
    QString status;
};


