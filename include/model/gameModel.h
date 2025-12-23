#pragma once

#include <QObject>
#include <QVector>
#include <QtGlobal>
#include <QRandomGenerator>
#include <algorithm>

#include "playerModel.h"


enum class GameState
{
    //ИНИЦИАЛИЗАЦИЯ
    NotStarted,
    SetupPlayers,
    MonthStart,

    //МЕСЯЧНЫЙ ЦИКЛ
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

    //ЗАВЕРШЕНИЕ
    GameOver
};

struct ProductionDecision
{
    int playerId = -1;
    int esmToProcess = 0;
};

struct MarketState
{
    int level = 3;        // уровень рынка (1–5) // старт с 3
    int esmSupply = 0;    // сколько ЕСМ продаёт банк
    int esmMinPrice = 0;  // минимальная цена ЕСМ
    int egpDemand = 0;    // сколько ЕГП покупает банк
    int egpMaxPrice = 0;  // максимальная цена ЕГП
};

struct EsmAllocation
{
    int playerId = -1;
    int allocated = 0;
    int price = 0;
};

struct EsmBid
{
    int playerId = 0;
    int amount = 0;
    int price = 0;
};

struct EgpBid
{
    int playerId = -1;
    int amount = 0;
    int price = 0;
};

struct EgpAllocation
{
    int playerId = -1;
    int allocated = 0;
    int price = 0;
};

class GameModel : public QObject
{
    Q_OBJECT

signals:
    void monthChanged(int month);
    void stateChanged(GameState state);
    void marketChanged(const MarketState& market);
    void esmBidsChanged(const QVector<EsmBid>& bids);
    void productionDecisionsChanged(const QVector<ProductionDecision>& decisions);
    void egpBidsChanged(const QVector<EgpBid>& bids);


public:
    explicit GameModel(QObject* parent = nullptr);

    void startGame();

    void advanceState();
    void advanceMonth();

    int month() const;
    void setMonth(int month);

    GameState state() const;
    void setState(GameState state);

    PlayerModel& localPlayer();
    const PlayerModel& localPlayer() const;

    const QVector<PlayerModel>& players() const;
    void setPlayers(const QVector<PlayerModel>& players);
    void updatePlayer(const PlayerModel& player);

    const MarketState& market() const;
    void setMarket(const MarketState& market);

    void recalcMarket(int alivePlayers);
    void advanceMarketLevel();

    const QVector<EsmBid>& esmBids() const;

    void clearEsmBids();
    void addEsmBid(const EsmBid& bid);

    QVector<EsmAllocation> allocateEsm() const;
    void applyEsmAllocations(const QVector<EsmAllocation>& allocs);

    void checkBankruptcies();

    void addProductionDecision(const ProductionDecision& decision);
    const QVector<ProductionDecision>& productionDecisions() const;
    void clearProductionDecisions();

    void limitProductionByFactories();

    void executeProduction();

    void addEgpBid(const EgpBid& bid);
    const QVector<EgpBid>& egpBids() const;
    void clearEgpBids();

    QVector<EgpAllocation> allocateEgp();

    void applyEgpAllocations(const QVector<EgpAllocation>& allocations);

    void applyLoanInterest();

    void repayDueLoans();

    bool takeLoan(int playerId, int amount);

    void setPlayerReady(int playerId);
    void tryAdvancePhase();

private:
    int m_month;
    GameState m_state;

    PlayerModel m_localPlayer;
    QVector<PlayerModel> m_players;

    MarketState m_market;

    QVector<EsmBid> m_esmBids;

    QVector<ProductionDecision> m_productionDecisions;

    QVector<EgpBid> m_egpBids;

    void enterState(GameState state);

    void payFixedCosts();
};
