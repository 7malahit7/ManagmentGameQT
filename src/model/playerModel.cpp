#include "playerModel.h"

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
bool PlayerModel::isBankrupt() const
{
    return m_isBankrupt;
}

void PlayerModel::setBankrupt(bool bankrupt)
{
    m_isBankrupt = bankrupt;
}
const QVector<Factory>& PlayerModel::factories() const
{
    return m_factories;
}

void PlayerModel::addFactory(const Factory& factory)
{
    m_factories.push_back(factory);
}

int PlayerModel::factoryCount() const
{
    return m_factories.size();
}

int PlayerModel::maxProductionCapacity() const
{
    int capacity = 0;

    for (const auto& factory : m_factories)
    {
        if (factory.type == FactoryType::Normal)
            capacity += 1;
        else if (factory.type == FactoryType::Automated)
            capacity += 2;
    }

    return capacity;
}

const QVector<Loan>& PlayerModel::loans() const
{
    return m_loans;
}

void PlayerModel::addLoan(const Loan& loan)
{
    m_loans.push_back(loan);
}

int PlayerModel::totalLoanAmount() const
{
    int sum = 0;
    for (const auto& loan : m_loans)
        sum += loan.amount;
    return sum;
}

void PlayerModel::setLoans(const QVector<Loan>& loans)
{
    m_loans = loans;
}
bool PlayerModel::isReady() const
{
    return m_ready;
}

void PlayerModel::setReady(bool ready)
{
    m_ready = ready;
}
