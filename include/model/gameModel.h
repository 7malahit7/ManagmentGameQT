#pragma once

#include <QVector>
#include "playerModel.h"

// Полное состояние игры согласно правилам
enum class GameState
{
    // ===== ИНИЦИАЛИЗАЦИЯ =====
    NotStarted,
    SetupPlayers,
    MonthStart,

    // ===== МЕСЯЧНЫЙ ЦИКЛ =====
    PayFixedCosts,
    MarketDetermination,

    RawMaterialBidding,
    RawMaterialAllocation,

    ProductionDecision,
    ProductionExecution,

    ProductBidding,
    ProductAllocation,

    LoanInterestPayment,
    LoanRepayment,
    LoanTaking,

    ConstructionDecision,
    ConstructionProgress,

    MonthEnd,

    // ===== ЗАВЕРШЕНИЕ =====
    GameOver
};

class GameModel
{
public:
    GameModel();

    // Local player
    PlayerModel& localPlayer();
    const PlayerModel& localPlayer() const;

    // Players
    const QVector<PlayerModel>& players() const;
    void setPlayers(const QVector<PlayerModel>& players);
    void updatePlayer(const PlayerModel& player);

    // Game state
    GameState state() const;
    void setState(GameState state);

private:
    PlayerModel m_localPlayer;
    QVector<PlayerModel> m_players;
    GameState m_state;
};
