#include "leftBarWidget.h"
#include <QFrame>
#include <QDebug>

LeftBarWidget::LeftBarWidget(QWidget *parent) : QWidget(parent)
{
    playersLabel = new QLabel("Игроки", this);
    QFrame* headerLine = new QFrame(this);

    mainLayout = new QVBoxLayout(this);
    playersLabel->setAlignment(Qt::AlignCenter);
    mainLayout->addWidget(playersLabel);
    mainLayout->addWidget(headerLine, 0, Qt::AlignCenter);
    mainLayout->addSpacing(35);

    headerLine->setFrameShape(QFrame::HLine);
    headerLine->setFrameShadow(QFrame::Sunken);
    headerLine->setFixedWidth(300);
    headerLine->setStyleSheet("background-color: red;");

    mainLayout->addStretch();
}

void LeftBarWidget::clearPlayers()
{
    int count = mainLayout->count();
    for (int i = count - 2; i > 1; --i)
    {
        QLayoutItem* item = mainLayout->takeAt(i);
        if (item)
        {
            if (QWidget* w = item->widget())
                w->deleteLater();
            delete item;
        }
    }
}
void LeftBarWidget::addNewPlayer(const PlayerModel &playerInfo, const QString& status)
{
    PlayerWidget* player = new PlayerWidget(playerInfo.getId(),
                                            playerInfo.getName(),
                                            playerInfo.getLevel(),
                                            playerInfo.getBalance(),
                                            playerInfo.getEgp(),
                                            playerInfo.getEsm(),
                                            status,
                                            this);
    mainLayout->insertWidget(mainLayout->count() - 1, player);

}

void LeftBarWidget::updatePlayers(const QVector<PlayerModel>& players)
{
    qDebug() << "[UI] Players list updated:";

    clearPlayers();

    for (const auto& p : players)
    {
        QString statusText;

        if (p.isBankrupt())
            statusText = "Банкрот";
        else if (p.isReady())
            statusText = "Готов";
        else
            statusText = "Ожидает...";

        qDebug()
            << " playerId =" << p.getId()
            << " name =" << p.getName()
            << " ready =" << p.isReady()
            << " bankrupt =" << p.isBankrupt();

        addNewPlayer(p, statusText);
    }
}
