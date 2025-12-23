#pragma once

#include <QString>
#include <QJsonDocument>
#include <QJsonObject>



enum class FactoryType
{
    Normal,
    Automated
};
struct Factory
{
    FactoryType type = FactoryType::Normal;
};

//ссуды
struct Loan
{
    int amount = 0;
    int startMonth = 0;
    int endMonth = 0;
};

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

    bool isBankrupt() const;
    void setBankrupt(bool bankrupt);

    QJsonDocument toJson() const;
    QJsonObject toJsonObject() const;
    static PlayerModel fromJson(const QJsonObject &obj);

    const QVector<Factory>& factories() const;
    void addFactory(const Factory& factory);
    int factoryCount() const;

    int maxProductionCapacity() const;

    const QVector<Loan>& loans() const;
    void addLoan(const Loan& loan);
    int totalLoanAmount() const;
    void setLoans(const QVector<Loan>& loans);

    bool isReady() const;
    void setReady(bool ready);


private:
    bool m_ready = false;

    QString name;
    bool isServer;
    quint8 id;
    int level;
    int balance;
    int egp;
    int esm;
    QString status;

    bool m_isBankrupt = false;

    QVector<Factory> m_factories;

    QVector<Loan> m_loans;
};


