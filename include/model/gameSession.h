#ifndef GAMESESSION_H
#define GAMESESSION_H
#include "playerModel.h"
#include <QObject>
#include <QString>

class PlayerModel;         // форвард
class NetworkController;   // форвард

class GameSession : public QObject
{
    Q_OBJECT
public:
    explicit GameSession(QObject* parent = nullptr);

    // Методы запуска игры
    void startGame(bool serverMode);
    void startNetworkGame(const QString& host);

    // Геттеры
    PlayerModel* getLocalPlayer() const { return localPlayer; }
    NetworkController* getNetwork() const { return network; }

signals:
    void gameStarted();
    void networkGameStarted(const QString& host);

private:
    PlayerModel* localPlayer;
    NetworkController* network;
};

#endif // GAMESESSION_H
