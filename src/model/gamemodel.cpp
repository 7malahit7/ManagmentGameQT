#include "gameModel.h"

GameModel::GameModel()
    : m_localPlayer("Player", false, 0),
    m_state(GameState::NotStarted)
{
}

PlayerModel& GameModel::localPlayer()
{
    return m_localPlayer;
}

const PlayerModel& GameModel::localPlayer() const
{
    return m_localPlayer;
}

const QVector<PlayerModel>& GameModel::players() const
{
    return m_players;
}

void GameModel::setPlayers(const QVector<PlayerModel>& players)
{
    m_players = players;
}

void GameModel::updatePlayer(const PlayerModel& player)
{
    for (auto& p : m_players) {
        if (p.getId() == player.getId()) {
            p = player;
            return;
        }
    }
    m_players.append(player);
}

GameState GameModel::state() const
{
    return m_state;
}

void GameModel::setState(GameState state)
{
    m_state = state;
}
