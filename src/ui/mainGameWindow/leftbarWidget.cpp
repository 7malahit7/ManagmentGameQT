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
    // Удаляем все виджеты игроков между полоской и stretch
    int count = mainLayout->count();
    // 0 - playersLabel, 1 - headerLine, count-1 - stretch
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
void LeftBarWidget::addNewPlayer(const PlayerModel &playerInfo)
{
    PlayerWidget* player = new PlayerWidget(playerInfo.getId(),
                                            playerInfo.getName(),
                                            playerInfo.getLevel(),
                                            playerInfo.getBalance(),
                                            playerInfo.getEgp(),
                                            playerInfo.getEsm(),
                                            playerInfo.getStatus(),
                                            this);
    mainLayout->insertWidget(mainLayout->count() - 1, player);
}

void LeftBarWidget::updatePlayers(const QVector<PlayerModel>& players)
{
    clearPlayers();
    for (const auto& p : players)
    {
        qDebug() << "added player";
        addNewPlayer(p);
    }
}
