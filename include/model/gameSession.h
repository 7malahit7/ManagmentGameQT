#ifndef GAMESESSION_H
#define GAMESESSION_H
#include "playerModel.h"
#include <QObject>
#include <QString>

class PlayerModel;
class NetworkController;

class GameSession : public QObject
{
    Q_OBJECT
public:
    explicit GameSession(QObject* parent = nullptr);

    void startGameAsHost(bool serverMode);
    void connectToGame(const QString& host);

    PlayerModel* getLocalPlayer() const { return localPlayer; }
    NetworkController* getNetwork() const { return network; }

signals:
    void gameStarted();
    void networkGameStarted(const QString& host);

private:
    PlayerModel* localPlayer;
    NetworkController* network;
};

#endif
