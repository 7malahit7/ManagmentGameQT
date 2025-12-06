#include "headerWidget.h"

HeaderWidget::HeaderWidget(const QString& month, const QString& phase, QWidget* parent) : QWidget(parent)
{
    mainLayout = new QHBoxLayout(this);
    currentMonth = new QLabel(QString("Месяц: %1").arg(month), this);
    currentPhase = new QLabel(QString("Фаза: %1").arg(phase), this);

    mainLayout->addWidget(currentMonth);
    mainLayout->addSpacing(100);
    mainLayout->addWidget(currentPhase);
    mainLayout->setAlignment(Qt::AlignCenter);
}

void HeaderWidget::setMonth(const QString &month)
{
    currentMonth->setText(QString("Месяц: %1").arg(month));
}

void HeaderWidget::setPhase(const QString &phase)
{
    currentPhase->setText(QString("Фаза: %1").arg(phase));
}
