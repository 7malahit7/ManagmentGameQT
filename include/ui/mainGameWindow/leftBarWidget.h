#pragma once

#include <QWidget>
#include <QVBoxLayout>
#include <QVector>
#include <QSplitter>
#include <QStyle>

#include "playerWidget.h"


class LeftBarWidget : public QWidget
{
    Q_OBJECT
private:
    QLabel* playersLabel;
    QVBoxLayout* mainLayout;
    QVector<PlayerWidget*> players;
public:
    explicit LeftBarWidget(QWidget* parent = nullptr);

    ~LeftBarWidget() = default;
signals:

};


