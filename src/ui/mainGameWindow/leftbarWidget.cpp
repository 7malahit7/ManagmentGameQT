#include "leftBarWidget.h"
#include <QFrame>

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

void LeftBarWidget::addNewPlayer(quint8 id, const QString &name)
{
    PlayerWidget* player = new PlayerWidget(id, name, 1, 10000, 10,10,this);
    mainLayout->insertWidget(mainLayout->count() - 1, player);
}

void LeftBarWidget::updatePlayers(const QVector<PlayerModel>& players)
{
    clearPlayers();
    for (const auto& p : players)
    {
        qDebug() << "added player";
        addNewPlayer(p.getId(), p.getName());
    }
}
