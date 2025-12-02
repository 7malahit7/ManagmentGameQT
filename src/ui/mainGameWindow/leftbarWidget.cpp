#include "mainGameWindow/leftBarWidget.h"

LeftBarWidget::LeftBarWidget(const QVector<PlayerWidget *>& players, QWidget *parent)
    : QWidget(parent)
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

    for(const auto &player : players){
        QFrame* line = new QFrame(this);
        line->setFrameShape(QFrame::HLine);
        line->setFrameShadow(QFrame::Sunken);

        player->setSizePolicy(QSizePolicy::Preferred, QSizePolicy::Fixed);

        mainLayout->addWidget(player);
        mainLayout->addSpacing(10);
        mainLayout->addWidget(line);
        mainLayout->addSpacing(10);
    }
    mainLayout->addStretch();

}
