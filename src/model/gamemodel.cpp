#include "gameModel.h"

struct MarketLevelInfo
{
    double esmMultiplier;
    int esmMinPrice;
    double egpMultiplier;
    int egpMaxPrice;
};

//6.1 из книги
static const MarketLevelInfo MARKET_LEVELS[5] =
    {
        {1.0, 800, 3.0, 6500}, // уровень 1
        {1.5, 650, 2.5, 6000}, // уровень 2
        {2.0, 500, 2.0, 5500}, // уровень 3
        {2.5, 400, 1.5, 5000}, // уровень 4
        {3.0, 300, 1.0, 4500}  // уровень 5
};
//6.2 из книги
static const double MARKET_TRANSITION[5][5] =
    {
        {1.0/3, 1.0/3, 1.0/6, 1.0/12, 1.0/12},  // 1
        {1.0/6, 1.0/3, 1.0/3, 1.0/6, 0.0},      // 2
        {0.0,   1.0/6, 1.0/3, 1.0/3, 1.0/6},    // 3
        {0.0,   0.0,   1.0/6, 1.0/3, 1.0/2},    // 4
        {0.0,   0.0,   0.0,   1.0/3, 2.0/3}     // 5
};

GameModel::GameModel(QObject* parent)
    : QObject(parent),
    m_month(0),
    m_state(GameState::NotStarted),
    m_localPlayer("Player", false, 0)
{
}

void GameModel::startGame()
{
    m_month = 1;
    setState(GameState::PayFixedCosts);

    recalcMarket(m_players.size());

    advanceState();
    advanceState();
}

void GameModel::advanceState()
{
    switch (m_state)
    {
    case GameState::PayFixedCosts:
        m_state = GameState::MarketDetermination;
        break;

    case GameState::MarketDetermination:
        recalcMarket(m_players.size());
        m_state = GameState::RawMaterialBidding;
        break;

    case GameState::RawMaterialBidding:
        m_state = GameState::RawMaterialAllocation;
        break;

    case GameState::RawMaterialAllocation:
        m_state = GameState::ProductionDecision;
        break;

    case GameState::ProductionDecision:
        m_state = GameState::ProductionExecution;
        break;

    case GameState::ProductionExecution:
        m_state = GameState::ProductBidding;
        break;

    case GameState::ProductBidding:
        m_state = GameState::ProductAllocation;
        break;

    case GameState::ProductAllocation:
        m_state = GameState::LoanInterestPayment;
        break;

    case GameState::LoanInterestPayment:
        m_state = GameState::LoanRepayment;
        break;

    case GameState::LoanRepayment:
        m_state = GameState::LoanTaking;
        break;

    case GameState::LoanTaking:
        m_state = GameState::MonthEnd;
        break;

    case GameState::MonthEnd:
        m_month++;
        emit monthChanged(m_month);
        m_state = GameState::PayFixedCosts;
        break;

    default:
        return;
    }

    enterState(m_state);
    emit stateChanged(m_state);
}

void GameModel::advanceMonth()
{
    m_state = GameState::PayFixedCosts;
    emit stateChanged(m_state);
    enterState(m_state);

    while (m_state != GameState::MonthEnd)
    {
        advanceState();
    }

    advanceState();
}

void GameModel::setPlayerReady(int playerId)
{
    for (auto& p : m_players)
    {
        if (p.getId() == playerId)
        {
            if (p.isReady())
                return;

            p.setReady(true);
            qDebug() << "[MODEL] Player" << playerId << "is READY";
        }
    }

    tryAdvancePhase();
}
void GameModel::tryAdvancePhase()
{
    for (const auto& p : m_players)
    {
        if (!p.isBankrupt() && !p.isReady())
            return;
    }

    for (auto& p : m_players)
        p.setReady(false);

    advanceState();
    qDebug() << "[MODEL] All players ready, advancing phase";
}

