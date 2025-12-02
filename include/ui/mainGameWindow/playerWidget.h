#pragma once

#include <QWidget>
#include <QLabel>
#include <QGridLayout>
#include <QStyle>

class PlayerWidget : public QWidget
{

    Q_OBJECT
private:
    QGridLayout* mainLayout;

    QLabel* playerName;
    QLabel* playerStatus;
    QLabel* playerLevel;
    QLabel* playerEsm;
    QLabel* playerEgp;
    QLabel* playerBalance;

public:
    explicit PlayerWidget(int id,
                          const QString& name,
                          int level,
                          int balance,
                          int egp,
                          int esm,
                          QWidget* parent = nullptr);

    void updateLevel(int level);
    void updateEsm(int esm);
    void updateEgp(int egp);
    void updateBalance(int balance);
    void updateStatus(const QString& status);

    void updateItems(int level, int esm, int egp, int balance, const QString& status);
    ~PlayerWidget() = default;
signals:

};
