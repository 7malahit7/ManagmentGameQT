#pragma once

#include <QObject>
#include <QWidget>
#include <QVBoxLayout>
#include <QVector>
#include <QSplitter>
#include <QStyle>

#include "mainGameWindow/playerWidget.h"


class LeftBarWidget : public QWidget
{
    Q_OBJECT
private:
    QLabel* playersLabel;
    QVBoxLayout* mainLayout;
    QVector<PlayerWidget*> players;
public:
    explicit LeftBarWidget(const QVector<PlayerWidget*> &players, QWidget* parent = nullptr);

    ~LeftBarWidget() = default;
signals:

};