int GameModel::month() const
{
    return m_month;
}

void GameModel::setMonth(int month)
{
    if (m_month == month)
        return;

    m_month = month;
    emit monthChanged(m_month);
}

GameState GameModel::state() const
{
    return m_state;
}

void GameModel::setState(GameState state)
{
    if (m_state == state)
        return;

    m_state = state;
    emit stateChanged(m_state);
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
    for (auto& p : m_players)
    {
        if (p.getId() == player.getId())
        {
            p = player;
            return;
        }
    }
}
const MarketState& GameModel::market() const
{
    return m_market;
}
void GameModel::setMarket(const MarketState& market)
{
    m_market = market;
    emit marketChanged(m_market);
}
void GameModel::recalcMarket(int alivePlayers)
{
    if (alivePlayers <= 0)
        return;

    MarketState market = m_market;

    const int levelIndex = qBound(1, market.level, 5) - 1;
    const auto& info = MARKET_LEVELS[levelIndex];

    market.esmSupply  = static_cast<int>(info.esmMultiplier * alivePlayers);
    market.esmMinPrice = info.esmMinPrice;

    market.egpDemand  = static_cast<int>(info.egpMultiplier * alivePlayers);
    market.egpMaxPrice = info.egpMaxPrice;

    setMarket(market);
}
void GameModel::advanceMarketLevel()
{
    const int currentLevel = qBound(1, m_market.level, 5);
    const int row = currentLevel - 1;

    const double r = QRandomGenerator::global()->generateDouble(); // [0;1] double

    double acc = 0.0;
    int nextLevel = currentLevel;

    for (int col = 0; col < 5; ++col)
    {
        acc += MARKET_TRANSITION[row][col];
        if (r <= acc)
        {
            nextLevel = col + 1;
            break;
        }
    }

    if (nextLevel != m_market.level)
    {
        m_market.level = nextLevel;
        recalcMarket(players().size());
    }
}

const QVector<EsmBid>& GameModel::esmBids() const
{
    return m_esmBids;
}
void GameModel::clearEsmBids()
{
    m_esmBids.clear();
    emit esmBidsChanged(m_esmBids);
}
void GameModel::addEsmBid(const EsmBid& bid)
{
    m_esmBids.push_back(bid);
    emit esmBidsChanged(m_esmBids);
}

QVector<EsmAllocation> GameModel::allocateEsm() const
{
    QVector<EsmAllocation> result;

    int remaining = m_market.esmSupply;

    QVector<EsmBid> bids = m_esmBids;

    std::sort(bids.begin(), bids.end(),
              [](const EsmBid& a, const EsmBid& b) {
                  if (a.price != b.price)
                      return a.price > b.price;

                  return a.playerId < b.playerId;
              });

    for (const auto& bid : bids)
    {
        if (remaining <= 0)
            break;

        const int take = qMin(bid.amount, remaining);

        EsmAllocation alloc;
        alloc.playerId = bid.playerId;
        alloc.allocated = take;
        alloc.price = bid.price;

        result.push_back(alloc);

        remaining -= take;
    }

    return result;
}

void GameModel::applyEsmAllocations(const QVector<EsmAllocation>& allocs)
{
    for (const auto& alloc : allocs)
    {
        for (auto& player : m_players)
        {
            if (player.getId() == alloc.playerId)
            {
                const int cost = alloc.allocated * alloc.price;

                player.setBalance(player.getBalance() - cost);
                player.setEsm(player.getEsm() + alloc.allocated);

                qDebug() << "[BANK] Player" << player.getId()
                         << "paid" << cost
                         << "new balance =" << player.getBalance()
                         << "ESM =" << player.getEsm();

                break;
            }
        }
    }
}

void GameModel::checkBankruptcies()
{
    for (auto& player : m_players)
    {
        if (!player.isBankrupt() && player.getBalance() < 0)
        {
            player.setBankrupt(true);

            qDebug() << "[BANK] Player" << player.getId()
                     << "is BANKRUPT and removed from game";
        }
    }
}

