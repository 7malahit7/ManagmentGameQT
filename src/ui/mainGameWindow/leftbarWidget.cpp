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
