#ifndef PLAYERMODEL_H
#define PLAYERMODEL_H

#include <QString>
#include <QJsonDocument>


class PlayerModel
{
public:
    explicit PlayerModel(const QString& name, bool isServer, quint8 id);

    QString getName() const;
    bool getIsServer() const;
    quint8 getId() const;

    void setName(const QString &name);
    void setIsServer(bool newIsServer);

    QJsonDocument toJson() const;
    void assignFromJsonId(const QString &jsonString);

    void setId(quint8 newId);

private:
    QString name;
    bool isServer;
    quint8 id;
};

#endif