void GameModel::addProductionDecision(const ProductionDecision& decision)
{
    m_productionDecisions.push_back(decision);
    emit productionDecisionsChanged(m_productionDecisions);

    qDebug() << "[MODEL] Production decision added:"
             << "playerId =" << decision.playerId
             << "esm =" << decision.esmToProcess;

}

const QVector<ProductionDecision>& GameModel::productionDecisions() const
{
    return m_productionDecisions;
}

void GameModel::clearProductionDecisions()
{
    m_productionDecisions.clear();
}

void GameModel::limitProductionByFactories()
{
    for (auto& decision : m_productionDecisions)
    {
        for (const auto& player : m_players)
        {
            if (player.getId() == decision.playerId)
            {
                int maxCap = player.maxProductionCapacity();

                if (decision.esmToProcess > maxCap)
                {
                    qDebug() << "[BANK] Production limited:"
                             << "playerId =" << decision.playerId
                             << "requested =" << decision.esmToProcess
                             << "allowed =" << maxCap;

                    decision.esmToProcess = maxCap;
                }
                break;
            }
        }
    }

    emit productionDecisionsChanged(m_productionDecisions);
}


void GameModel::executeProduction()
{
    for (const auto& decision : m_productionDecisions)
    {
        for (auto& player : m_players)
        {
            if (player.getId() != decision.playerId)
                continue;

            int esm = decision.esmToProcess;
            if (esm <= 0)
                continue;

            if (player.getEsm() < esm)
            {
                qDebug() << "[BANK] Production failed (not enough ESM)"
                         << "playerId =" << player.getId();
                continue;
            }

            int cost = esm * 2000; // упрощённо

            player.setEsm(player.getEsm() - esm);
            player.setEgp(player.getEgp() + esm);
            player.setBalance(player.getBalance() - cost);

            qDebug() << "[BANK] Production executed:"
                     << "playerId =" << player.getId()
                     << "ESM used =" << esm
                     << "EGP produced =" << esm
                     << "cost =" << cost
                     << "balance =" << player.getBalance();
        }
    }
}
void GameModel::addEgpBid(const EgpBid& bid)
{
    m_egpBids.push_back(bid);
    emit egpBidsChanged(m_egpBids);
}

const QVector<EgpBid>& GameModel::egpBids() const
{
    return m_egpBids;
}

void GameModel::clearEgpBids()
{
    m_egpBids.clear();
    emit egpBidsChanged(m_egpBids);
}

QVector<EgpAllocation> GameModel::allocateEgp()
{
    QVector<EgpAllocation> result;

    int remainingDemand = m_market.egpDemand;
    if (remainingDemand <= 0)
        return result;

    auto bids = m_egpBids;

    std::sort(bids.begin(), bids.end(),
              [](const EgpBid& a, const EgpBid& b) {
                  return a.price < b.price;
              });

    for (const auto& bid : bids)
    {
        if (remainingDemand <= 0)
            break;

        int amount = qMin(bid.amount, remainingDemand);
        if (amount <= 0)
            continue;

        EgpAllocation a;
        a.playerId  = bid.playerId;
        a.allocated = amount;
        a.price     = bid.price;

        result.push_back(a);
        remainingDemand -= amount;
    }

    return result;
}

void GameModel::applyEgpAllocations(const QVector<EgpAllocation>& allocations)
{
    for (const auto& a : allocations)
    {
        for (auto& player : m_players)
        {
            if (player.getId() != a.playerId)
                continue;

            int available = player.getEgp();
            int sold = qMin(a.allocated, available);

            if (sold <= 0)
                continue;

            int income = sold * a.price;

            player.setEgp(player.getEgp() - sold);
            player.setBalance(player.getBalance() + income);

            qDebug() << "[BANK] EGP sold:"
                     << "playerId =" << player.getId()
                     << "amount =" << sold
                     << "income =" << income
                     << "balance =" << player.getBalance();
        }
    }

    clearEgpBids();
}

