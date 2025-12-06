#pragma once
#include <QWidget>
#include <QHBoxLayout>
#include <QLabel>

class HeaderWidget : public QWidget
{
    Q_OBJECT
private:
    QHBoxLayout* mainLayout;
    QLabel* currentMonth;
    QLabel* currentPhase;

public:
    explicit HeaderWidget(const QString& month, const QString& phase, QWidget* parent = nullptr);
    void setMonth(const QString &month);
    void setPhase(const QString &phase);
};
