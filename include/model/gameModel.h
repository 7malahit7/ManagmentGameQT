#pragma once

#include <QVector>
#include "playerModel.h"

enum class GameState
{
    // ===== ИНИЦИАЛИЗАЦИЯ =====
    NotStarted,            // игра ещё не началась
    SetupPlayers,          // начальная раздача ресурсов
    MonthStart,            // начало месяца, смена старшинства

    // ===== МЕСЯЧНЫЙ ЦИКЛ =====
    PayFixedCosts,         // 1. постоянные издержки
    MarketDetermination,   // 2. банк определяет рынок

    RawMaterialBidding,    // 3. заявки на ЕСМ
    RawMaterialAllocation, // распределение ЕСМ

    ProductionDecision,    // 4. объявление производства
    ProductionExecution,   // переработка ЕСМ → ЕГП

    ProductBidding,        // 5. заявки на продажу ЕГП
    ProductAllocation,     // продажа продукции

    LoanInterestPayment,   // 6. выплата процентов
    LoanRepayment,         // 7. погашение ссуд
    LoanTaking,            // 8. получение ссуд

    ConstructionDecision,  // 9. заявки на строительство
    ConstructionProgress,  // ход строительства

    MonthEnd,              // конец месяца

    // ===== ЗАВЕРШЕНИЕ =====
    GameOver               // конец игры
};

class GameModel
{
public:
    GameModel();

    PlayerModel& setLocalPlayer();
    const PlayerModel& getLocalPlayer() const;

    const QVector<PlayerModel>& getPlayers() const;
    void setPlayers(const QVector<PlayerModel>& players);
    void updatePlayer(const PlayerModel& player);

    GameState getState() const;
    void setState(GameState state);

private:
    PlayerModel localPlayer;
    QVector<PlayerModel> players;
    GameState state;
};