void GameModel::applyLoanInterest()
{
    for (auto& player : m_players)
    {
        int totalDebt = player.totalLoanAmount();
        if (totalDebt <= 0)
            continue;

        int interest = totalDebt / 100;

        if (interest <= 0)
            continue;

        player.setBalance(player.getBalance() - interest);

        qDebug() << "[BANK] Loan interest paid:"
                 << "playerId =" << player.getId()
                 << "interest =" << interest
                 << "balance =" << player.getBalance();
    }

}

void GameModel::repayDueLoans()
{
    const int currentMonth = m_month;

    for (auto& player : m_players)
    {
        QVector<Loan> remainingLoans;

        for (const auto& loan : player.loans())
        {
            if (loan.endMonth > currentMonth)
            {
                remainingLoans.push_back(loan);
                continue;
            }

            // Ссуда подлежит погашению
            player.setBalance(player.getBalance() - loan.amount);

            qDebug() << "[BANK] Loan repaid:"
                     << "playerId =" << player.getId()
                     << "amount =" << loan.amount
                     << "balance =" << player.getBalance();
        }

        player.setLoans(remainingLoans);
    }
}

bool GameModel::takeLoan(int playerId, int amount)
{
    for (auto& player : m_players)
    {
        if (player.getId() != playerId)
            continue;

        if (player.isBankrupt())
        {
            qDebug() << "[BANK] Loan denied: player is bankrupt";
            return false;
        }

        int guaranteedCapital = 0;
        for (const auto& f : player.factories())
        {
            guaranteedCapital +=
                (f.type == FactoryType::Normal) ? 5000 : 10000;
        }

        int maxDebt = guaranteedCapital / 2;
        int currentDebt = player.totalLoanAmount();

        if (currentDebt + amount > maxDebt)
        {
            qDebug() << "[BANK] Loan denied: limit exceeded";
            return false;
        }

        Loan loan;
        loan.amount = amount;
        loan.startMonth = m_month;
        loan.endMonth = m_month + 12;

        player.addLoan(loan);
        player.setBalance(player.getBalance() + amount);

        qDebug() << "[BANK] Loan taken:"
                 << "playerId =" << playerId
                 << "amount =" << amount
                 << "balance =" << player.getBalance();

        return true;
    }

    return false;
}

void GameModel::enterState(GameState state)
{
    switch (state)
    {
    case GameState::PayFixedCosts:
        payFixedCosts();
        break;

    case GameState::MarketDetermination:
        recalcMarket(m_players.size());
        break;

    case GameState::RawMaterialAllocation:
        applyEsmAllocations(allocateEsm());
        clearEsmBids();
        checkBankruptcies();
        break;

    case GameState::ProductionExecution:
        limitProductionByFactories();
        executeProduction();
        break;

    case GameState::ProductAllocation:
        applyEgpAllocations(allocateEgp());
        clearEgpBids();
        checkBankruptcies();
        break;

    case GameState::LoanInterestPayment:
        applyLoanInterest();
        checkBankruptcies();
        break;

    case GameState::LoanRepayment:
        repayDueLoans();
        checkBankruptcies();
        break;

    default:
        break;
    }
}

void GameModel::payFixedCosts()
{
    for (auto& player : m_players)
    {
        if (player.isBankrupt())
            continue;

        int cost = 0;

        cost += player.getEsm() * 300;
        cost += player.getEgp() * 500;
        cost += player.factories().size() * 1000;

        player.setBalance(player.getBalance() - cost);

        qDebug() << "[BANK] Fixed costs paid:"
                 << "playerId =" << player.getId()
                 << "cost =" << cost
                 << "balance =" << player.getBalance();
    }

    checkBankruptcies();
}
