#include "playerWidget.h"

PlayerWidget::PlayerWidget(int id,
                           const QString& name,
                           int level,
                           int balance,
                           int egp,
                           int esm,
                           const QString& status,
                           QWidget* parent)
    : QWidget(parent)
{
    mainLayout = new QGridLayout(this);

    playerName = new QLabel(name, this);
    playerLevel = new QLabel(QString("%1 Уровень").arg(level), this);
    playerBalance = new QLabel(QString("%1$").arg(balance), this);
    playerEgp = new QLabel(QString("%1 ЕГП").arg(egp), this);
    playerEsm = new QLabel(QString("%1 ЕСМ").arg(esm), this);
    playerStatus = new QLabel(status, this);

    playerBalance->setStyleSheet("color: lightgreen; font-family: Segoe UI; font-size: 14pt;");

    mainLayout->addWidget(playerName,0,0,1,2);
    mainLayout->addWidget(playerBalance,0,2);
    mainLayout->addWidget(playerLevel,1,0);
    mainLayout->addWidget(playerEgp,1,1);
    mainLayout->addWidget(playerEsm,1,2);
    mainLayout->addWidget(playerStatus,2,0,1,3);

    mainLayout->setSpacing(10);
}

void PlayerWidget::updateLevel(int level) { playerLevel->setText(QString("%1 Уровень").arg(level)); }
void PlayerWidget::updateEsm(int esm) { playerEsm->setText(QString("%1 ЕСМ").arg(esm)); }
void PlayerWidget::updateEgp(int egp) { playerEgp->setText(QString("%1 ЕГП").arg(egp)); }
void PlayerWidget::updateBalance(int balance) { playerBalance->setText(QString("%1$").arg(balance)); }
void PlayerWidget::updateStatus(const QString &status) { playerStatus->setText(status); }
void PlayerWidget::updateItems(int level, int esm, int egp, int balance, const QString &status)
{
    updateLevel(level);
    updateEsm(esm);
    updateEgp(egp);
    updateBalance(balance);
    updateStatus(status);
}
